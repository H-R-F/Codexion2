/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by coder             #+#    #+#             */
/*   Updated: 2026/09/17 20:17:30 by hrf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	execute_compile_phase(t_coder *coder)
{
	if (acquire_dongles(coder) != 0)
		return (-1);
	pthread_mutex_lock(&coder->sim->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->sim->state_mutex);
	print_log(coder->sim, coder->id, "is compiling");
	smart_usleep(coder->sim->time_to_compile, coder->sim);
	release_dongles(coder);
	pthread_mutex_lock(&coder->sim->state_mutex);
	coder->compiles_count++;
	pthread_mutex_unlock(&coder->sim->state_mutex);
	return (0);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		smart_usleep(1, coder->sim);
	while (!is_simulation_stopped(coder->sim))
	{
		if (execute_compile_phase(coder) != 0)
			break ;
		if (is_simulation_stopped(coder->sim))
			break ;
		print_log(coder->sim, coder->id, "is debugging");
		smart_usleep(coder->sim->time_to_debug, coder->sim);
		if (is_simulation_stopped(coder->sim))
			break ;
		print_log(coder->sim, coder->id, "is refactoring");
		smart_usleep(coder->sim->time_to_refactor, coder->sim);
	}
	return (NULL);
}
