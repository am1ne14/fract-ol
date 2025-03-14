/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   big_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 16:20:45 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 16:24:04 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*	
 *	void	sort_big(t_stack **a, t_stack **b);
 *	void    push_small_elements(t_stack **a, t_stack **b, int size);
 *	void	sort_b_stack(t_stack **a, t_stack **b);
 *	void	final_rotate(t_stack **a);
 */

void	sort_big(t_stack **a, t_stack **b)
{
	int	size;

	size = get_stack_size(*a);
	push_small_elements(a, b, size);
	sort_three(a);
	sort_b_stack(a, b);
	final_rotate(a);
}

void	push_small_elements(t_stack **a, t_stack **b, int size)
{
	int	pushed;

	pushed = 0;
	while (size > 6 && pushed < size - 3)
	{
		if ((*a)->index <= pushed)
		{
			pb(a, b, 1);
			rb(b, 1);
			pushed++;
		}
		else if ((*a)->index <= pushed + (size / 6))
		{
			pb(a, b, 1);
			pushed++;
		}
		else
			ra(a, 1);
	}
	while (size - pushed > 3)
	{
		pb(a, b, 1);
		pushed++;
	}
}

void	sort_b_stack(t_stack **a, t_stack **b)
{
	while (*b)
	{
		get_target_position(a, b);
		get_cost(a, b);
		do_cheapest_move(a, b);
	}
}

void	final_rotate(t_stack **a)
{
	int	i;

	get_position(a);
	i = get_lowest_index_position(a);
	if (i <= get_stack_size(*a) / 2)
	{
		while (i-- > 0)
			ra(a, 1);
	}
	else
	{
		i = get_stack_size(*a) - i;
		while (i-- > 0)
			rra(a, 1);
	}
}
