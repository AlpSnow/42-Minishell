/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_segments.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:22:53 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/15 06:06:53 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	add_pipe_segment(char *origin, t_segment **segment_list, \
							t_data *data)
{
	t_segment	*new_segment;

	new_segment = create_segment();
	if (new_segment == NULL)
		lexer_error_exit(origin, segment_list, data);
	new_segment->type = PIPE;
	segment_add_back(segment_list, new_segment);
	return (1);
}

static int	add_single_operator_segment(char *str, t_segment **segment_list, \
										char *origin, t_data *data)
{
	t_segment	*new_segment;

	if (str[0] == '<')
	{
		new_segment = create_segment();
		if (new_segment == NULL)
			lexer_error_exit(origin, segment_list, data);
		new_segment->type = INPUT_REDIRECTION;
		segment_add_back(segment_list, new_segment);
		return (1);
	}
	else if (str[0] == '>')
	{
		new_segment = create_segment();
		if (new_segment == NULL)
			lexer_error_exit(origin, segment_list, data);
		new_segment->type = OUTPUT_REDIRECTION;
		segment_add_back(segment_list, new_segment);
		return (1);
	}
	else if (str[0] == '|')
		return (add_pipe_segment(origin, segment_list, data));
	return (0);
}

static int	add_double_operator_segment(char *str, t_segment **segment_list, \
										char *origin, t_data *data)
{
	t_segment	*new_segment;

	if (str[0] == '<' && str[1] != '\0' && str[1] == '<')
	{
		new_segment = create_segment();
		if (new_segment == NULL)
			lexer_error_exit(origin, segment_list, data);
		new_segment->type = HEREDOC;
		segment_add_back(segment_list, new_segment);
		return (2);
	}
	else if (str[0] == '>' && str[1] != '\0' && str[1] == '>')
	{
		new_segment = create_segment();
		if (new_segment == NULL)
			lexer_error_exit(origin, segment_list, data);
		new_segment->type = APPEND;
		segment_add_back(segment_list, new_segment);
		return (2);
	}
	return (0);
}

int	add_operator_segment(char *str, t_segment **segment_list, char *origin, \
							t_data *data)
{
	int	chars_read;

	chars_read = 0;
	chars_read = add_double_operator_segment(str, segment_list, origin, data);
	if (chars_read > 0)
		return (chars_read);
	chars_read = add_single_operator_segment(str, segment_list, origin, data);
	return (chars_read);
}
