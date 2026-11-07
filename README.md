# fract-ol

`fract-ol` draws the Mandelbrot set or a Julia set in a MiniLibX window. It
calculates the escape iteration for each pixel, maps that count to a color,
and writes the result into an image buffer.

## Build

The project uses the bundled `libft` and Linux MiniLibX. Its current Makefile
expects MiniLibX at `/home/mohel-am/Documents/minilibx-linux`; change `MLX_DIR`
and the matching include path in the Makefile/header to the location on your
machine before building if that path does not exist.

```sh
make
```

## Run

```sh
./fractol mandelbrot
./fractol julia
./fractol julia -0.7 0.27015
```

The Julia set uses `-0.7 + 0.27015i` when no parameters are supplied. The
program also accepts a real and imaginary parameter after `julia`; these are
parsed by the project's `ft_atof` helper.

## Controls

- Mouse wheel: zoom in or out around the cursor position.
- `C`: cycle through the color shifts.
- `Esc` or close the window: exit and release the MiniLibX resources.

## Rendering details

The window and image are 800 by 800 pixels. `mandel_frac` and `julia_frac`
map each pixel to a point in the complex plane, scaled by the current zoom and
offset. Both iteration functions repeatedly apply the corresponding complex
quadratic formula until the magnitude exceeds 2 or the 100-iteration limit is
reached. Points still inside at the limit are drawn black; other points use
the sinusoidal RGB palette in `render.c`.

Mouse zoom calculates the complex coordinate beneath the cursor before
changing the zoom, then adjusts the offsets so that coordinate stays under
the cursor after the redraw.
