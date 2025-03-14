/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 17:05:09 by mohel-am          #+#    #+#             */
/*   Updated: 2024/11/01 21:56:06 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char *str, char sep)
{
	int	i;
	int	words;
	int	new;

	i = 0;
	words = 0;
	new = 1;
	while (str[i])
	{
		if (str[i] == sep)
			new = 1;
		else if (new)
		{
			words++;
			new = 0;
		}
		i++;
	}
	return (words);
}

static char	**free_split(char **result, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (NULL);
}

static char	*get_next_word(const char *str, char sep, int *i)
{
	int	start;
	int	end;

	while (str[*i] && str[*i] == sep)
		(*i)++;
	start = *i;
	while (str[*i] && str[*i] != sep)
		(*i)++;
	end = *i;
	return (ft_substr(str, start, end - start));
}

char	**ft_split(char const *str, char sep)
{
	int		i;
	int		j;
	char	**result;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	result = malloc(sizeof(char *) * (count_words((char *)str, sep) + 1));
	if (!result)
		return (NULL);
	while (str[i])
	{
		if (str[i] != sep)
		{
			result[j] = get_next_word(str, sep, &i);
			if (!result[j])
				return (free_split (result, j));
			j++;
		}
		else
			i++;
	}
	result[j] = NULL;
	return (result);
}
