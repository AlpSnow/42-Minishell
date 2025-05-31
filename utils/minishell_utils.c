/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:59:45 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/08 17:09:23 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env(char *name, char **env)
{
	int		i;
	int		len;
	char	*tmp;
	char	*ptr;

	if (!name)
		return (NULL);
	i = 0;
	tmp = ft_strjoin(name, "=");
	len = ft_strlen(tmp);
	while (env[i])
	{
		ptr = ft_strnstr(env[i], tmp, len);
		if (ptr)
			return (free(tmp), ptr + len);
		i++;
	}
	free(tmp);
	return (NULL);
}

int	count_line(char **tab)
{
	int	i;

	if (!tab)
		return (0);
	i = 0;
	while (tab[i])
		i++;
	return (i);
}

char	*add_chr(unsigned char buf, char *str)
{
	char	*nstr;
	int		i;

	i = 0;
	nstr = ft_calloc(ft_strlen(str) + 2, sizeof(char));
	if (!nstr)
		return (NULL);
	while (str[i])
	{
		nstr[i] = str[i];
		i++;
	}
	nstr[i] = buf;
	nstr[i + 1] = 0;
	return (nstr);
}

char	*ft_str_realoc(char *s1, char const *s2)
{
	char	*ptr;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	ptr = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (ptr == NULL)
		return (NULL);
	i = 0;
	while (s1[i] != '\0')
	{
		ptr[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		ptr[i + j] = s2[j];
		j++;
	}
	ptr[i + j] = '\0';
	free(s1);
	return (ptr);
}

char	**sort_tab(char **tab)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (tab[i])
	{
		j = i + 1;
		while (tab[j])
		{
			if (ft_strcmp(tab[i], tab[j]) > 0)
			{
				tmp = tab[i];
				tab[i] = tab[j];
				tab[j] = tmp;
			}
			j++;
		}
		i++;
	}
	return (tab);
}
