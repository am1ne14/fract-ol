/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:13:08 by mohel-am          #+#    #+#             */
/*   Updated: 2024/11/01 21:32:26 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int to_find)
{
	int		i;
	char	x;

	i = ft_strlen(str);
	x = (char )to_find;
	while (i > 0)
	{
		if (str[i] == x)
		{
			return ((char *) str + i);
		}
		i--;
	}
	if (str[i] == x)
		return ((char *)str);
	return (NULL);
}
