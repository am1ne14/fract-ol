/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 22:45:06 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/27 02:13:49 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	main(int ac, char **av)
{
	t_fractol	data;

	init_data(&data);
	check_args(ac, av, &data);
	data.mlx = mlx_init();
	if (!data.mlx)
		error_exit("Cannot connect to mlx server\n");
	data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "fract-ol");
	if (!data.win)
		error_exit("Cannot create a window\n");
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img)
		error_exit("Failed to create image\n");
	data.img_addr = mlx_get_data_addr(data.img, &data.bpp,
			&data.line_length, &data.endian);
	if (!data.img_addr)
		error_exit("Failed to get image data address\n");
	if (data.type != 0)
		render(&data);
	mlx_hook(data.win, 17, 0, (int (*)())clear_fractol, &data);
	mlx_key_hook(data.win, (int (*)())handle_key, &data);
	mlx_mouse_hook(data.win, (int (*)())handle_mouse, &data);
	mlx_loop(data.mlx);
	return (0);
}
