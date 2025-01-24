#include "fractol.h"


void	init_data(t_fractol *data)
{
	data->mlx = NULL;
	data->win = NULL;
	data->img = NULL;
	data->img_addr = NULL;
	data->type = 0;
	data->julia_c.real = 0.0;
	data->julia_c.imaginaire = 0.0;
	data->zoom = 1.0;
	data->offset_x = 0.0;
	data->offset_y = 0.0;
	data->max_iter = 100;
	data->color_shift = 0;
	data->bpp = 0;
	data->line_length = 0;
	data->endian = 0;
}
