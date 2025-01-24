#include "fractol.h"

void	render(t_fractol *data)
{
	if (data->img)
		mlx_clear_window(data->mlx, data->win);
	if (data->type == MANDELBROT)
		mandel_frac(data);
	else if (data->type == JULIA)
		julia_frac(data);
	else
		error_exit("Invalid fractal type");
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int psychedelic_color(int iter, int max_iter, int color_shift)
{
    if (iter == max_iter)
	    return 0x000000;
    double t = (double)iter / max_iter;
    int red = (int)(sin(5 * t + color_shift) * 127 + 128);
    int green = (int)(sin(5 * t + color_shift + 2) * 127 + 128);
    int blue = (int)(sin(5 * t + color_shift + 4) * 127 + 128);

    return (red << 16) | (green << 8) | blue;
}
