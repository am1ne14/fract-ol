/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:10:46 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/26 02:24:52 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	render(t_fractol *data)
{
	if (data->img)
		mlx_clear_window(data->mlx, data->win);
	if (data->type == MANDELBROT)
		mandel_frac(data, 0);
	else if (data->type == JULIA)
		julia_frac(data, 0);
	else
		error_exit("Invalid fractal type");
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int	color_swap(int iter, int max_iter, int color_shift)
{
	int		red;
	int		green;
	int		blue;
	double	t;

	if (iter == max_iter)
		return (0x000000);
	t = (double)iter / max_iter;
	red = (int)(sin(5 * t + color_shift) * 127 + 128);
	green = (int)(sin(5 * t + color_shift + 2) * 127 + 128);
	blue = (int)(sin(5 * t + color_shift + 4) * 127 + 128);
	return ((red << 16) | (green << 8) | blue);
}
