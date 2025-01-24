#include "fractol.h"
/*
void julia_frac(t_fractol *data)
{
    int x;
    int color;
    int iter;
    int y;
    int pixel;
    t_comp z;
    
    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            z.real = (x - WIDTH / 2.0) * (4.0 / WIDTH) / data->zoom + data->offset_x;
            z.imaginaire = (y - HEIGHT / 2.0) * (4.0 / HEIGHT) / data->zoom + data->offset_y;
            iter = julia_iter(z, data->julia_c, data->max_iter);
            color = psychedelic_color(iter, data->max_iter);
            pixel = (y * data->line_length) + (x * (data->bpp / 8));
            if (pixel >= 0 && pixel < (HEIGHT * data->line_length))
                *(int *)(data->img_addr + pixel) = color;
            x++;
        }
        y++;
    }
}
*/

void julia_frac(t_fractol *data)
{
    int x, y, iter, color, pixel;
    t_comp z;

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            z.real = (x - WIDTH / 2.0) * 4.0 / WIDTH / data->zoom + data->offset_x;
            z.imaginaire = (y - HEIGHT / 2.0) * 4.0 / HEIGHT / data->zoom + data->offset_y;
            iter = julia_iter(z, data->julia_c, data->max_iter);
            color = psychedelic_color(iter, data->max_iter, data->color_shift);
            pixel = (y * data->line_length) + (x * data->bpp / 8);
            *(int *)(data->img_addr + pixel) = color;
            x++;
        }
        y++;
    }
}

int julia_iter(t_comp z, t_comp c, int max_iter)
{
    int iter;
    double tmp;

    iter = 0;
    while ((z.real * z.real + z.imaginaire * z.imaginaire <= 4) && iter < max_iter)
    {
        tmp = z.real * z.real - z.imaginaire * z.imaginaire + c.real;
        z.imaginaire = 2 * z.real * z.imaginaire + c.imaginaire;
        z.real = tmp;
        iter++;
    }
    return iter;
}
