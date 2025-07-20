NAME		= fractol
CC		= cc
CFLAGS		= -Wall -Wextra -Werror
MLX_DIR		?= minilibx-linux
INCLUDES	= -I. -Ilibft -I$(MLX_DIR)
SRC_DIR		= .
OBJ_DIR		= obj
LIBFT_DIR	= libft
SRCS		= main.c helpers.c helpers2.c init.c julia.c key_hooks.c mandel.c render.c
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
LIBFT		= $(LIBFT_DIR)/libft.a
MLX			= $(MLX_DIR)/libmlx.a
LIBS		= -L$(LIBFT_DIR) -lft -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lbsd
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
# MiniLibX uses one unprototyped callback API for handlers with different arities.
$(OBJ_DIR)/main.o: CFLAGS += -Wno-cast-function-type
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)
$(MLX):
	$(MAKE) -C $(MLX_DIR)
clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	@if [ -f $(MLX_DIR)/Makefile.gen ]; then $(MAKE) -C $(MLX_DIR) clean; fi
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
re: fclean all
.PHONY: all clean fclean re
