/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:15:47 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/16 14:26:06 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*
 *	int	is_number(char *str);
 *	int	check_duplicates(char **av);
 *	int	check_splitted_numbers(char **splitted_av);
 *	int	check_input(char **av);
 *	 void	free_split(char **av);
 * */

int	check_input(char **av)
{
	char	**splitted_av;
	char	*trimmed_arg;
	int		i;

	i = 1;
	while (av[i])
	{
		trimmed_arg = ft_strtrim(av[i], " ");
		if (ft_strlen(trimmed_arg) == 0)
		{
			free(trimmed_arg);
			exit_error();
		}
		free(trimmed_arg);
		splitted_av = ft_split(av[i], ' ');
		if (!check_splitted_numbers(splitted_av))
		{
			free_split(splitted_av);
			return (0);
		}
		free_split(splitted_av);
		i++;
	}
	return (1);
}

static int	check_int_range(char *str)
{
	long	num;

	num = ft_atol(str);
	if (num > INT_MAX || num < INT_MIN)
		return (0);
	return (1);
}

int	check_splitted_numbers(char **splitted_av)
{
	int	j;

	j = 0;
	while (splitted_av[j])
	{
		if (!is_number(splitted_av[j]))
			return (0);
		if (!check_int_range(splitted_av[j]))
			return (0);
		j++;
	}
	return (1);
}

int	is_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

long	ft_atol(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}
