/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:41:01 by lmarck            #+#    #+#             */
/*   Updated: 2025/04/25 14:27:40 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_segment_type	*get_type_list(t_segment *sg, int n, t_data *data);
static char				**get_file_list(t_segment *segment, t_data *data);
static int				get_n_redir(t_segment *segment_list);
static char				**get_args(t_segment *seg, t_data *data);

/****************************************************************
Lance l'execution apres avoir separe les arguments des input et output files
******************************************************************/
t_cmd	*get_command(t_segment *segment_list, t_data *data)
{
	t_cmd		*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		exit_minishell(NULL, "malloc fail", 1, data);
	cmd->arg = NULL;
	cmd->filename_list = NULL;
	cmd->type_list = NULL;
	cmd->n_redir = get_n_redir(segment_list);
	cmd->arg = get_args(segment_list, data);
	cmd->filename_list = get_file_list(segment_list, data);
	cmd->type_list = get_type_list(segment_list, cmd->n_redir, data);
	return (cmd);
}

static t_segment_type	*get_type_list(t_segment *sg, int n, t_data *data)
{
	t_segment_type	*t_list;
	t_segment		*cur;
	int				i;

	t_list = malloc(sizeof(t_segment_type) * n);
	if (!t_list)
		exit_minishell(NULL, "malloc fail", 1, data);
	i = 0;
	cur = sg;
	while (cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (is_redirection(cur))
			t_list[i++] = cur->type;
		cur = cur->next;
	}
	return (t_list);
}

static char	**get_file_list(t_segment *segment, t_data *data)
{
	char		**f_list;
	t_segment	*cur;

	f_list = NULL;
	cur = segment;
	while (cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (is_redirection(cur) && cur->next)
			f_list = add_line_cmd(f_list, cur->next->content->str, data);
		cur = cur->next;
	}
	return (f_list);
}

static int	get_n_redir(t_segment *segment_list)
{
	t_segment	*cur;
	int			n;

	n = 0;
	cur = segment_list;
	while (cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (is_redirection(cur))
			n++;
		cur = cur->next;
	}
	return (n);
}

/****************************************************************
extrait le tableau d'arguments qui sera envoye a l'execution
en ometant les redirection et leurs noms de fichiers
/!\previous du premiers segment doit pointer sur NULL
******************************************************************/
static char	**get_args(t_segment *seg, t_data *data)
{
	char		**arg;
	t_segment	*cur;

	arg = NULL;
	cur = seg;
	while (cur->type != PIPE && cur->type != END_OF_FILE)
	{
		if (cur->type == WORD && !is_redirection(cur->previous))
			arg = add_line_cmd(arg, cur->content->str, data);
		cur = cur->next;
	}
	return (arg);
}
