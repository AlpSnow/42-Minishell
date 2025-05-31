/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 04:36:30 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/23 06:27:06 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	parse_input_line_into_segments(char *input_line, \
						t_segment **segment_list, char *origin, t_data *data)
{
	int	i;

	i = 0;
	while (input_line[i] != '\0')
	{
		i += skip_white_space(&input_line[i]);
		if (input_line[i] == '\0')
			break ;
		if (input_line[i] == '|'
			|| input_line[i] == '<' || input_line[i] == '>')
			i += add_operator_segment(&input_line[i], segment_list, \
										origin, data);
		else
			i += add_word_segment(&input_line[i], segment_list, origin, data);
	}
}

t_segment	*lexer(char *input_line, t_data *data)
{
	t_segment	*segment_list;
	t_segment	*segment_end;
	char		*origin;

	origin = input_line;
	segment_list = NULL;
	parse_input_line_into_segments(input_line, &segment_list, origin, data);
	segment_end = create_segment();
	if (segment_end == NULL)
		lexer_error_exit(origin, &segment_list, data);
	segment_end->type = END_OF_FILE;
	segment_add_back(&segment_list, segment_end);
	return (segment_list);
}
