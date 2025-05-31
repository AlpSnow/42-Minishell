/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_extra.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:44:16 by lmarck            #+#    #+#             */
/*   Updated: 2025/04/23 23:06:30 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**dup_tab(char **tab)
{
	char	**ntab;
	int		i;

	i = 0;
	ntab = ft_calloc(count_line(tab) + 1, sizeof(char *));
	if (!ntab)
		return (NULL);
	while (tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	return (ntab);
}

int	ft_is_zero(int n)
{
	if (n)
		return (1);
	return (0);
}

char	**secure_split(char *line, char sep, t_data *data)
{
	char	**tab;

	if (!line)
		return (NULL);
	tab = ft_split(line, sep);
	if (!tab)
		exit_minishell(NULL, "malloc failed", MALLOC_FAIL, data);
	return (tab);
}

/****************************************************************
ft_putstr_fd qui renvoie -1 si write fail et 0 en cas de succes
******************************************************************/
int	ft_putstrs_fd(char *s, int fd)
{
	int	r;

	if (!s)
		return (-1);
	r = write(fd, s, ft_strlen(s));
	if (r >= 0)
		return (0);
	else
		return (-1);
}

/*****************************************************************
ft_strcmp qui compare depuis la fin de la chaine sans prendre en compte
la longueur renvoie 0 si pas de differece ex (hello world, world) return 0
******************************************************************/
int	rev_strlcmp(const char *str, const char *suffix)
{
	int	i;
	int	j;

	i = ft_strlen(str) - 1;
	j = ft_strlen(suffix) - 1;
	if (j > i)
		return (1);
	while (j >= 0)
	{
		if (str[i] != suffix[j])
			return (1);
		i--;
		j--;
	}
	return (0);
}
