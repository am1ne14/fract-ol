/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:44:47 by mohel-am          #+#    #+#             */
/*   Updated: 2024/10/25 15:10:01 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	y;
	size_t			i;

	str = (unsigned char *)s;
	y = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (str[i] == y)
		{
			return ((void *) &str[i]);
		}
		i++;
	}
	return (NULL);
}
