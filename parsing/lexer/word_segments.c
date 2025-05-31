/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_segments.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:56:38 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/15 06:14:46 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	prepare_segment_for_insertion(t_segment *segment, \
										t_segment **segment_list)
{
	t_word_fragments	*fragment;

	fragment = segment->content;
	while (fragment != NULL)
	{
		if (fragment->quote_type == QUOTE_SINGLE
			|| fragment->quote_type == QUOTE_DOUBLE)
			segment->are_there_quotes_inside = 1;
		fragment = fragment->next;
	}
	segment->type = WORD;
	segment_add_back(segment_list, segment);
}

int	add_word_segment(char *str, t_segment **segment_list, char *origin, \
						t_data *data)
{
	t_segment	*segment;
	int			are_quotes_closed;
	int			i;

	segment = create_segment();
	if (segment == NULL)
		lexer_error_exit(origin, segment_list, data);
	i = 0;
	are_quotes_closed = 0;
	segment->content = split_word_into_fragments(str, &i, &are_quotes_closed);
	if (segment->content == NULL && are_quotes_closed == -1)
	{
		segment->type = ERROR_QUOTE;
		segment_add_back(segment_list, segment);
		return (i);
	}
	else if (segment->content == NULL)
	{
		free(segment);
		lexer_error_exit(origin, segment_list, data);
	}
	prepare_segment_for_insertion(segment, segment_list);
	return (i);
}
