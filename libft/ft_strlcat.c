/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 16:29:09 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/31 02:25:04 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len_dest;
	size_t	len_source;
	size_t	i;

	len_dest = 0;
	len_source = 0;
	if (dst == NULL || src == NULL)
		return (0);
	while (src[len_source] != '\0')
		len_source++;
	while (len_dest < size && dst[len_dest] != '\0')
		len_dest++;
	if (len_dest == size)
		return (size + len_source);
	i = 0;
	while (i + len_dest + 1 < size && src[i] != '\0')
	{
		dst[len_dest + i] = src[i];
		i++;
	}
	dst[len_dest + i] = '\0';
	return (len_dest + len_source);
}

// size_t	ft_strlcat(char *dst, const char *src, size_t size)
// {
// 	size_t	lensource;
// 	size_t	lendest;
// 	size_t	i;

// 	if (dst == NULL || src == NULL)
// 		return (0);
// 	if ((!dst && size <= 0) || (!src && size <= 0))
// 		return (0);
// 	lensource = ft_strlen(src);
// 	lendest = ft_strlen(dst);
// 	if (size <= lendest)
// 	{
// 		return (size + lensource);
// 	}
// 	i = 0;
// 	while (src[i] != '\0' && (lendest + i) < (size - 1))
// 	{
// 		dst[lendest + i] = src[i];
// 		i++;
// 	}
// 	dst[lendest + i] = '\0';
// 	return (lendest + lensource);
// }
