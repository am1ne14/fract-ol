/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_b_stack.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:24:54 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 17:02:50 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*
 *	void	get_target_position(t_stack **a, t_stack **b)
 *	int	find_closest_upper_index(t_stack **a, int b_idx);
 *	void	get_position(t_stack **stack)
 *	int	get_target(t_stack **a, int b_idx)
 *	int	find_closest_lower_index(t_stack **a, int target_idx);
*/

void	get_target_position(t_stack **a, t_stack **b)
{
	t_stack	*tmp_b;
	int		target_pos;

	tmp_b = *b;
	while (tmp_b)
	{
		target_pos = get_target(a, tmp_b->index);
		tmp_b->target_pos = target_pos;
		tmp_b = tmp_b->next;
	}
}

void	get_position(t_stack **stack)
{
	t_stack	*tmp;
	int		i;

	tmp = *stack;
	i = 0;
	while (tmp)
	{
		tmp->pos = i;
		tmp = tmp->next;
		i++;
	}
}

int	get_target(t_stack **a, int b_idx)
{
	int	target_pos;

	target_pos = find_closest_upper_index(a, b_idx);
	if (target_pos != -1)
		return (target_pos);
	return (find_closest_lower_index(a, INT_MAX));
}

int	find_closest_lower_index(t_stack **a, int target_idx)
{
	t_stack	*tmp_a;
	int		target_pos;

	tmp_a = *a;
	target_pos = 0;
	while (tmp_a)
	{
		if (tmp_a->index < target_idx)
		{
			target_idx = tmp_a->index;
			target_pos = tmp_a->pos;
		}
		tmp_a = tmp_a->next;
	}
	return (target_pos);
}

int	find_closest_upper_index(t_stack **a, int b_idx)
{
	t_stack	*tmp_a;
	int		target_idx;
	int		target_pos;

	tmp_a = *a;
	target_idx = INT_MAX;
	target_pos = 0;
	while (tmp_a)
	{
		if (tmp_a->index > b_idx && tmp_a->index < target_idx)
		{
			target_idx = tmp_a->index;
			target_pos = tmp_a->pos;
		}
		tmp_a = tmp_a->next;
	}
	if (target_idx == INT_MAX)
		return (-1);
	return (target_pos);
}
