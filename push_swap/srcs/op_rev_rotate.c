/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_rev_rotate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:39:13 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 16:52:47 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*
 *	void	rra(t_stack **a, int print);
 *	void	rrb(t_stack **b, int print);
 *	void	rrr(t_stack **a, t_stack **b, int print);
 * */

t_stack	*get_stack_before_bottom(t_stack *stack)
{
	while (stack && stack->next && stack->next->next)
		stack = stack->next;
	return (stack);
}

void	rra(t_stack **a, int print)
{
	t_stack	*last;
	t_stack	*before_last;

	if (!*a || !(*a)->next)
		return ;
	before_last = get_stack_before_bottom(*a);
	last = before_last->next;
	before_last->next = NULL;
	last->next = *a;
	*a = last;
	if (print)
		ft_putstr_fd("rra\n", 1);
}

void	rrb(t_stack **b, int print)
{
	t_stack	*last;
	t_stack	*before_last;

	if (!*b || !(*b)->next)
		return ;
	before_last = get_stack_before_bottom(*b);
	last = before_last->next;
	before_last->next = NULL;
	last->next = *b;
	*b = last;
	if (print)
		ft_putstr_fd("rrb\n", 1);
}

void	rrr(t_stack **a, t_stack **b, int print)
{
	rra(a, 0);
	rrb(b, 0);
	if (print)
		ft_putstr_fd("rrr\n", 1);
}
