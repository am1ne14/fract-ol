/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 22:35:23 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/29 02:02:14 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	handle_key(int keycode, t_fractol *data)
{
	if (keycode == 65307)
	{
		clear_fractol(data);
	}
	else if (keycode == 67 || keycode == 99)
	{
		data->color_shift++;
		if (data->color_shift > 3)
			data->color_shift = 0;
		render(data);
	}
	return (0);
}

int	clear_fractol(t_fractol *data)
{
	if (!data)
		exit(0);
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_loop_end(data->mlx);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	exit(0);
}

int	handle_mouse(int button, int x, int y, t_fractol *data)
{
	double	mouse_real;
	double	mouse_imag;

	mouse_real = (x - WIDTH / 2.0) / (0.5 * data->zoom * WIDTH)
		+ data->offset_x;
	mouse_imag = (y - HEIGHT / 2.0) / (0.5 * data->zoom * HEIGHT)
		+ data->offset_y;
	if (button == 4)
		data->zoom *= 1.1;
	else if (button == 5)
		data->zoom /= 1.1;
	else
		return (0);
	data->offset_x = mouse_real - (x - WIDTH / 2.0)
		/ (0.5 * data->zoom * WIDTH);
	data->offset_y = mouse_imag - (y - HEIGHT / 2.0)
		/ (0.5 * data->zoom * HEIGHT);
	render(data);
	return (0);
}
