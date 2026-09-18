/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by aben-sab          #+#    #+#             */
/*   Updated: 2026/09/17 20:17:30 by hrf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	broadcast_all_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

static int	check_all_compiles_done(t_simulation *sim)
{
	int	i;
	int	all_done;

	all_done = 1;
	i = 0;
	while (i < sim->num_coders)
	{
		if (sim->coders[i].compiles_count < sim->compiles_required)
		{
			all_done = 0;
			break ;
		}
		i++;
	}
	if (all_done)
	{
		sim->stop_simulation = 1;
		return (1);
	}
	return (0);
}

static int	check_burnout_loop(t_simulation *sim)
{
	int			i;
	int			burned_id;
	long long	now;

	i = 0;
	while (i < sim->num_coders)
	{
		now = get_time_ms();
		if (now - sim->coders[i].last_compile_start >= sim->time_to_burnout)
		{
			sim->stop_simulation = 1;
			burned_id = sim->coders[i].id;
			pthread_mutex_unlock(&sim->state_mutex);
			broadcast_all_dongles(sim);
			print_log(sim, burned_id, "burned out");
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;

	sim = (t_simulation *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->state_mutex);
		if (check_burnout_loop(sim))
			return (NULL);
		if (check_all_compiles_done(sim))
		{
			pthread_mutex_unlock(&sim->state_mutex);
			broadcast_all_dongles(sim);
			return (NULL);
		}
		pthread_mutex_unlock(&sim->state_mutex);
		usleep(1000);
	}
	return (NULL);
}
