/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:34:57 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/09 18:34:09 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (tab[i])
		free(tab[i++]);
	free(tab);
	tab = NULL;
}

void	free_cmds(t_cmd **cmds)
{
	int	i;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i])
	{
		free_cmd(cmds[i]);
		i++;
	}
	free(cmds);
}

void	free_cmd(t_cmd *cmd)
{
	free_tab(cmd->arg);
	cmd->arg = NULL;
	free_tab(cmd->filename_list);
	cmd->filename_list = NULL;
	free(cmd->type_list);
	cmd->type_list = NULL;
	free(cmd);
	cmd = NULL;
}

void	free_tmp(t_data *data)
{
	free_cmds(data->cmds);
	data->cmds = NULL;
	free_segment_list(&data->seg_list);
	data->seg_list = NULL;
	free(data->pipes);
	data->pipes = NULL;
	free(data->pids);
	data->pids = NULL;
	heredoc_free(&data->heredoc_files);
}

void	free_all(t_data *data, int child)
{
	free_cmds(data->cmds);
	data->cmds = NULL;
	free_segment_list(&data->seg_list);
	data->seg_list = NULL;
	free(data->pipes);
	data->pipes = NULL;
	free(data->pids);
	data->pids = NULL;
	free_tab(data->env);
	if (!child)
		heredoc_unlink(&data->heredoc_files);
	heredoc_free(&data->heredoc_files);
}
