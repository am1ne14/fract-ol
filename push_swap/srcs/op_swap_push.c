/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_swap_push.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:50:06 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 16:53:05 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*
 * void	sa(t_stack **a, int print);
 * void	sb(t_stack **b, int print);
 * void	ss(t_stack **a, t_stack **b, int print);
 * void	pa(t_stack **a, t_stack **b, int print);
 * void	pb(t_stack **a, t_stack **b, int print);
 * */

void	sa(t_stack **a, int print)
{
	t_stack	*first;
	t_stack	*second;

	if (!*a || !(*a)->next)
		return ;
	first = *a;
	second = (*a)->next;
	first->next = second->next;
	second->next = first;
	*a = second;
	if (print)
		ft_putstr_fd("sa\n", 1);
}

void	sb(t_stack **b, int print)
{
	t_stack	*first;
	t_stack	*second;

	if (!*b || !(*b)->next)
		return ;
	first = *b;
	second = (*b)->next;
	first->next = second->next;
	second->next = first;
	*b = second;
	if (print)
		ft_putstr_fd("sb\n", 1);
}

void	ss(t_stack **a, t_stack **b, int print)
{
	sa(a, 0);
	sb(b, 0);
	if (print)
		ft_putstr_fd("ss\n", 1);
}

void	pa(t_stack **a, t_stack **b, int print)
{
	t_stack	*temp;

	if (!*b)
		return ;
	temp = *b;
	*b = (*b)->next;
	temp->next = *a;
	*a = temp;
	if (print)
		ft_putstr_fd("pa\n", 1);
}

void	pb(t_stack **a, t_stack **b, int print)
{
	t_stack	*temp;

	if (!*a)
		return ;
	temp = *a;
	*a = (*a)->next;
	temp->next = *b;
	*b = temp;
	if (print)
		ft_putstr_fd("pb\n", 1);
}
