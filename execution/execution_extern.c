/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_extern.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:54:48 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/09 16:37:57 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execve_dfl(char *path, char **arg, char **env);

void	child_exec_extern(t_cmd *cmd, t_data *data)
{
	int		red;

	red = 0;
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (cmd->n_redir != 0)
			red = set_redirection(cmd, data, -1);
		if (red >= 0)
			exec_extern(cmd, -1, data);
		else
			exit_minishell(NULL, NULL, 1, data);
	}
}

/****************************************************************
Initialise les signaux, execute un programmme exterieur dans un process enfant
et remet les signaux par defaut.
******************************************************************/

void	init_exec_extern(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		exit_minishell(NULL, "malloc fail", 1, data);
	if (pid == 0)
		child_exec_extern(cmd, data);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &status, 0);
		handle_child_exit(status, &data->last_exit_status);
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
}
/****************************************************************
Execute un programme exterieur et verifie qu'il ce trouve dans le chemin
Relatif, Absolue ou dans le PATH de l'environement
Exit(126) si le programme n'existe pas ou la permission denied
******************************************************************/

void	exec_extern(t_cmd *cmd, int i, t_data *data)
{
	char	*exec_path;
	char	**env_paths;

	env_paths = NULL;
	if (ft_strchr(cmd->arg[0], '/'))
	{
		if (access(cmd->arg[0], X_OK) == 0)
			execve_dfl(cmd->arg[0], cmd->arg, data->env);
		permission_denied(cmd->arg[0], NULL, data);
	}
	else
	{
		env_paths = secure_split(get_env("PATH", data->env), ':', data);
		while (env_paths && env_paths[++i])
		{
			exec_path = add_chr('/', env_paths[i]);
			exec_path = ft_str_realoc(exec_path, cmd->arg[0]);
			if (access(exec_path, X_OK) == 0)
				execve_dfl(exec_path, cmd->arg, data->env);
			free(exec_path);
		}
	}
	if (errno == EACCES)
		permission_denied(cmd->arg[0], env_paths, data);
	exit_exec_extern(cmd->arg[0], ft_strchr(cmd->arg[0], '/'), env_paths, data);
}

static void	execve_dfl(char *path, char **arg, char **env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, arg, env);
}

void	handle_child_exit(int status, int *last_exit_status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT && isatty(STDERR_FILENO))
			write(STDERR_FILENO, "Quit\n", 5);
		*last_exit_status = 128 + sig;
	}
	else if (WIFEXITED(status))
	{
		*last_exit_status = WEXITSTATUS(status);
	}
	else
		*last_exit_status = 1;
}
