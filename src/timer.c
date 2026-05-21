/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 05:57:36 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 06:08:36 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL));
}

void	ft_usleep(long long time_in_ms)
{
	long long	start_time;

	start_time = get_time_ms();
	while ((get_time_ms() - start_time) < time_in_ms)
		usleep(500);
}

int	is_cooldown_over(t_dongle *dongle, long long duration)
{
	long long	current_time;

	current_time = get_time_ms();
	if (dongle->last_released_time == 0)
		return (1);
	return ((current_time - dongle->last_released_time) >= duration);
}
