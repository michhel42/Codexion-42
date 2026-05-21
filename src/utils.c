/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:12:19 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 06:06:32 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stddef.h>

void	print_log(t_data *data, int id, char *msg)
{
	pthread_mutex_lock(&data->log_mutex);
	if (!is_simulation_over(data))
		printf("%lld %d %s\n", get_time_ms() - data->start, id, msg);
	pthread_mutex_unlock(&data->log_mutex);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	int		op;
	int		total;

	i = 0;
	op = 1;
	total = 0;
	while (nptr[i] && ((nptr[i] >= 9 && nptr[i] <= 13)
			|| nptr[i] == 32))
		i++;
	if (nptr[i] && (nptr[i] == '+' || nptr[i] == '-'))
	{
		if (nptr[i] == '-')
			op = -op;
		i++;
	}
	while (nptr[i] != '\0' && nptr[i] >= '0' && nptr[i] <= '9')
	{
		total = (total * 10) + (nptr[i] - '0');
		i++;
	}
	return (op * total);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	swap_nodes(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}
