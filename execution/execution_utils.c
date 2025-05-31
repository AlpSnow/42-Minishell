/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:38:50 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/10 16:48:15 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/****************************************************************
Return 1 si un argument est le nom d;ub built-in
******************************************************************/
int	is_build_in(char *name)
{
	if (!ft_strcmp("exit", name))
		return (1);
	else if (!ft_strcmp("export", name))
		return (1);
	else if (!ft_strcmp("echo", name))
		return (1);
	else if (!ft_strcmp("unset", name))
		return (1);
	else if (!ft_strcmp("env", name))
		return (1);
	else if (!ft_strcmp("pwd", name))
		return (1);
	else if (!ft_strcmp("cd", name))
		return (1);
	else
		return (0);
}

char	**add_line_cmd(char **tab, char *str, t_data *data)
{
	char	**ntab;
	int		i;

	if (!str)
		return (tab);
	i = 0;
	ntab = ft_calloc(count_line(tab) + 2, sizeof(char *));
	if (!ntab)
		exit_minishell(NULL, "malloc fail", 1, data);
	while (tab && tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	ntab[i] = ft_strdup(str);
	if (!ntab[i])
	{
		free(ntab);
		exit_minishell(NULL, "malloc fail", 1, data);
	}
	return (free(tab), ntab);
}

int	is_redirection(t_segment *seg)
{
	if (!seg)
		return (0);
	if (seg->type == INPUT_REDIRECTION)
		return (1);
	else if (seg->type == OUTPUT_REDIRECTION)
		return (1);
	else if (seg->type == APPEND)
		return (1);
	else if (seg->type == HEREDOC)
		return (1);
	return (0);
}

static int	is_directory(const char *path)
{
	struct stat	sb;

	return (lstat(path, &sb) == 0 && S_ISDIR(sb.st_mode));
}

void	permission_denied(char *path, char **env_paths, t_data *data)
{
	int	saved_errno;

	saved_errno = errno;
	if (saved_errno == EACCES && is_directory(path))
	{
		free_tab(env_paths);
		exit_minishell(path, "is a directory", 126, data);
	}
	error_perror(path);
	free_tab(env_paths);
	if (saved_errno == ENOENT)
		exit_minishell(NULL, NULL, 127, data);
	else
		exit_minishell(NULL, NULL, 126, data);
}
