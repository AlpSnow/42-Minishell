/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiguous_redirect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:34:05 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/23 06:40:33 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	has_ifs_white(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

static int	is_ambiguous(t_word_fragments *fragment)
{
	int	nb_words;

	nb_words = 0;
	while (fragment != NULL)
	{
		if (fragment->quote_type == QUOTE_NONE)
		{
			if (fragment->str[0] == '\0')
				;
			else if (has_ifs_white(fragment->str) == 1)
				return (1);
			else
				nb_words += 1;
		}
		else
			nb_words += 1;
		fragment = fragment->next;
	}
	if (nb_words == 0)
		return (1);
	else
		return (0);
}

static int	detect_ambiguous_redirect(t_segment *segment_list, \
							t_segment **segment_head, t_data *data)
{
	if (!(segment_list->type == INPUT_REDIRECTION
			|| segment_list->type == OUTPUT_REDIRECTION
			|| segment_list->type == APPEND
			|| segment_list->type == HEREDOC))
		return (0);
	if (segment_list->next == NULL || segment_list->next->type != WORD)
		return (0);
	if (is_ambiguous(segment_list->next->content) == 1)
	{
		write(2, "minishell: ambiguous redirect\n", 30);
		heredoc_clean_file(&data->heredoc_files);
		free_segment_list(segment_head);
		data->last_exit_status = 1;
		return (1);
	}
	return (0);
}

int	ambiguous_redirect(t_segment *segment_list, t_data *data)
{
	t_segment	*segment_head;

	segment_head = segment_list;
	while (segment_list != NULL)
	{
		if (detect_ambiguous_redirect(segment_list, &segment_head, data) == 1)
			return (1);
		segment_list = segment_list->next;
	}
	return (0);
}
