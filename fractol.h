#ifndef FRACTOL_H
#define FRACTOL_H

#include "minilibx/mlx.h"
//#include "libft/libft.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define WIDTH 1000
#define HEIGHT 1000
#define MAX_INTERATIONS 50

#define MANDELBROT 1
#define JULIA 2

typedef struct s_comp
{
    double real;
    double imaginaire;
} t_comp;

typedef struct s_fractol
{
    void *mlx;
    void *win;
    void *img;
    char *img_addr;
    int bpp;
    int line_length;
    int endian;
    int type;
    t_comp julia_c;
    double zoom;
    double offset_x;
    double offset_y;
    int max_iter;
    int color_shift;
} t_fractol;

void init_data(t_fractol *data);
void check_args(int ac, char **av, t_fractol *data);
void error_exit(char *msg);
void usage(void);
void render(t_fractol *data);
void mandel_frac(t_fractol *data);
int iter_mandel(t_comp z, int max_iter);
void julia_frac(t_fractol *data);
int julia_iter(t_comp z, t_comp c, int max_iter);
int handle_mouse(int button, int x, int y, t_fractol *data);
int	handle_key(int keycode, t_fractol *data);
int psychedelic_color(int iter, int max_iter, int color_shift);
 #endif
