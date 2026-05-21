/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:12:27 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 05:50:03 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	create_coders_threads(t_data *data)
{
	int		i;
	t_coder	*c;

	i = 0;
	while (i < data->num_coders)
	{
		c = &data->coders[i];
		pthread_create(&c->thread_id, NULL, coder_routine, c);
		i++;
	}
}

static void	join_coders_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_coders)
	{
		pthread_join(data->coders[i].thread_id, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data		config;
	pthread_t	monitor;

	if (!parser_checker(argc, argv))
		return (1);
	if (!init_data(&config, argv, argc))
	{
		write(2, "Error: Memory allocation fail!\n", 31);
		free_all(&config);
		return (1);
	}
	config.start = get_time_ms();
	init_coders_start(&config);
	create_coders_threads(&config);
	pthread_create(&monitor, NULL, monitor_routine, &config);
	join_coders_threads(&config);
	pthread_join(monitor, NULL);
	free_all(&config);
	return (0);
}
