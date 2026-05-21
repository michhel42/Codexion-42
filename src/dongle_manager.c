/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:12:35 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 08:01:01 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long long	find_priority(t_coder *coder)
{
	long long	last_compile;

	if (coder->global_data->sheduler_type == SHEDULER_FIFO)
		return (get_time_ms());
	else
	{
		pthread_mutex_lock(&coder->global_data->state_mutex);
		last_compile = coder->last_compile_start;
		pthread_mutex_unlock(&coder->global_data->state_mutex);
		return (last_compile + coder->global_data->time_to_burnout);
	}
}

static int	dongle_loop(t_coder *coder, t_dongle *dongle)
{
	while (1)
	{
		if (is_simulation_over(coder->global_data))
		{
			heap_remove_coder(&dongle->queue, coder->id);
			pthread_cond_broadcast(&dongle->cond);
			pthread_mutex_unlock(&dongle->mutex);
			return (0);
		}
		else if (dongle->queue.data[0].coder_id == coder->id)
		{
			if (is_cooldown_over(dongle, coder->global_data->dongle_cooldown))
				break ;
			pthread_mutex_unlock(&dongle->mutex);
			usleep(500);
			pthread_mutex_lock(&dongle->mutex);
			continue ;
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	return (1);
}

int	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	long long	priority;

	pthread_mutex_lock(&dongle->mutex);
	priority = find_priority(coder);
	if (is_simulation_over(coder->global_data))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	heap_push(&dongle->queue, priority, coder->id);
	if (!dongle_loop(coder, dongle))
		return (0);
	print_log(coder->global_data, coder->id, DONGLE);
	pthread_mutex_unlock(&dongle->mutex);
	return (1);
}

void	release_dongle(t_dongle *dongle, int coder_id)
{
	pthread_mutex_lock(&dongle->mutex);
	heap_remove_coder(&dongle->queue, coder_id);
	dongle->last_released_time = get_time_ms();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*right;
	t_dongle	*left;

	right = coder->right_dongle;
	left = coder->left_dongle;
	if (coder->id % 2 == 0)
	{
		release_dongle(right, coder->id);
		release_dongle(left, coder->id);
	}
	else
	{
		release_dongle(left, coder->id);
		release_dongle(right, coder->id);
	}
}
