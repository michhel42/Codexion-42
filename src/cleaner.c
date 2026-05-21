/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 23:50:23 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 06:07:00 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_all(t_data *data)
{
	int	i;

	if (data->dongles)
	{
		i = 0;
		while (i < data->num_coders)
		{
			pthread_mutex_destroy(&data->dongles[i].mutex);
			pthread_cond_destroy(&data->dongles[i].cond);
			if (data->dongles[i].queue.data)
				free(data->dongles[i].queue.data);
			i++;
		}
		free(data->dongles);
	}
	if (data->coders)
		free(data->coders);
	pthread_mutex_destroy(&data->state_mutex);
	pthread_mutex_destroy(&data->log_mutex);
}
