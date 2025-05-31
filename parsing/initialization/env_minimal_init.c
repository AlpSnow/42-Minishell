/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_minimal_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 23:27:30 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/14 23:29:14 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**fill_minimal_env(char **env_minimal)
{
	env_minimal[1] = ft_strdup("SHLVL=1");
	if (env_minimal[1] == NULL)
		return (free(env_minimal[0]), free(env_minimal), NULL);
	env_minimal[2] = ft_strdup(DEFAULT_PATH);
	if (env_minimal[2] == NULL)
	{
		return (free(env_minimal[0]), free(env_minimal[1]),
			free(env_minimal), NULL);
	}
	env_minimal[3] = NULL;
	return (env_minimal);
}

char	**init_minimal_env(void)
{
	char	**env_minimal;
	char	*pwd;
	char	*tmp;

	env_minimal = malloc(sizeof(char *) * 4);
	if (env_minimal == NULL)
		return (NULL);
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		pwd = ft_strdup("/");
	if (pwd == NULL)
		return (free(env_minimal), NULL);
	tmp = ft_strjoin("PWD=", pwd);
	free(pwd);
	if (tmp == NULL)
		return (free(env_minimal), NULL);
	env_minimal[0] = tmp;
	if (fill_minimal_env(env_minimal) == NULL)
		return (NULL);
	return (env_minimal);
}
