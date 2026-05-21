/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:12:24 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 06:05:50 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wake_up_everyone(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

static int	check_burnout(t_coder *coder, t_data *data)
{
	long long	current_time;
	long long	last_compile;

	current_time = get_time_ms();
	pthread_mutex_lock(&data->state_mutex);
	last_compile = coder->last_compile_start;
	pthread_mutex_unlock(&data->state_mutex);
	if ((current_time - last_compile) > data->time_to_burnout)
	{
		if (!is_simulation_over(data))
		{
			print_log(data, coder->id, BURN_OUT);
			pthread_mutex_lock(&data->state_mutex);
			data->simulation_running = 0;
			pthread_mutex_unlock(&data->state_mutex);
			wake_up_everyone(data);
		}
		return (1);
	}
	return (0);
}

static int	check_all_coders(t_data *data, int *all_finished)
{
	int	i;

	i = 0;
	*all_finished = 1;
	while (i < data->num_coders)
	{
		if (check_burnout(&data->coders[i], data))
			return (1);
		pthread_mutex_lock(&data->state_mutex);
		if (data->num_compiles_required == -1
			|| data->coders[i].nb_compiles < data->num_compiles_required)
			*all_finished = 0;
		pthread_mutex_unlock(&data->state_mutex);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		all_finished;

	data = (t_data *)arg;
	while (1)
	{
		if (check_all_coders(data, &all_finished))
			return (NULL);
		if (data->num_compiles_required != -1 && all_finished)
		{
			pthread_mutex_lock(&data->state_mutex);
			data->simulation_running = 0;
			pthread_mutex_unlock(&data->state_mutex);
			wake_up_everyone(data);
			return (NULL);
		}
		usleep(3000);
	}
	return (NULL);
}
