/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 23:29:29 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/14 23:30:33 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_str_only_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

static char	*validate_shlvl(char *str)
{
	int	shlvl;

	if (is_str_only_numeric(str) == 0)
	{
		write(2, ERROR_SHLVL_INVALID, 64);
		return (ft_strdup("SHLVL=1"));
	}
	shlvl = ft_atoi(str);
	if (shlvl >= 999 || shlvl < 0)
	{
		write(2, ERROR_SHLVL_TOO_HIGH, 76);
		return (ft_strdup("SHLVL=1"));
	}
	return (NULL);
}

char	*increase_shlvl(char *old_shell_lvl)
{
	int		shlvl_int;
	char	*shlvl_str;
	char	*new_shlvl;
	char	*validation;

	validation = validate_shlvl(old_shell_lvl + 6);
	if (validation != NULL)
		return (validation);
	shlvl_int = ft_atoi(old_shell_lvl + 6) + 1;
	shlvl_str = ft_itoa(shlvl_int);
	if (shlvl_str == NULL)
		return (NULL);
	new_shlvl = ft_strjoin("SHLVL=", shlvl_str);
	free(shlvl_str);
	if (new_shlvl == NULL)
		return (NULL);
	return (new_shlvl);
}
