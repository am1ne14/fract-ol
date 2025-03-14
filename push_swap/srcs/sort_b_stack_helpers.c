/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_b_stack_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:34:09 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 16:36:25 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*
 *	void	get_cost(t_stack **a, t_stack **b)
 *	void	do_cheapest_move(t_stack **a, t_stack **b)
 *	void	do_move(t_stack **a, t_stack **b, int cost_a, int cost_b)
 *	void	finish_rotation(t_stack **a, t_stack **b, int cost_a, int cost_b)
 */

void	get_cost(t_stack **a, t_stack **b)
{
	t_stack	*tmp_a;
	t_stack	*tmp_b;
	int		size_a;
	int		size_b;

	tmp_a = *a;
	tmp_b = *b;
	size_a = get_stack_size(tmp_a);
	size_b = get_stack_size(tmp_b);
	while (tmp_b)
	{
		if (tmp_b->pos <= size_b / 2)
			tmp_b->cost_b = tmp_b->pos;
		else
			tmp_b->cost_b = -(size_b - tmp_b->pos);
		if (tmp_b->target_pos <= size_a / 2)
			tmp_b->cost_a = tmp_b->target_pos;
		else
			tmp_b->cost_a = -(size_a - tmp_b->target_pos);
		tmp_b = tmp_b->next;
	}
}

void	do_cheapest_move(t_stack **a, t_stack **b)
{
	t_stack	*tmp;
	int		cheapest;
	int		cost_a;
	int		cost_b;
	int		cost;

	tmp = *b;
	cheapest = INT_MAX;
	while (tmp)
	{
		cost = absolute_value(tmp->cost_a) + absolute_value(tmp->cost_b);
		if (cost < cheapest)
		{
			cheapest = cost;
			cost_a = tmp->cost_a;
			cost_b = tmp->cost_b;
		}
		tmp = tmp->next;
	}
	do_move(a, b, cost_a, cost_b);
}

void	do_move(t_stack **a, t_stack **b, int cost_a, int cost_b)

{
	while (cost_a > 0 && cost_b > 0)
	{
		rr(a, b, 1);
		cost_a--;
		cost_b--;
	}
	while (cost_a < 0 && cost_b < 0)
	{
		rrr(a, b, 1);
		cost_a++;
		cost_b++;
	}
	finish_rotation(a, b, cost_a, cost_b);
	pa(a, b, 1);
}

void	finish_rotation(t_stack **a, t_stack **b, int cost_a, int cost_b)
{
	while (cost_a > 0)
	{
		ra(a, 1);
		cost_a--;
	}
	while (cost_a < 0)
	{
		rra(a, 1);
		cost_a++;
	}
	while (cost_b > 0)
	{
		rb(b, 1);
		cost_b--;
	}
	while (cost_b < 0)
	{
		rrb(b, 1);
		cost_b++;
	}
}
