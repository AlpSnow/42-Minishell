/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsupported_symbols.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:34:29 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/06 09:16:15 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	init_unsupported_symbols_beginning(const char \
											**unsupported_symbols, int *i)
{
	unsupported_symbols[(*i)++] = "&&";
	unsupported_symbols[(*i)++] = "||";
	unsupported_symbols[(*i)++] = "!";
	unsupported_symbols[(*i)++] = ";";
	unsupported_symbols[(*i)++] = "&";
	unsupported_symbols[(*i)++] = "(";
	unsupported_symbols[(*i)++] = ")";
}

static void	init_unsupported_symbols(const char **unsupported_symbols)
{
	int	i;

	i = 0;
	init_unsupported_symbols_beginning(unsupported_symbols, &i);
	unsupported_symbols[i++] = "[";
	unsupported_symbols[i++] = "]";
	unsupported_symbols[i++] = "[[";
	unsupported_symbols[i++] = "]]";
	unsupported_symbols[i++] = "((";
	unsupported_symbols[i++] = "))";
	unsupported_symbols[i++] = "*";
	unsupported_symbols[i++] = "\\";
	unsupported_symbols[i++] = "~";
	unsupported_symbols[i++] = "#";
	unsupported_symbols[i++] = "$(";
	unsupported_symbols[i++] = "$[";
	unsupported_symbols[i] = NULL;
}

static int	has_fragment_unsupported_symbol(char *str, \
	const char **unsupported_symbols, t_segment **segment_head, t_data *data)
{
	int	i;

	i = 0;
	while (unsupported_symbols[i] != NULL)
	{
		if (ft_strnstr(str, unsupported_symbols[i], ft_strlen(str)) != NULL)
		{
			free_segment_list(segment_head);
			write(2, ERROR_UNSUPPORTED_SYMBOL, 74);
			write(2, unsupported_symbols[i], ft_strlen(unsupported_symbols[i]));
			write(2, "\n", 1);
			data->last_exit_status = 1;
			return (1);
		}
		i++;
	}
	return (0);
}

int	detect_unsupported_symbols(t_segment *segment_list, \
							t_segment **segment_head, t_data *data)
{
	const char			*unsupported_symbols[21];
	t_word_fragments	*current;

	if (segment_list->previous != NULL
		&& segment_list->previous->type == HEREDOC)
		return (0);
	if (segment_list->type == WORD)
	{
		init_unsupported_symbols(unsupported_symbols);
		current = segment_list->content;
		while (current != NULL)
		{
			if (current->quote_type == QUOTE_NONE)
			{
				if (has_fragment_unsupported_symbol(current->str, \
					unsupported_symbols, segment_head, data) == 1)
				{
					return (1);
				}
			}
			current = current->next;
		}
	}
	return (0);
}
