/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:27:38 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 09:26:41 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	apply_expansion_if_needed(t_word_fragments *fragment, t_data *data, \
									t_segment **segment_head, int is_heredoc)
{
	int	status_code;

	if (fragment->quote_type == QUOTE_NONE
		|| fragment->quote_type == QUOTE_DOUBLE)
	{
		status_code = env_expansion_apply_to_fragment(fragment, \
									data, segment_head, is_heredoc);
		if (status_code == 1)
			return (1);
		else if (status_code == 2 || status_code == 99)
			return (2);
	}
	return (0);
}

static int	is_heredoc_delimiter(t_segment *segment_list)
{
	if (segment_list->type == WORD
		&& segment_list->previous != NULL
		&& segment_list->previous->type == HEREDOC)
	{
		return (1);
	}
	return (0);
}

static int	apply_expansion_to_segment(t_segment *segment,
	t_data *data, t_segment **segment_head, int is_heredoc)
{
	t_word_fragments	*current;
	int					status_code;

	current = segment->content;
	while (current != NULL)
	{
		status_code = apply_expansion_if_needed(current, data, \
										segment_head, is_heredoc);
		if (status_code == 1)
			return (1);
		else if (status_code == 2 || status_code == 99)
			return (2);
		current = current->next;
	}
	return (0);
}

int	env_expansion_apply(t_segment *segment_list, t_data *data, int is_heredoc)
{
	t_segment	*segment_head;
	int			status_code;

	segment_head = segment_list;
	while (segment_list != NULL)
	{
		if (is_heredoc_delimiter(segment_list) && is_heredoc == 0)
		{
			segment_list = segment_list->next;
			continue ;
		}
		if (segment_list->type == WORD)
		{
			status_code = apply_expansion_to_segment(segment_list, \
									data, &segment_head, is_heredoc);
			if (status_code != 0)
				return (status_code);
		}
		segment_list = segment_list->next;
	}
	return (0);
}
