/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:12:48 by mohel-am          #+#    #+#             */
/*   Updated: 2024/10/25 15:11:41 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*tmp_s;

	tmp_s = (unsigned char *)s;
	while (n > 0)
	{
		*tmp_s = (unsigned char)c;
		tmp_s++;
		n--;
	}
	return (s);
}
