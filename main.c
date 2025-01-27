/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 22:45:06 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/27 01:12:14 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	check_args(int ac, char **av, t_fractol *data)
{
	if (ac < 2 || av[1] == NULL )
		usage();
	if (ft_strncmp(av[1], "mandelbrot", 10) == 0)
		data->type = MANDELBROT;
	else if (ft_strncmp(av[1], "julia", 5) == 0)
	{
		if (ac != 4)
			usage();
		data->julia_c.real = ft_atof(av[2]);
		data->julia_c.imaginaire = ft_atof(av[3]);
		data->type = JULIA;
	}
	else
		usage();
}

void	error_exit(char *msg)
{
	ft_putstr_fd(msg, 1);
	exit(EXIT_FAILURE);
}

void	usage(void)
{
	ft_putstr_fd("Usage:\n", 1);
	ft_putstr_fd("fractol mandelbrot\n", 1);
	ft_putstr_fd("./fractol Julia <real_part> <imaginary_part>\n", 1);
	exit(EXIT_FAILURE);
}

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
	mlx_hook(data.win, 17, 0, clear_fractol, &data);
	mlx_hook(data.win, 2, 1L << 0, handle_key, &data);
	mlx_mouse_hook(data.win, handle_mouse, &data);
	mlx_loop(data.mlx);
	return (0);
}
