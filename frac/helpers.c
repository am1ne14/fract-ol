/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 12:27:11 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/29 01:50:23 by mohel-am         ###   ########.fr       */
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

void	check_args(int ac, char **av, t_fractol *data)
{
	if (ac < 2 || av[1] == NULL )
		usage();
	if (ac == 2 && ft_strncmp(av[1], "mandelbrot", 10) == 0
		&& ft_strlen(av[1]) == 10)
		data->type = MANDELBROT;
	else if (ft_strncmp(av[1], "julia", 5) == 0 && ft_strlen(av[1]) == 5)
	{
		if (ac == 2)
		{
			data->julia_c.real = -0.7;
			data->julia_c.imaginaire = 0.27015;
		}
		else if (ac == 4)
		{
			data->julia_c.real = ft_atof(av[2]);
			data->julia_c.imaginaire = ft_atof(av[3]);
		}
		else
			usage();
		data->type = JULIA;
	}
	else
		usage();
}
