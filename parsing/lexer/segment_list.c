/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:30:18 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/15 06:06:49 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_segment	*create_segment(void)
{
	t_segment	*segment;

	segment = (t_segment *)malloc(sizeof(t_segment));
	if (segment == NULL)
		return (NULL);
	segment->index = 0;
	segment->content = NULL;
	segment->type = SEGMENT_UNSET;
	segment->are_there_quotes_inside = 0;
	segment->next = NULL;
	segment->previous = NULL;
	return (segment);
}

void	segment_add_back(t_segment **segment_list, t_segment *new_segment)
{
	t_segment	*current;

	if (segment_list == NULL || new_segment == NULL)
		return ;
	if (*segment_list == NULL)
	{
		*segment_list = new_segment;
		return ;
	}
	current = *segment_list;
	while (current->next != NULL)
	{
		current = current->next;
	}
	new_segment->index = (current->index + 1);
	current->next = new_segment;
	new_segment->previous = current;
}

t_word_fragments	*create_fragment(char *str, t_quote_type quote_type)
{
	t_word_fragments	*fragment;

	fragment = (t_word_fragments *)malloc(sizeof(t_word_fragments));
	if (fragment == NULL)
		return (NULL);
	fragment->str = str;
	fragment->quote_type = quote_type;
	fragment->next = NULL;
	fragment->previous = NULL;
	return (fragment);
}

void	fragment_add_back(t_word_fragments **fragments_list, \
							t_word_fragments *new_fragment)
{
	t_word_fragments	*current;

	if (fragments_list == NULL || new_fragment == NULL)
		return ;
	if (*fragments_list == NULL)
	{
		*fragments_list = new_fragment;
		return ;
	}
	current = *fragments_list;
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = new_fragment;
	new_fragment->previous = current;
}
