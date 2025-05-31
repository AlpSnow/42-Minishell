/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reassemble_fragments.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 04:50:42 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 09:06:45 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	join_fragments_to_str(t_word_fragments *fragments, char **full_str)
{
	char	*tmp;

	*full_str = ft_strdup("");
	if (*full_str == NULL)
		return (1);
	while (fragments != NULL)
	{
		tmp = strjoin_safe(*full_str, fragments->str);
		free(*full_str);
		if (tmp == NULL)
			return (1);
		*full_str = tmp;
		fragments = fragments->next;
	}
	return (0);
}

static void	cleanup_and_replace(t_segment *segment, char *full_str)
{
	t_word_fragments	*current;
	t_word_fragments	*tmp;

	current = segment->content;
	free(current->str);
	current->str = full_str;
	current = current->next;
	segment->content->next = NULL;
	while (current)
	{
		tmp = current->next;
		free(current->str);
		free(current);
		current = tmp;
	}
}

void	reassemble_fragments(t_segment *segment_list, t_data *data)
{
	char				*full_str;
	t_word_fragments	*current;
	t_segment			*segment_head;

	segment_head = segment_list;
	while (segment_list != NULL && segment_list->type != END_OF_FILE)
	{
		if (segment_list->type == WORD)
		{
			current = segment_list->content;
			if (join_fragments_to_str(current, &full_str) == 1)
			{
				write(2, ERROR_REASSEMBLE, ft_strlen(ERROR_REASSEMBLE));
				heredoc_clean_file(&data->heredoc_files);
				free_segment_list(&segment_head);
				rl_clear_history();
				free_tab(data->env);
				exit(1);
			}
			cleanup_and_replace(segment_list, full_str);
		}
		segment_list = segment_list->next;
	}
}
