/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:12:30 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 08:01:05 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_data *data)
{
	int	i;
	int	num_c;

	num_c = data->num_coders;
	data->dongles = malloc(sizeof(t_dongle) * num_c);
	if (!data->dongles)
		return (0);
	i = 0;
	while (i < num_c)
	{
		if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
			return (0);
		if (pthread_cond_init(&data->dongles[i].cond, NULL) != 0)
			return (0);
		data->dongles[i].last_released_time = 0;
		data->dongles[i].queue.capacity = num_c;
		data->dongles[i].queue.size = 0;
		data->dongles[i].queue.data = malloc(sizeof(t_heap_node) * num_c);
		if (!data->dongles[i].queue.data)
			return (0);
		i++;
	}
	return (1);
}

static int	init_coders(t_data *data)
{
	int	i;
	int	num_c;

	num_c = data->num_coders;
	data->coders = malloc(sizeof(t_coder) * num_c);
	if (!data->coders)
		return (0);
	i = 0;
	while (i < num_c)
	{
		data->coders[i].id = i + 1;
		data->coders[i].nb_compiles = 0;
		data->coders[i].global_data = data;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = &data->dongles[(i + 1) % num_c];
		i++;
	}
	return (1);
}

int	init_data(t_data *data, char **av, int ac)
{
	data->num_coders = ft_atoi(av[1]);
	data->time_to_burnout = ft_atoi(av[2]);
	data->time_to_compile = ft_atoi(av[3]);
	data->time_to_debug = ft_atoi(av[4]);
	data->time_to_refactor = ft_atoi(av[5]);
	data->num_compiles_required = -1;
	data->coders = NULL;
	data->dongles = NULL;
	if (ac == 9)
		data->num_compiles_required = ft_atoi(av[6]);
	data->dongle_cooldown = ft_atoi(av[ac - 2]);
	if (ft_strcmp(av[ac - 1], "fifo") == 0)
		data->sheduler_type = SHEDULER_FIFO;
	else
		data->sheduler_type = SHEDULER_EDF;
	data->simulation_running = 1;
	if (pthread_mutex_init(&data->state_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->log_mutex, NULL) != 0)
		return (0);
	if (!init_dongles(data) || !init_coders(data))
		return (0);
	return (1);
}

void	init_coders_start(t_data *data)
{
	long long	burn_out;
	int			i;

	burn_out = data->start - data->time_to_burnout - 1;
	i = 0;
	while (i < data->num_coders)
	{
		data->coders[i].last_compile_start = data->start;
		i++;
	}
}
