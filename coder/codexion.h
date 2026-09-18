/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by coder             #+#    #+#             */
/*   Updated: 2026/09/18 02:28:07 by hrf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

# define SCHEDULER_FIFO 0
# define SCHEDULER_EDF  1
# define WITH_LOCK 1
# define NO_LOCK   0

/*
** Priority Queue Node representing a dongle acquisition request.
*/
typedef struct s_heap_node
{
	int			coder_id;
	long long	request_time;
	long long	deadline;
}	t_heap_node;

/*
** Priority Queue (Min-Heap) for managing dongle request arbitration.
*/
typedef struct s_heap
{
	t_heap_node	*nodes;
	int			capacity;
	int			size;
	int			scheduler_type;
}	t_heap;

static inline int	is_higher_priority(t_heap *h, int a, int b)
{
	if (h->scheduler_type == SCHEDULER_EDF
		&& h->nodes[a].deadline != h->nodes[b].deadline)
		return (h->nodes[a].deadline < h->nodes[b].deadline);
	if (h->nodes[a].request_time != h->nodes[b].request_time)
		return (h->nodes[a].request_time < h->nodes[b].request_time);
	return (h->nodes[a].coder_id < h->nodes[b].coder_id);
}

struct	s_simulation;

/*
** Dongle structure holding state, mutex, condition variable, and request queue.
*/
typedef struct s_dongle
{
	int					id;
	int					in_use;
	long long			last_released_ms;
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	t_heap				queue;
}	t_dongle;

/*
** Coder structure storing state, timings, and thread reference.
*/
typedef struct s_coder
{
	int					id;
	int					compiles_count;
	long long			last_compile_start;
	pthread_t			thread;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	struct s_simulation	*sim;
}	t_coder;

/*
** Main Simulation structure containing all shared data and configuration.
*/
typedef struct s_simulation
{
	int				num_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				compiles_required;
	long long		dongle_cooldown;
	int				scheduler;
	int				stop_simulation;
	long long		start_time;
	int				mutexes_init;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	log_mutex;
	pthread_t		monitor_thread;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_simulation;

/* --- utils.c --- */
long long	get_time_ms(void);
void		smart_usleep(long long time_in_ms, t_simulation *sim);
void		print_log(t_simulation *sim, int coder_id, const char *status);
int			ft_atoi_positive(const char *str);

/* --- heap.c --- */
int			heap_init(t_heap *heap, int capacity, int scheduler_type);
void		heap_free(t_heap *heap);
int			heap_push(t_heap *heap, t_heap_node node);
int			heap_peek(t_heap *heap, t_heap_node *out_node);
int			heap_pop(t_heap *heap, t_heap_node *out_node);

/* --- init.c --- */
int			parse_args(int argc, char **argv, t_simulation *sim);
int			init_simulation(t_simulation *sim);

/* --- dongles.c --- */
int			acquire_dongles(t_coder *coder);
void		release_dongles(t_coder *coder);

/* --- coder.c --- */
void		*coder_routine(void *arg);

/* --- monitor.c --- */
void		*monitor_routine(void *arg);

/* --- cleanup.c --- */
void		clean_simulation(t_simulation *sim);
int			is_simulation_stopped(t_simulation *sim);

#endif
