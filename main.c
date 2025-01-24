#include "fractol.h"

void	check_args(int ac, char **av , t_fractol *data)
{
	char	*endptr;
	if (ac < 2 || av[1] == NULL )
		usage();
	if (strcmp(av[1], "mandelbrot") == 0)
		data->type = MANDELBROT;
	else if (strcmp(av[1], "julia") == 0)
	{
		if (ac != 4)
			usage();
		data->julia_c.real = strtod(av[2], &endptr);
		if (*endptr != '\0')
			usage();
		data->julia_c.imaginaire = strtod(av[3], &endptr);
		if (*endptr != '\0')
			usage();
		data->type = JULIA;
	}
	else
		usage();
}
    
void	error_exit(char *msg)
{
	printf("Error : %s \n", msg);
	exit(EXIT_FAILURE);
}

void	usage(void)
{
	printf("Usage:\n");
	printf("fractol mandelbrot\n");
	printf("./fractol Julia <real_part> <imaginary_part>\n");
	exit(EXIT_FAILURE);
}

int	main(int ac, char **av)
{
	t_fractol data;

	init_data(&data);
	check_args(ac, av, &data);
	data.mlx = mlx_init();
	if (!data.mlx)
		error_exit("Cannot connect to mlx server");
	data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "fract-ol");
	if (!data.win)
		error_exit("Cannot create a window");
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img)
		error_exit("Failed to create image");
	data.img_addr = mlx_get_data_addr(data.img, &data.bpp, &data.line_length, &data.endian);
	if (!data.img_addr)
		error_exit("Failed to get image data address");
	if (data.type != 0)
		render(&data);
	mlx_hook(data.win, 2, 1L << 0, handle_key, &data);
	mlx_mouse_hook(data.win, handle_mouse, &data);
	mlx_loop(data.mlx);
	return 0;
}
