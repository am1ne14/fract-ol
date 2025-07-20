/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 02:17:36 by mohel-am          #+#    #+#             */
/*   Updated: 2025/01/29 01:01:01 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "mlx.h"
# include "libft/libft.h"
# include <math.h>

# define WIDTH 800
# define HEIGHT 800
# define MAX_INTERATIONS 100

# define MANDELBROT 1
# define JULIA 2

typedef struct s_comp
{
	double	real;
	double	imaginaire;
}	t_comp;

typedef struct s_fractol
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*img_addr;
	int		bpp;
	int		line_length;
	int		endian;
	int		type;
	t_comp	julia_c;
	double	zoom;
	double	offset_x;
	double	offset_y;
	int		max_iter;
	int		color_shift;
}	t_fractol;

void	init_data(t_fractol *data);
void	check_args(int ac, char **av, t_fractol *data);
void	clean_exit(int exit_code, t_fractol *data);
void	error_exit(char *msg);
void	render(t_fractol *data);
void	parse_integer_part(const char **str, double *res, int *sign);
void	usage(void);
void	parse_fractional_part(const char **str, double *res, double *divider);
void	mandel_frac(t_fractol *data, int y);
void	julia_frac(t_fractol *data, int y);
int		clear_fractol(t_fractol *data);
int		iter_mandel(t_comp z, int max_iter);
int		julia_iter(t_comp z, t_comp c, int max_iter);
int		handle_mouse(int button, int x, int y, t_fractol *data);
int		handle_key(int keycode, t_fractol *data);
int		color_swap(int iter, int max_iter, int color_shift);
double	ft_atof(const char *str);

#endif
