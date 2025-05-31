/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:05:25 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/08 17:05:49 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	safe_dup2(t_data *data, int fd1, int fd2);

int	set_redirection(t_cmd *cmd, t_data *data, int i)
{
	int	fd;

	while (++i < cmd->n_redir && cmd->n_redir)
	{
		fd = open_target(cmd->type_list[i], cmd->filename_list[i]);
		if (fd < 0)
		{
			error_perror(cmd->filename_list[i]);
			data->last_exit_status = 1;
			return (-1);
		}
		apply_redir(data, cmd->type_list[i], fd);
	}
	return (0);
}

int	open_target(int type, const char *name)
{
	if (type == INPUT_REDIRECTION || type == HEREDOC)
		return (open(name, O_RDONLY));
	if (type == OUTPUT_REDIRECTION)
		return (open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (type == APPEND)
		return (open(name, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

void	apply_redir(t_data *d, int type, int fd)
{
	if (type == INPUT_REDIRECTION || type == HEREDOC)
		safe_dup2(d, fd, STDIN_FILENO);
	else
		safe_dup2(d, fd, STDOUT_FILENO);
	close(fd);
}

void	set_redirection_parent(t_cmd *cmd, t_data *data)
{
	int	bk_stdin;
	int	bk_stdout;

	bk_stdin = dup(STDIN_FILENO);
	bk_stdout = dup(STDOUT_FILENO);
	if (bk_stdin == -1 || bk_stdout == -1)
		exit_minishell(NULL, "dup fail", 1, data);
	if (set_redirection(cmd, data, -1) >= 0)
	{
		if (cmd->arg && ft_strcmp(cmd->arg[0], "exit"))
			data->last_exit_status = built_in_exec(cmd, data);
	}
	if (dup2(bk_stdin, STDIN_FILENO) == -1
		|| dup2(bk_stdout, STDOUT_FILENO) == -1)
		exit_minishell(NULL, "dup2 fail", 1, data);
	close(bk_stdin);
	close(bk_stdout);
	if (cmd->arg && !ft_strcmp(cmd->arg[0], "exit"))
		built_in_exec(cmd, data);
	free_tmp(data);
}

static void	safe_dup2(t_data *data, int fd1, int fd2)
{
	if (dup2(fd1, fd2) < 0)
		exit_minishell(NULL, "dup2 fail", 1, data);
	if (fd1 != fd2)
		close(fd1);
}
