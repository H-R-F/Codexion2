/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrf <hrf@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 21:57:00 by coder             #+#    #+#             */
/*   Updated: 2026/09/17 20:16:55 by hrf              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *heap, int capacity, int scheduler_type)
{
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->nodes)
		return (-1);
	heap->capacity = capacity;
	heap->size = 0;
	heap->scheduler_type = scheduler_type;
	return (0);
}

int	heap_push(t_heap *heap, t_heap_node node)
{
	int			i;
	int			parent;
	t_heap_node	tmp;

	if (heap->size >= heap->capacity)
		return (-1);
	i = heap->size;
	heap->nodes[i] = node;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (is_higher_priority(heap, i, parent))
		{
			tmp = heap->nodes[i];
			heap->nodes[i] = heap->nodes[parent];
			heap->nodes[parent] = tmp;
			i = parent;
		}
		else
			break ;
	}
	return (0);
}

static void	sift_down(t_heap *heap, int i)
{
	int			l;
	int			r;
	int			s;
	t_heap_node	tmp;

	while (1)
	{
		l = 2 * i + 1;
		r = 2 * i + 2;
		s = i;
		if (l < heap->size && is_higher_priority(heap, l, s))
			s = l;
		if (r < heap->size && is_higher_priority(heap, r, s))
			s = r;
		if (s != i)
		{
			tmp = heap->nodes[i];
			heap->nodes[i] = heap->nodes[s];
			heap->nodes[s] = tmp;
			i = s;
		}
		else
			break ;
	}
}

int	heap_pop(t_heap *heap, t_heap_node *out_node)
{
	if (heap->size == 0)
		return (-1);
	*out_node = heap->nodes[0];
	heap->nodes[0] = heap->nodes[heap->size - 1];
	heap->size--;
	sift_down(heap, 0);
	return (0);
}

int	heap_peek(t_heap *heap, t_heap_node *out_node)
{
	if (heap->size == 0)
		return (-1);
	*out_node = heap->nodes[0];
	return (0);
}
