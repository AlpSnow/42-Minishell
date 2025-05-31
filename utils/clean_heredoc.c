/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 17:08:21 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/09 18:33:02 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_free(t_heredoc_file **heredoc_file_lst)
{
	t_heredoc_file	*tmp;

	while (heredoc_file_lst != NULL && *heredoc_file_lst != NULL)
	{
		tmp = (*heredoc_file_lst)->next;
		free((*heredoc_file_lst)->path);
		free(*heredoc_file_lst);
		*heredoc_file_lst = tmp;
	}
}

void	heredoc_unlink(t_heredoc_file **lst)
{
	t_heredoc_file	*cur;

	if (!lst)
		return ;
	cur = *lst;
	while (cur)
	{
		unlink(cur->path);
		cur = cur->next;
	}
}
