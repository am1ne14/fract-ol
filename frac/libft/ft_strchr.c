/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:02:31 by mohel-am          #+#    #+#             */
/*   Updated: 2024/10/30 12:02:23 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int to_find)
{
	unsigned int	i;
	char			x;

	i = 0;
	x = (char)to_find;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == x)
		{
			return ((char *) &str[i]);
		}
		i++;
	}
	if (str[i] == x)
	{
		return ((char *) &str[i]);
	}
	return (NULL);
}
