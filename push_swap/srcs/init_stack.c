/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stack.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 14:18:58 by mohel-am          #+#    #+#             */
/*   Updated: 2025/03/13 14:25:16 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/push_swap.h"
/*
 *	t_stack	*init_stack(int ac, char **av);
 *	void	process_argument(char *arg, t_stack **stack);
 *	t_stack	*create_new_node(int value);
 *	void	add_to_stack(t_stack **stack, t_stack *new);
 *	int	check_dup(t_stack *a);
 * */

t_stack	*init_stack(int ac, char **av)
{
	t_stack	*stack;
	int		i;

	stack = NULL;
	i = 1;
	while (i < ac)
	{
		process_argument(av[i], &stack);
		i++;
	}
	return (stack);
}

void	process_argument(char *arg, t_stack **stack)
{
	char	**splitted_av;
	t_stack	*new;
	int		j;

	splitted_av = ft_split(arg, ' ');
	j = 0;
	while (splitted_av[j])
	{
		new = create_new_node(ft_atoi(splitted_av[j]));
		if (!new)
		{
			free_stack(*stack);
			exit_error();
		}
		add_to_stack(stack, new);
		j++;
	}
	free_split(splitted_av);
}

t_stack	*create_new_node(int value)
{
	t_stack	*new;

	new = (t_stack *)malloc(sizeof(t_stack));
	if (!new)
		return (NULL);
	new->value = value;
	new->index = -1;
	new->pos = -1;
	new->target_pos = -1;
	new->cost_a = -1;
	new->cost_b = -1;
	new->next = NULL;
	return (new);
}

void	add_to_stack(t_stack **stack, t_stack *new)
{
	t_stack	*temp;

	if (!new)
		return ;
	if (!*stack)
	{
		*stack = new;
		return ;
	}
	temp = *stack;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

int	check_dup(t_stack *a)
{
	t_stack	*tmp;
	t_stack	*current;

	current = a;
	while (current)
	{
		tmp = current->next;
		while (tmp)
		{
			if (current->value == tmp->value)
			{
				free_stack(a);
				return (1);
			}
			tmp = tmp->next;
		}
		current = current->next;
	}
	return (0);
}
