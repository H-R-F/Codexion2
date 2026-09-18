/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by coder             #+#    #+#             */
/*   Updated: 2026/09/18 16:07:13 by hrf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_simulation_stopped(t_simulation *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->state_mutex);
	stopped = sim->stop_simulation;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stopped);
}

void	heap_free(t_heap *heap)
{
	if (heap->nodes)
	{
		free(heap->nodes);
		heap->nodes = NULL;
	}
	heap->capacity = 0;
	heap->size = 0;
}

void	clean_simulation(t_simulation *sim)
{
	int	i;

	if (sim->dongles)
	{
		i = 0;
		while (i < sim->num_coders)
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			heap_free(&sim->dongles[i].queue);
			i++;
		}
		free(sim->dongles);
		sim->dongles = NULL;
	}
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
	if (sim->mutexes_init)
	{
		pthread_mutex_destroy(&sim->state_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
	}
}
