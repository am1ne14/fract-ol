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
			free(splitted_av);
			return (0);
		}
		free(splitted_av);
		i++;
	}
	if (!check_duplicates(av))
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
		j++;
	}
	return (1);
}

int	check_duplicates(char **av)
{
	int	i;
	int	j;
	int	num1;
	int	num2;

	i = 1;
	while (av[i])
	{
		num1 = ft_atoi(av[i]);
		j = i + 1;
		while (av[j])
		{
			num2 = ft_atoi(av[j]);
			if (num1 == num2)
				return (0);
			j++;
		}
		i++;
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
