/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:36:44 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/07 17:56:53 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/****************************************************************
execute la command de cmd_arg[0] et renvoie le retour de la commande execute
******************************************************************/
int	exec_command(t_cmd *cmd, t_data *data)
{
	if (!cmd->arg)
	{
		set_redirection_parent(cmd, data);
		return (0);
	}
	if (is_build_in(cmd->arg[0]))
	{
		set_redirection_parent(cmd, data);
		return (data->last_exit_status);
	}
	else if (rev_strlcmp(cmd->arg[0], "/minishell") == 0)
		return (exec_minishell(cmd, data));
	else
	{
		init_exec_extern(cmd, data);
		return (data->last_exit_status);
	}
}

/****************************************************************
execute les fonctions build in correspondant a arg[0] et return leurs return
Si arg[0] ne correspont a aucun buil-in renvoie 0
******************************************************************/
int	built_in_exec(t_cmd *cmd, t_data *data)
{
	if (!ft_strcmp("exit", cmd->arg[0]))
		bi_exit(cmd->arg, data);
	if (!ft_strcmp("export", cmd->arg[0]))
		return (bi_export(cmd->arg, data));
	if (!ft_strcmp("echo", cmd->arg[0]))
		return (bi_echo(cmd->arg, 1, data));
	if (!ft_strcmp("unset", cmd->arg[0]))
		return (bi_unset(cmd->arg, data));
	if (!ft_strcmp("env", cmd->arg[0]))
		return (bi_env(cmd->arg, data));
	if (!ft_strcmp("pwd", cmd->arg[0]))
		return (bi_pwd(cmd->arg, data));
	if (!ft_strcmp("cd", cmd->arg[0]))
		return (bi_cd(cmd->arg, data));
	return (0);
}

/****************************************************************
Execute minishell en initialisant les signaux correctement
en en les remettant par defaut
******************************************************************/
int	exec_minishell(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		exit_minishell(NULL, "fork fail", FORK_FAIL, data);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		set_redirection(cmd, data, -1);
		execve(cmd->arg[0], cmd->arg, data->env);
		perror("execve");
		exit(127);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		data->last_exit_status = WEXITSTATUS(status);
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	return (data->last_exit_status);
}
