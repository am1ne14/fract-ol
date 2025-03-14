/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:34:42 by mohel-am          #+#    #+#             */
/*   Updated: 2024/11/06 02:34:28 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *to, const void *from, size_t n)
{
	unsigned char	*tmp_to;
	unsigned char	*tmp_from;
	size_t			i;

	if (to == NULL && from == NULL)
		return (NULL);
	tmp_to = (unsigned char *)to;
	tmp_from = (unsigned char *)from;
	i = 0;
	while (i < n)
	{
		tmp_to[i] = tmp_from[i];
		i++;
	}
	return (to);
}
