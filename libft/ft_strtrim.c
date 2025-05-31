/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:19:34 by mwallis           #+#    #+#             */
/*   Updated: 2024/10/24 18:02:29 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		total;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = ft_strlen(s1) - 1;
	while (s1[i] != '\0' && ft_strchr(set, s1[i]) != NULL)
		i++;
	while (j >= i && ft_strchr(set, s1[j]) != NULL)
		j--;
	total = j - i + 1;
	str = (char *)malloc(sizeof(char) * (total + 1));
	if (str == NULL)
	{
		return (NULL);
	}
	ft_strlcpy(str, &s1[i], total + 1);
	return (str);
}
