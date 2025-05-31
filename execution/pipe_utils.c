/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 15:01:06 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/09 16:32:36 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_pipe(t_segment *seg_list)
{
	t_segment	*cur;
	int			n;

	cur = seg_list;
	n = 0;
	while (cur && cur->type != END_OF_FILE)
	{
		if (cur->type == PIPE)
			n++;
		cur = cur->next;
	}
	return (n);
}

void	close_unused_fd(t_data *data)
{
	int	i;

	if (!data->pipes)
		return ;
	i = 0;
	while (i < data->n_pipe)
	{
		if (data->pipes[i][0] > 2)
			close(data->pipes[i][0]);
		if (data->pipes[i][1] > 2)
			close(data->pipes[i][1]);
		i++;
	}
}

void	make_pipes(t_data *data)
{
	int	i;

	data->pipes = malloc(data->n_pipe * sizeof (int [2]));
	if (!data->pipes)
		exit_minishell(NULL, "malloc fail", 1, data);
	i = 0;
	while (i < data->n_pipe)
	{
		if (pipe(data->pipes[i]) == -1)
			exit_minishell(NULL, "pipe fail", 1, data);
		i++;
	}
}

void	sig_int_quit_ign(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_int_quit_reset(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
