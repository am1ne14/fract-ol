/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 21:40:14 by mohel-am          #+#    #+#             */
/*   Updated: 2024/10/31 17:27:14 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	trim(char c, char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
		{
			return (1);
		}
		i++;
	}
	return (0);
}

static int	start(char *str, char *set)
{
	int	i;

	i = 0;
	while (str[i] != '\0' && trim(str[i], set))
		i++;
	return (i);
}

static int	end(char *str, char *set)
{
	int	i;

	i = ft_strlen(str) - 1;
	while (i >= 0 && trim(str[i], set))
		i--;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	int		i;
	int		start_index;
	int		end_index;

	i = 0;
	start_index = start((char *)s1, (char *)set);
	end_index = end((char *)s1, (char *)set);
	if (start_index > end_index)
	{
		result = malloc(1);
		if (result)
			result[0] = '\0';
		return (result);
	}
	result = malloc(sizeof(char) * (end_index - start_index + 2));
	if (!result)
		return (NULL);
	while (i < (end_index - start_index + 1))
	{
		result[i] = s1[start_index + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
