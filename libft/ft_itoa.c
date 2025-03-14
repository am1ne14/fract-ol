/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohel-am <mohel-am@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:46:42 by mohel-am          #+#    #+#             */
/*   Updated: 2024/10/29 18:34:23 by mohel-am         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	num_len(long nbr)
{
	int	len;

	len = 0;
	if (nbr < 0)
	{	
		len++;
		nbr = -nbr;
	}
	else if (nbr == 0)
		return (1);
	while (nbr != 0)
	{
		nbr = nbr / 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		nbr_len;
	long	nbr;

	nbr = n;
	nbr_len = num_len(nbr);
	res = malloc(sizeof(char) * (nbr_len + 1));
	if (!res)
		return (NULL);
	res[nbr_len] = '\0';
	if (nbr == 0)
		res[0] = '0';
	if (nbr < 0)
	{
		res[0] = '-';
		nbr = -nbr;
	}
	while (nbr != 0)
	{
		res[--nbr_len] = ((nbr % 10) + 48);
		nbr /= 10;
	}
	return (res);
}
