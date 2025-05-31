/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:56:18 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/10 15:39:55 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	put_error_mini(char *cmd, char *error);

void	exit_minishell(char *cmd, char *error, int ex, t_data *dt)
{
	if (error)
	{
		put_error_mini(cmd, error);
		write(2, "\n", 1);
	}
	free_tab(dt->env);
	free_tmp(dt);
	heredoc_clean_file(&dt->heredoc_files);
	close_unused_fd(dt);
	exit(ex);
}

static void	put_error_mini(char *cmd, char *error)
{
	write (2, "minishell: ", 12);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(error, 2);
}

void	put_error(char *cmd, char *arg, char *error)
{
	write (2, "minishell: ", 12);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd("'", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': ", 2);
	}
	ft_putstr_fd(error, 2);
}

void	error_perror(char *arg)
{
	write(2, "minishell: ", 12);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(" :", 2);
	perror(NULL);
}

void	exit_exec_extern(char *cmd, char *f, char **path, t_data *dt)
{
	free_tab(path);
	if (errno == EISDIR)
		exit_minishell(cmd, "is a directory", 126, dt);
	else if (!f)
		exit_minishell(cmd, "command not found", 127, dt);
	else
		exit_minishell(cmd, "No such file or directory", 127, dt);
}
