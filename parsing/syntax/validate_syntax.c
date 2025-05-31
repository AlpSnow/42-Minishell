/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:34:42 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/23 06:53:40 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	detect_unclosed_quotes(t_segment *segment_list, \
								t_segment **segment_head, t_data *data)
{
	if (segment_list->type == ERROR_QUOTE)
	{
		free_segment_list(segment_head);
		write(2, ERROR_UNCLOSED_QUOTES, 73);
		data->last_exit_status = 2;
		return (1);
	}
	return (0);
}

int	validate_syntax(t_segment *segment_list, t_data *data)
{
	t_segment	*segment_head;

	segment_head = segment_list;
	while (segment_list != NULL)
	{
		if (detect_unclosed_quotes(segment_list, &segment_head, data) == 1)
			return (1);
		if (detect_unsupported_symbols(segment_list, &segment_head, data) == 1)
			return (1);
		if (detect_invalid_operator_combination(segment_list, \
			&segment_head, data) == 1)
			return (1);
		if (detect_invalid_pipe_combination(segment_list, \
			&segment_head, data) == 1)
			return (1);
		if (detect_unexpected_eof_after_operator(segment_list, \
			&segment_head, data) == 1)
			return (1);
		if (detect_start_operator(segment_list, &segment_head, data) == 1)
			return (1);
		segment_list = segment_list->next;
	}
	return (0);
}
