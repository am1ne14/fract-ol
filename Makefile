# Project Name
NAME		= fractol

# Compiler and Flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I includes -I libft -I minilibx

# Directories
SRC_DIR		= .
OBJ_DIR		= obj
LIBFT_DIR	= libft
MLX_DIR		= minilibx

# Source Files
SRCS		= main.c helpers.c init.c julia_frac.c key_hooks.c mandel.c render.c
OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# Libraries
LIBFT		= $(LIBFT_DIR)/libft.a
MLX			= $(MLX_DIR)/libmlx.a
LIBS		= -L $(LIBFT_DIR) -lft -L $(MLX_DIR) -lmlx -lXext -lX11 -lm

# Rules
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX):
	$(MAKE) -C $(MLX_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
