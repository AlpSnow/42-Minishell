/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:10:32 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/10 13:36:49 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void		child_process(int i, t_data *data, t_cmd *cmd);
static t_cmd	**get_cmd_array(t_segment *seg_list, t_data *data);
static void		parent_process(t_data *data);
static void		safe_dup2(int fd1, int fd2, t_data *data);

int	exec_piped(t_segment *seg_list, t_data *data)
{
	int		i;

	data->cmds = get_cmd_array(seg_list, data);
	if (data->n_pipe == 0)
		return (exec_command(data->cmds[0], data));
	make_pipes(data);
	data->pids = malloc((data->n_pipe + 1) * sizeof(pid_t));
	if (!data->pids)
		exit_minishell(NULL, "malloc fail", 1, data);
	i = 0;
	while (i < data->n_pipe + 1)
	{
		data->pids[i] = fork();
		if (data->pids[i] < 0)
		{
			close_unused_fd(data);
			exit_minishell(NULL, "fork fail", 1, data);
		}
		if (data->pids[i] == 0)
			child_process(i, data, data->cmds[i]);
		i++;
	}
	parent_process(data);
	return (data->last_exit_status);
}

static void	parent_process(t_data *data)
{
	int		status;
	int		i;
	int		got_sigint;

	sig_int_quit_ign();
	close_unused_fd(data);
	got_sigint = 0;
	i = -1;
	while (++i < data->n_pipe + 1)
	{
		waitpid(data->pids[i], &status, 0);
		if ((WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			|| (WIFEXITED(status) && WEXITSTATUS(status) == 130))
			got_sigint = 1;
		if (i == data->n_pipe)
		{
			if (WIFEXITED(status))
				data->last_exit_status = WEXITSTATUS(status);
			else
				data->last_exit_status = 128 + WTERMSIG(status);
		}
	}
	if (got_sigint && isatty(STDOUT_FILENO))
		write(STDOUT_FILENO, "\n", 1);
	sig_int_quit_reset();
}

static void	child_process(int i, t_data *data, t_cmd *cmd)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (i > 0)
		safe_dup2(data->pipes[i - 1][0], STDIN_FILENO, data);
	if (i < data->n_pipe)
		safe_dup2(data->pipes[i][1], STDOUT_FILENO, data);
	close_unused_fd(data);
	data->last_exit_status = exec_command(cmd, data);
	free_all(data, 1);
	exit(data->last_exit_status);
}

static t_cmd	**get_cmd_array(t_segment *seg_list, t_data *data)
{
	t_cmd		**cmd_array;
	t_segment	*cur;
	int			n_pipe;
	int			i;

	n_pipe = data->n_pipe;
	cmd_array = ft_calloc(n_pipe + 2, sizeof(t_cmd *));
	if (!cmd_array)
		exit_minishell(NULL, "malloc failed", 1, data);
	i = 0;
	cur = seg_list;
	while (i <= n_pipe)
	{
		cmd_array[i] = get_command(cur, data);
		while (cur->type != PIPE && cur->type != END_OF_FILE)
			cur = cur->next;
		if (cur && cur->type == PIPE)
			cur = cur->next;
		i++;
	}
	return (cmd_array);
}

static void	safe_dup2(int fd1, int fd2, t_data *data)
{
	if (dup2(fd1, fd2) < 0)
	{
		close_unused_fd(data);
		exit_minishell(NULL, "dup2 fail", 1, data);
	}
	if (fd1 != fd2)
		close(fd1);
}
