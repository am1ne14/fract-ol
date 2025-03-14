NAME = push_swap
CC = cc
RM = rm -f
CFLAGS = -Wall -Werror -Wextra -I inc

SRCS = srcs/big_sort.c\
	srcs/check_input.c\
	srcs/helpers.c\
	srcs/init_stack.c\
	srcs/main.c\
	srcs/op_rev_rotate.c\
	srcs/op_rotate.c\
	srcs/op_swap_push.c\
	srcs/sort_b_stack.c\
	srcs/sort_b_stack_helpers.c\
	srcs/sort_three.c\
	srcs/stack_utils.c\
	srcs/push_swap.c


OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT_AR = libft/libft.a

INCLUDES = inc/push_swap.h

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_AR)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_AR) -o $(NAME)

$(LIBFT_AR):
	@make -s -C $(LIBFT_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make -s clean -C $(LIBFT_DIR)
	@$(RM) $(OBJS)

fclean: clean
	@make -s clean -C $(LIBFT_DIR)
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
