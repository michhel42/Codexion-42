/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vihardy <vihardy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 23:55:25 by vihardy           #+#    #+#             */
/*   Updated: 2026/05/21 06:07:29 by vihardy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_valid_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || str[i] == '\0')
		return (0);
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static int	check_all_numeric(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac - 1)
	{
		if (!is_valid_numeric(av[i]))
		{
			write(2, "ERROR: Arguments must be positive integers.\n", 44);
			return (0);
		}
		i++;
	}
	return (1);
}

int	parser_checker(int ac, char **av)
{
	if (ac != 8 && ac != 9)
	{
		write(2, "ERROR: Invalid number of arguments.\n", 36);
		return (0);
	}
	if (!check_all_numeric(ac, av))
		return (0);
	if (ft_atoi(av[1]) < 1)
	{
		write(2, "Error: There must be at least 1 coder.\n", 39);
		return (0);
	}
	if (ft_strcmp(av[ac - 1], "fifo") != 0 && ft_strcmp(av[ac - 1], "edf") != 0)
	{
		write(2, "ERROR: Sheduler must be exactly 'fifo' or 'edf'.\n", 50);
		return (0);
	}
	return (1);
}
