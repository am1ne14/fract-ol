/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 01:48:17 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/28 14:21:17 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	error_exit(char *msg)
{
	ft_putstr_fd(msg, 1);
	exit(EXIT_FAILURE);
}

void	usage(void)
{
	ft_putstr_fd("  Usage:\n", 1);
	ft_putstr_fd("\t./fractol mandelbrot\n", 1);
	ft_putstr_fd("\t./fractol Julia <real_part> <imaginary_part>\n", 1);
	exit(EXIT_FAILURE);
}
