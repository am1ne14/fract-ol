/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_three.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:56:37 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/18 16:19:48 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*
 * void	sort_three(t_stack **a);
 * int	get_highest_index(t_stack *stack);
 *
 * */
int	get_highest_index(t_stack *stack)
{
	int		highest;

	highest = stack->index;
	while (stack)
	{
		if (stack->index > highest)
			highest = stack->index;
		stack = stack->next;
	}
	return (highest);
}

void	sort_three(t_stack **a)
{
	int		highest;

	if (is_sorted(*a))
		return ;
	highest = get_highest_index(*a);
	if ((*a)->index == highest)
		ra(a, 1);
	else if ((*a)->next->index == highest)
		rra(a, 1);
	if ((*a)->index > (*a)->next->index)
		sa(a, 1);
}
