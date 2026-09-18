/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by coder             #+#    #+#             */
/*   Updated: 2026/09/17 20:17:30 by hrf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start_threads(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL, coder_routine,
				&sim->coders[i]) != 0)
			return (-1);
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim) != 0)
		return (-1);
	return (0);
}

static void	join_threads(t_simulation *sim)
{
	int	i;

	pthread_join(sim->monitor_thread, NULL);
	i = 0;
	while (i < sim->num_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	int				i;

	if (parse_args(argc, argv, &sim) != 0)
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (init_simulation(&sim) != 0)
	{
		clean_simulation(&sim);
		return (1);
	}
	sim.start_time = get_time_ms();
	i = 0;
	while (i < sim.num_coders)
		sim.coders[i++].last_compile_start = sim.start_time;
	if (start_threads(&sim) != 0)
	{
		sim.stop_simulation = 1;
		clean_simulation(&sim);
		return (1);
	}
	join_threads(&sim);
	return (clean_simulation(&sim), 0);
}
