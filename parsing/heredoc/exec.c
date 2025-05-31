/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:33:03 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 11:04:06 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	extract_delimiter_and_check_heredoc_expansion(t_segment \
						*heredoc_segment, char **delimiter, int *allow_expand)
{
	t_segment	*delimiter_segments;

	delimiter_segments = heredoc_segment->next;
	*delimiter = heredoc_join_fragments(delimiter_segments->content);
	if (*delimiter == NULL)
		return (2);
	if (delimiter_segments->are_there_quotes_inside == 1)
		*allow_expand = 0;
	else
		*allow_expand = 1;
	return (0);
}

static int	create_file_and_store_path_fragment(t_segment *heredoc_segment,
	t_data *data, int *fd, char **heredoc_path)
{
	t_segment	*delimiter_segments;

	delimiter_segments = heredoc_segment->next;
	if (heredoc_create_file(fd, heredoc_path, data) == 2)
		return (2);
	free_word_fragments(delimiter_segments->content);
	delimiter_segments->content = NULL;
	delimiter_segments->content = create_fragment(*heredoc_path, QUOTE_NONE);
	if (delimiter_segments->content == NULL)
	{
		close(*fd);
		return (2);
	}
	return (0);
}

static void	cleanup_after_heredoc(t_heredoc_norm_helper *h)
{
	enable_echoctl();
	sigaction(SIGINT, &h->previous_sigaction, NULL);
	get_next_line(STDIN_FILENO, 1);
	close(h->fd);
	free(h->delimiter);
}

static int	heredoc_execute(t_segment *heredoc_segment, t_data *data)
{
	t_heredoc_norm_helper	h;
	int						status_malloc;

	if (extract_delimiter_and_check_heredoc_expansion(heredoc_segment, \
									&h.delimiter, &h.allow_expand) == 2)
		return (2);
	if (create_file_and_store_path_fragment(heredoc_segment, data, \
											&h.fd, &h.heredoc_path) == 2)
		return (free(h.delimiter), 2);
	disable_echoctl();
	set_heredoc_sigint_handler(&h.previous_sigaction);
	status_malloc = read_and_write_heredoc_lines(data, &h);
	if (status_malloc == 1)
	{
		enable_echoctl();
		sigaction(SIGINT, &h.previous_sigaction, NULL);
		return (1);
	}
	else if (status_malloc == 2)
		return (2);
	cleanup_after_heredoc(&h);
	return (0);
}

int	handle_heredoc(t_segment *segment_list, t_data *data)
{
	int			status_malloc;
	t_segment	*segment_head;

	segment_head = segment_list;
	while (segment_list != NULL && segment_list->type != END_OF_FILE)
	{
		if (segment_list->type == HEREDOC)
		{
			status_malloc = heredoc_execute(segment_list, data);
			if (status_malloc == 1)
				return (1);
			if (status_malloc == 2)
			{
				write(2, ERROR_HEREDOC_MALLOC, ft_strlen(ERROR_HEREDOC_MALLOC));
				heredoc_clean_file(&data->heredoc_files);
				free_segment_list(&segment_head);
				rl_clear_history();
				free_tab(data->env);
				exit(1);
			}
		}
		segment_list = segment_list->next;
	}
	return (0);
}
