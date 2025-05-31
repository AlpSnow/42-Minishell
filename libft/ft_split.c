/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:27:10 by mwallis           #+#    #+#             */
/*   Updated: 2024/10/24 17:19:46 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_countword_split(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else
			i++;
	}
	return (count);
}

static char	*ft_strndup_split(const char *s, size_t n)
{
	char	*ptr;
	size_t	i;

	ptr = (char *)malloc(sizeof(char) * (n + 1));
	if (ptr == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < n)
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

static int	ft_getnextw_split(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i])
	{
		i++;
	}
	return (i);
}

static char	**ft_freetab_split(char **tab, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(tab[i++]);
	}
	free(tab);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	int		j;
	int		words;
	char	**tab;

	if (s == NULL)
		return (NULL);
	words = ft_countword_split(s, c);
	tab = (char **)malloc(sizeof(char *) * (ft_countword_split(s, c) + 1));
	if (tab == NULL)
		return (NULL);
	j = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (j < words)
		{
			tab[j++] = ft_strndup_split(s, ft_getnextw_split(s, c));
			if (tab[j - 1] == NULL)
				return (ft_freetab_split(tab, j));
		}
		while (*s != c && *s)
			s++;
	}
	return (tab[j] = NULL, tab);
}
