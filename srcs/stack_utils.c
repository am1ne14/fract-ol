/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:25:49 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 14:33:10 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*int	is_sorted(t_stack *stack);
 * void	index_stack(t_stack **stack);
 * int	get_stack_size(t_stack *stack);
 * int	find_min_unindexed(t_stack *stack);
 * */

int	is_sorted(t_stack *stack)
{
	while (stack && stack->next)
	{
		if (stack->value > stack->next->value)
			return (0);
		stack = stack->next;
	}
	return (1);
}

int	get_stack_size(t_stack *stack)
{
	int	size;

	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}

void	index_stack(t_stack **stack)
{
	t_stack	*current;
	int		min;
	int		i;
	int		size;

	size = get_stack_size(*stack);
	i = 0;
	while (i < size)
	{
		min = find_min_unindexed(*stack);
		current = *stack;
		while (current)
		{
			if (current->value == min && current->index == -1)
			{
				current->index = i;
				break ;
			}
			current = current->next;
		}
		i++;
	}
}

int	find_min_unindexed(t_stack *stack)
{
	t_stack	*tmp;
	int		min;

	tmp = stack;
	min = INT_MAX;
	while (tmp)
	{
		if (tmp->value < min && tmp->index == -1)
			min = tmp->value;
		tmp = tmp->next;
	}
	return (min);
}
