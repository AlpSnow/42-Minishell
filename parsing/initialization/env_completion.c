/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_completion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 22:53:46 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/14 22:55:26 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**add_to_env(char **env, char *new_var)
{
	int		i;
	int		len;
	char	**new_env;

	if (new_var == NULL)
		return (free_tab(env), NULL);
	len = 0;
	while (env != NULL && env[len] != NULL)
		len++;
	new_env = malloc(sizeof(char *) * (len + 2));
	if (new_env == NULL)
		return (free_tab(env), free(new_var), NULL);
	i = 0;
	while (i < len)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}

static char	**add_missing_pwd(char **env)
{
	char	*pwd;
	char	*pwd_full_line;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		pwd = ft_strdup("/");
	if (pwd == NULL)
		return (free_tab(env), NULL);
	pwd_full_line = ft_strjoin("PWD=", pwd);
	free(pwd);
	if (pwd_full_line == NULL)
		return (free_tab(env), NULL);
	env = add_to_env(env, pwd_full_line);
	return (env);
}

static char	**add_session_env_vars(char **env)
{
	if (get_env("PWD", env) == NULL)
	{
		env = add_missing_pwd(env);
		if (env == NULL)
			return (NULL);
	}
	if (get_env("SHLVL", env) == NULL)
	{
		env = add_to_env(env, ft_strdup("SHLVL=1"));
		if (env == NULL)
			return (NULL);
	}
	return (env);
}

static char	**add_system_env_vars(char **env)
{
	if (get_env("PATH", env) == NULL)
	{
		env = add_to_env(env, ft_strdup(DEFAULT_PATH));
		if (env == NULL)
			return (NULL);
	}
	return (env);
}

char	**complete_env_if_needed(char **env)
{
	env = add_session_env_vars(env);
	if (env == NULL)
		return (NULL);
	env = add_system_env_vars(env);
	if (env == NULL)
		return (NULL);
	return (env);
}
