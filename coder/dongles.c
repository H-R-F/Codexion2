/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by coder             #+#    #+#             */
/*   Updated: 2026/09/17 17:56:40 by hrf              ###  ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	can_take_dongles(t_coder *coder, long long now)
{
	t_heap_node	top_l;
	t_heap_node	top_r;
	t_dongle	*left;
	t_dongle	*right;
	long long	cd;

	left = coder->left_dongle;
	right = coder->right_dongle;
	cd = coder->sim->dongle_cooldown;
	if (left->in_use || right->in_use)
		return (0);
	if (now < left->last_released_ms + cd
		|| now < right->last_released_ms + cd)
		return (0);
	if (heap_peek(&left->queue, &top_l) != 0
		|| heap_peek(&right->queue, &top_r) != 0)
		return (0);
	if (top_l.coder_id != coder->id || top_r.coder_id != coder->id)
		return (0);
	return (1);
}

static void	order_and_lock(t_coder *coder, t_dongle **d1,
		t_dongle **d2, int do_lock)
{
	if (coder->left_dongle->id < coder->right_dongle->id)
	{
		*d1 = coder->left_dongle;
		*d2 = coder->right_dongle;
	}
	else
	{
		*d1 = coder->right_dongle;
		*d2 = coder->left_dongle;
	}
	if (coder->left_dongle == coder->right_dongle)
		*d2 = NULL;
	if (do_lock)
	{
		pthread_mutex_lock(&(*d1)->mutex);
		if (*d2 && *d2 != *d1)
			pthread_mutex_lock(&(*d2)->mutex);
	}
}

static int	acquire_loop(t_coder *coder, t_dongle *d1,
		t_dongle *d2, t_heap_node *req)
{
	long long	now;

	while (!is_simulation_stopped(coder->sim))
	{
		now = get_time_ms();
		if (d2 && can_take_dongles(coder, now))
		{
			coder->left_dongle->in_use = 1;
			coder->right_dongle->in_use = 1;
			heap_pop(&coder->left_dongle->queue, req);
			heap_pop(&coder->right_dongle->queue, req);
			print_log(coder->sim, coder->id, "has taken a dongle");
			print_log(coder->sim, coder->id, "has taken a dongle");
			return (1);
		}
		if (d2 && d2 != d1)
			pthread_mutex_unlock(&d2->mutex);
		pthread_mutex_unlock(&d1->mutex);
		usleep(500);
		order_and_lock(coder, &d1, &d2, WITH_LOCK);
	}
	return (0);
}

int	acquire_dongles(t_coder *coder)
{
	t_dongle		*d1;
	t_dongle		*d2;
	t_heap_node		req;

	order_and_lock(coder, &d1, &d2, WITH_LOCK);
	req.coder_id = coder->id;
	req.request_time = get_time_ms();
	req.deadline = coder->last_compile_start + coder->sim->time_to_burnout;
	heap_push(&coder->left_dongle->queue, req);
	if (d2)
		heap_push(&coder->right_dongle->queue, req);
	if (acquire_loop(coder, d1, d2, &req))
		return (0);
	if (d2 && d2 != d1)
		pthread_mutex_unlock(&d2->mutex);
	pthread_mutex_unlock(&d1->mutex);
	return (-1);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*d1;
	t_dongle	*d2;
	long long	now;

	order_and_lock(coder, &d1, &d2, NO_LOCK);
	now = get_time_ms();
	coder->left_dongle->last_released_ms = now;
	coder->right_dongle->last_released_ms = now;
	coder->left_dongle->in_use = 0;
	coder->right_dongle->in_use = 0;

	if (d2 && d2 != d1)
		pthread_mutex_unlock(&d2->mutex);
	pthread_mutex_unlock(&d1->mutex);
}
