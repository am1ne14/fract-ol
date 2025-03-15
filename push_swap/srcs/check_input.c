/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:15:47 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 14:18:18 by mohel-am         ###   ########.fr       */
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
	int		i;

	i = 1;
	while (av[i])
	{
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

void	free_split(char **av)
{
	int i = 0;

    if (av == NULL) {
        return;
    }

    while (av[i] != NULL) {
        free(av[i]);
        i++;
    }
    free(av);
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
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
		{
			if (sign == 1)
				return (LONG_MAX);
			else
				return (LONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}