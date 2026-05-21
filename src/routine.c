/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:12:22 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 08:01:47 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_simulation_over(t_data *data)
{
	int	over;

	pthread_mutex_lock(&data->state_mutex);
	over = !data->simulation_running;
	pthread_mutex_unlock(&data->state_mutex);
	return (over);
}

static void	coder_action(t_coder *coder, char *action, long long time)
{
	print_log(coder->global_data, coder->id, action);
	ft_usleep(time);
}

static int	dongle_choice(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		if (!acquire_dongle(coder, coder->left_dongle))
			return (0);
		if (coder->left_dongle == coder->right_dongle)
			return (0);
		if (!acquire_dongle(coder, coder->right_dongle))
		{
			release_dongle(coder->left_dongle, coder->id);
			return (0);
		}
	}
	else
	{
		if (!acquire_dongle(coder, coder->right_dongle))
			return (0);
		if (coder->left_dongle == coder->right_dongle)
			return (0);
		if (!acquire_dongle(coder, coder->left_dongle))
		{
			release_dongle(coder->right_dongle, coder->id);
			return (0);
		}
	}
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)arg;
	data = coder->global_data;
	if (coder->id % 2 == 0)
		ft_usleep(1);
	while (!is_simulation_over(data))
	{
		if (!dongle_choice(coder))
			break ;
		pthread_mutex_lock(&data->state_mutex);
		coder->last_compile_start = get_time_ms();
		pthread_mutex_unlock(&data->state_mutex);
		coder_action(coder, COMPILE, data->time_to_compile);
		pthread_mutex_lock(&data->state_mutex);
		coder->nb_compiles++;
		pthread_mutex_unlock(&data->state_mutex);
		release_dongles(coder);
		if (is_simulation_over(data))
			break ;
		coder_action(coder, DEBUG, data->time_to_debug);
		coder_action(coder, REFACTOR, data->time_to_refactor);
	}
	return (NULL);
}
