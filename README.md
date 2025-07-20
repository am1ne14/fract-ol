# fract-ol

A small C program for rendering the Mandelbrot and Julia fractals with the
MiniLibX X11 library.

## Build requirements

On Debian or Ubuntu, install the compiler, Make, X11 development files, and
MiniLibX's BSD utility dependency:

```sh
sudo apt install build-essential xorg libxext-dev libbsd-dev
```

On Fedora, install the corresponding packages with:

```sh
sudo dnf install gcc make libX11-devel libXext-devel libbsd-devel
```

MiniLibX is included in [`minilibx-linux/`](minilibx-linux/) with its upstream
license. Build the project from the repository root:

```sh
make
```

## Run

The repository includes a prebuilt Linux x86-64 executable named `fractol`.
It needs the X11 runtime libraries and an available display. `make` rebuilds it.

```sh
./fractol mandelbrot
./fractol Julia
./fractol Julia -0.7 0.27015
```

The Makefile uses the bundled library by default. To build against a separate
MiniLibX checkout, set `MLX_DIR`, for example:

```sh
make MLX_DIR=/path/to/minilibx-linux
```
