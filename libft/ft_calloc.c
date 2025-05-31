/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:52:45 by mwallis           #+#    #+#             */
/*   Updated: 2024/10/30 17:19:40 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (size != 0 && (nmemb > SIZE_MAX / size))
	{
		return (NULL);
	}
	ptr = (void *)malloc(size * nmemb);
	if (ptr == NULL)
	{
		return (NULL);
	}
	ft_bzero(ptr, (size * nmemb));
	return (ptr);
}
