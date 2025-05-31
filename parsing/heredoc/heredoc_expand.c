/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:35:51 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 10:37:59 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*handle_heredoc_fragment_alloc_failure(char *str, \
													int *status_malloc)
{
	*status_malloc = 2;
	return (free(str), NULL);
}

static char	*handle_heredoc_segment_alloc_failure(t_word_fragments *fragment, \
													int *status_malloc)
{
	*status_malloc = 2;
	free_word_fragments(fragment);
	return (NULL);
}

static char	*handle_heredoc_expansion_failure(int status, \
		t_word_fragments *fragment, t_segment *segment, int *status_malloc)
{
	if (status == 1)
	{
		*status_malloc = 1;
		return (NULL);
	}
	if (status == 2)
	{
		free_word_fragments(fragment);
		free(segment);
		*status_malloc = 2;
		return (NULL);
	}
	return (NULL);
}

static char	*heredoc_expand_word(char *str, t_data *data, int *status_malloc)
{
	t_word_fragments	*fragment;
	t_segment			*segment;
	char				*expanded_line;
	int					status;

	fragment = create_fragment(str, QUOTE_NONE);
	if (fragment == NULL)
		return (handle_heredoc_fragment_alloc_failure(str, status_malloc));
	segment = create_segment();
	if (segment == NULL)
		return (handle_heredoc_segment_alloc_failure(fragment, status_malloc));
	segment->type = WORD;
	segment->content = fragment;
	status = env_expansion_apply(segment, data, 1);
	if (status != 0)
		return (handle_heredoc_expansion_failure(status, fragment, \
				segment, status_malloc));
	expanded_line = fragment->str;
	free(fragment);
	free(segment);
	return (expanded_line);
}

char	*heredoc_expand_if_needed(char *input_line, t_data *data, \
									int should_expand, int *status_malloc)
{
	char	*line_copy;
	char	*expanded_line;

	if (should_expand == 0)
	{
		return (input_line);
	}
	line_copy = ft_strdup(input_line);
	if (line_copy == NULL)
	{
		free(input_line);
		*status_malloc = 2;
		return (NULL);
	}
	expanded_line = heredoc_expand_word(line_copy, data, status_malloc);
	if (expanded_line == NULL)
	{
		free(input_line);
		return (NULL);
	}
	free(input_line);
	return (expanded_line);
}
