/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:03:30 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/29 01:43:57 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	mandel_frac(t_fractol *data, int y)
{
	int		x;
	int		iter;
	int		color;
	int		pixel;
	t_comp	z;

	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			z.real = (x - WIDTH / 2.0) * 4.0 / WIDTH
				/ data->zoom + data->offset_x;
			z.imaginaire = (y - HEIGHT / 2.0) * 4.0 / HEIGHT
				/ data->zoom + data->offset_y;
			iter = iter_mandel(z, data->max_iter);
			color = color_swap(iter, data->max_iter, data->color_shift);
			pixel = (y * data->line_length) + (x * data->bpp / 8);
			*(int *)(data->img_addr + pixel) = color;
			x++;
		}
		y++;
	}
}

int	iter_mandel(t_comp z, int max_iter)
{
	int		iter;
	double	tmp;
	t_comp	c;

	iter = 0;
	c = z;
	while ((z.real * z.real + z.imaginaire * z.imaginaire <= 4)
		&& iter < max_iter)
	{
		tmp = z.real * z.real - z.imaginaire * z.imaginaire + c.real;
		z.imaginaire = 2 * z.real * z.imaginaire + c.imaginaire;
		z.real = tmp;
		iter++;
	}
	return (iter);
}
