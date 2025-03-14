/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 17:11:33 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 17:37:27 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "../libft/libft.h"

typedef struct s_stack
{
	int				value;
	int				index;
	int				pos;
	int				target_pos;
	int				cost_a;
	int				cost_b;
	struct s_stack	*next;
}	t_stack;

//big_sort.c
void	sort_big(t_stack **a, t_stack **b);
void	push_small_elements(t_stack **a, t_stack **b, int size);
void	sort_b_stack(t_stack **a, t_stack **b);
void	sort_b_stack(t_stack **a, t_stack **b);
void	final_rotate(t_stack **a);
//check_input.c
int		is_number(char *str);
int		check_duplicates(char **av);
int		check_splitted_numbers(char **splitted_av);
int		check_input(char **av);
//helpers.c
int		absolute_value(int nb);
void	exit_error(void);
void	free_stack(t_stack *stack);
int		get_lowest_index_position(t_stack **stack);
//init_stack.c
t_stack	*init_stack(int ac, char **av);
void	process_argument(char *arg, t_stack **stack);
t_stack	*create_new_node(int value);
void	add_to_stack(t_stack **stack, t_stack *new);
//op_rev_rotate.c
void	rra(t_stack **a, int print);
void	rrb(t_stack **b, int print);
void	rrr(t_stack **a, t_stack **b, int print);
//op_rotate.c
void	ra(t_stack **a, int print);
void	rb(t_stack **b, int print);
void	rr(t_stack **a, t_stack **b, int print);
//op_swap_push.c
void	sa(t_stack **a, int print);
void	sb(t_stack **b, int print);
void	ss(t_stack **a, t_stack **b, int print);
void	pa(t_stack **a, t_stack **b, int print);
void	pb(t_stack **a, t_stack **b, int print);
//sort_b_stack.c
void	get_target_position(t_stack **a, t_stack **b);
int		find_closest_upper_index(t_stack **a, int b_idx);
void	get_position(t_stack **stack);
int		get_target(t_stack **a, int b_idx);
int		find_closest_lower_index(t_stack **a, int target_idx);
//sort_b_stack_helpers.c
void	get_cost(t_stack **a, t_stack **b);
void	do_cheapest_move(t_stack **a, t_stack **b);
void	do_move(t_stack **a, t_stack **b, int cost_a, int cost_b);
void	finish_rotation(t_stack **a, t_stack **b, int cost_a, int cost_b);
//sort_three.c
void	sort_three(t_stack **a);
int		get_highest_index(t_stack *stack);
//stack_utils.c
int		is_sorted(t_stack *stack);
void	index_stack(t_stack **stack);
int		get_stack_size(t_stack *stack);
int		find_min_unindexed(t_stack *stack);
//push_swap.h
void	push_swap(t_stack **a, t_stack **b);

#endif
