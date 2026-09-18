/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by coder             #+#    #+#             */
/*   Updated: 2026/09/18 16:07:31 by hrf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_args(int argc, char **argv, t_simulation *sim)
{
	if (argc != 9)
		return (-1);
	sim->num_coders = ft_atoi_positive(argv[1]);
	sim->time_to_burnout = ft_atoi_positive(argv[2]);
	sim->time_to_compile = ft_atoi_positive(argv[3]);
	sim->time_to_debug = ft_atoi_positive(argv[4]);
	sim->time_to_refactor = ft_atoi_positive(argv[5]);
	sim->compiles_required = ft_atoi_positive(argv[6]);
	sim->dongle_cooldown = ft_atoi_positive(argv[7]);
	if (sim->num_coders <= 0)
		return (-1);
	if (sim->time_to_burnout < 0 || sim->time_to_compile < 0
		|| sim->time_to_debug < 0 || sim->time_to_refactor < 0
		|| sim->compiles_required < 1 || sim->dongle_cooldown < 0)
		return (-1);
	if (strcmp(argv[8], "fifo") == 0)
		sim->scheduler = SCHEDULER_FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		sim->scheduler = SCHEDULER_EDF;
	else
		return (-1);
	return (0);
}

static void	free_dongles_partial(t_simulation *sim, int done)
{
	int	i;

	i = 0;
	while (i < done)
	{
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		heap_free(&sim->dongles[i].queue);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

static int	init_dongles(t_simulation *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->num_coders);
	if (!sim->dongles)
		return (-1);
	i = 0;
	while (i < sim->num_coders)
	{
		sim->dongles[i].id = i + 1;
		sim->dongles[i].in_use = 0;
		sim->dongles[i].last_released_ms = 0;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		if (heap_init(&sim->dongles[i].queue, sim->num_coders,
				sim->scheduler) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			free_dongles_partial(sim, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	init_coders(t_simulation *sim)
{
	int	i;

	sim->coders = malloc(sizeof(t_coder) * sim->num_coders);
	if (!sim->coders)
		return (-1);
	i = 0;
	while (i < sim->num_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compiles_count = 0;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % sim->num_coders];
		sim->coders[i].sim = sim;
		i++;
	}
	return (0);
}

int	init_simulation(t_simulation *sim)
{
	sim->dongles = NULL;
	sim->coders = NULL;
	sim->mutexes_init = 0;
	sim->stop_simulation = 0;
	sim->start_time = 0;
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->state_mutex);
		return (-1);
	}
	sim->mutexes_init = 1;
	if (init_dongles(sim) != 0)
		return (-1);
	if (init_coders(sim) != 0)
		return (-1);
	return (0);
}
