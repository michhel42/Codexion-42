/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:12:33 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 06:04:38 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	verif_smallest(t_heap *heap, int id, int smallest)
{
	return ((id < heap->size)
		&& (heap->data[id].priority < heap->data[smallest].priority
			|| (heap->data[id].priority == heap->data[smallest].priority
				&& heap->data[id].coder_id < heap->data[smallest].coder_id)));
}

void	heapify_down(t_heap *heap, int i)
{
	int	smallest;
	int	left;
	int	right;

	smallest = i;
	left = 2 * i + 1;
	right = 2 * i + 2;
	if (verif_smallest(heap, left, smallest))
		smallest = left;
	if (verif_smallest(heap, right, smallest))
		smallest = right;
	if (smallest != i)
	{
		swap_nodes(&heap->data[i], &(heap->data[smallest]));
		heapify_down(heap, smallest);
	}
}

t_heap_node	heap_pop(t_heap *heap)
{
	t_heap_node	root;

	if (heap->size <= 0)
	{
		root.coder_id = -1;
		return (root);
	}
	root = heap->data[0];
	if (heap->size == 1)
	{
		heap->size--;
		return (root);
	}
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	heapify_down(heap, 0);
	return (root);
}

void	heap_push(t_heap *heap, long long priority, int coder_id)
{
	int	i;

	i = heap->size;
	if (i >= heap->capacity)
		return ;
	heap->data[i].priority = priority;
	heap->data[i].coder_id = coder_id;
	heap->size++;
	while (i != 0 && heap->data[i].priority < heap->data[(i - 1) / 2].priority)
	{
		swap_nodes(&heap->data[i], &heap->data[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

void	heap_remove_coder(t_heap *heap, int coder_id)
{
	int	i;

	i = 0;
	while (i < heap->size)
	{
		if (heap->data[i].coder_id == coder_id)
			break ;
		i++;
	}
	if (i == heap->size)
		return ;
	if (i == heap->size - 1)
	{
		heap->size--;
		return ;
	}
	heap->data[i] = heap->data[heap->size - 1];
	heap->size--;
	heapify_down(heap, i);
}
