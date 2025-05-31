/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 19:14:26 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/31 00:41:17 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*destination;
	const unsigned char	*source;

	if (n == 0)
		return (dest);
	if (dest == NULL || src == NULL)
	{
		return (NULL);
	}
	i = 0;
	destination = dest;
	source = src;
	while (i < n)
	{
		destination[i] = source[i];
		i++;
	}
	return (dest);
}
