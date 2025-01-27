/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 12:27:11 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/27 01:13:40 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}

double	ft_atof(const char *str)
{
	double	res;
	int		sign;
	double	divider;

	res = 0.0;
	sign = 1;
	divider = 1.0;
	parse_integer_part(&str, &res, &sign);
	parse_fractional_part(&str, &res, &divider);
	return (sign * res / divider);
}

void	parse_fractional_part(const char **str, double *res, double *divider)
{
	if (**str == '.')
	{
		(*str)++;
		while (**str >= '0' && **str <= '9')
		{
			*res = *res * 10 + (**str - '0');
			*divider *= 10;
			(*str)++;
		}
	}
}

void	parse_integer_part(const char **str, double *res, int *sign)
{
	while (is_space(**str))
		(*str)++;
	if (**str == '-')
		*sign = -1;
	if (**str == '-' || **str == '+')
		(*str)++;
	while (**str >= '0' && **str <= '9')
	{
		*res = *res * 10 + (**str - '0');
		(*str)++;
	}
}
