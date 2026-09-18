/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by coder             #+#    #+#             */
/*   Updated: 2026/09/17 20:17:30 by hrf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	smart_usleep(long long time_in_ms, t_simulation *sim)
{
	long long	start;

	start = get_time_ms();
	while (!is_simulation_stopped(sim))
	{
		if (get_time_ms() - start >= time_in_ms)
			break ;
		usleep(500);
	}
}

void	print_log(t_simulation *sim, int coder_id, const char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&sim->log_mutex);
	pthread_mutex_lock(&sim->state_mutex);
	if (!sim->stop_simulation || strcmp(status, "burned out") == 0)
	{
		timestamp = get_time_ms() - sim->start_time;
		printf("%lld %d %s\n", timestamp, coder_id, status);
	}
	pthread_mutex_unlock(&sim->state_mutex);
	pthread_mutex_unlock(&sim->log_mutex);
}

int	ft_atoi_positive(const char *str)
{
	int			i;
	long long	res;

	if (!str || !str[0])
		return (-1);
	i = 0;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] < '0' || str[i] > '9')
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return ((int)res);
}
