/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segment_list_free.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:34:18 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/15 05:35:20 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	free_word_fragments(t_word_fragments *fragment)
{
	t_word_fragments	*tmp;

	while (fragment != NULL)
	{
		tmp = fragment->next;
		if (fragment->str != NULL)
			free(fragment->str);
		free(fragment);
		fragment = tmp;
	}
}

void	free_segment_list(t_segment **segment)
{
	t_segment	*tmp;

	while (*segment != NULL)
	{
		tmp = (*segment)->next;
		if ((*segment)->content != NULL)
			free_word_fragments((*segment)->content);
		free(*segment);
		*segment = tmp;
	}
}
