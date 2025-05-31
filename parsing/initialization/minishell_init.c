/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 23:30:47 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/14 23:34:08 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	**alloc_env_copy(int len)
{
	char	**env;

	env = malloc(sizeof(char *) * (len + 1));
	if (env == NULL)
		return (NULL);
	return (env);
}

static char	**fill_env_with_entries(char **envp, char **env)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "SHLVL=", 6) == 0)
			env[i] = increase_shlvl(envp[i]);
		else
			env[i] = ft_strdup(envp[i]);
		if (env[i] == NULL)
		{
			while (i > 0)
				free(env[--i]);
			free(env);
			return (NULL);
		}
		i++;
	}
	env[i] = NULL;
	return (env);
}

static char	**copy_env(char **envp)
{
	char	**env;
	int		i;

	i = 0;
	if (envp == NULL || envp[i] == NULL)
	{
		env = init_minimal_env();
		return (env);
	}
	while (envp[i] != NULL)
		i++;
	env = alloc_env_copy(i);
	if (env == NULL)
		return (NULL);
	env = fill_env_with_entries(envp, env);
	if (env == NULL)
		return (NULL);
	env = complete_env_if_needed(env);
	if (env == NULL)
		return (NULL);
	return (env);
}

int	minishell_init(char **envp, t_data *data)
{
	g_signal = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	data->last_exit_status = 0;
	data->heredoc_files = NULL;
	data->seg_list = NULL;
	data->cmds = NULL;
	data->pids = NULL;
	data->pipes = NULL;
	data->env = copy_env(envp);
	if (data->env == NULL)
		return (write (2, ERROR_ENV, 72), -1);
	return (0);
}
