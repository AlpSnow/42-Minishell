/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid_operators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:34:17 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/23 06:46:53 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	write_invalid_operator(t_segment_type invalid_operator)
{
	if (invalid_operator == INPUT_REDIRECTION)
		write(2, "<", 1);
	else if (invalid_operator == OUTPUT_REDIRECTION)
		write(2, ">", 1);
	else if (invalid_operator == APPEND)
		write(2, ">>", 2);
	else if (invalid_operator == HEREDOC)
		write(2, "<<", 2);
	else if (invalid_operator == PIPE)
		write(2, "|", 1);
}

int	detect_invalid_operator_combination(t_segment *segment_list, \
									t_segment **segment_head, t_data *data)
{
	if ((segment_list->type == INPUT_REDIRECTION
			|| segment_list->type == OUTPUT_REDIRECTION
			|| segment_list->type == APPEND
			|| segment_list->type == HEREDOC)
		&& segment_list->next != NULL
		&& (segment_list->next->type == INPUT_REDIRECTION
			|| segment_list->next->type == OUTPUT_REDIRECTION
			|| segment_list->next->type == APPEND
			|| segment_list->next->type == HEREDOC
			|| segment_list->next->type == PIPE))
	{
		write(2, ERROR_INVALID_OPERATOR, ft_strlen(ERROR_INVALID_OPERATOR));
		write_invalid_operator(segment_list->next->type);
		write(2, "'\n", 2);
		free_segment_list(segment_head);
		data->last_exit_status = 1;
		return (1);
	}
	return (0);
}

int	detect_invalid_pipe_combination(t_segment *segment_list, \
									t_segment **segment_head, t_data *data)
{
	if (segment_list->type == PIPE
		&& segment_list->next != NULL
		&& segment_list->next->type == PIPE)
	{
		write(2, ERROR_INVALID_OPERATOR, ft_strlen(ERROR_INVALID_OPERATOR));
		write_invalid_operator(segment_list->next->type);
		write(2, "'\n", 2);
		free_segment_list(segment_head);
		data->last_exit_status = 1;
		return (1);
	}
	return (0);
}

int	detect_unexpected_eof_after_operator(t_segment *segment_list, \
								t_segment **segment_head, t_data *data)
{
	if ((segment_list->type == INPUT_REDIRECTION
			|| segment_list->type == OUTPUT_REDIRECTION
			|| segment_list->type == APPEND
			|| segment_list->type == HEREDOC
			|| segment_list->type == PIPE)
		&& segment_list->next != NULL
		&& segment_list->next->type == END_OF_FILE)
	{
		write(2, ERROR_OPERATOR_EOF, ft_strlen(ERROR_OPERATOR_EOF));
		free_segment_list(segment_head);
		data->last_exit_status = 1;
		return (1);
	}
	return (0);
}

int	detect_start_operator(t_segment *segment_list, \
						t_segment **segment_head, t_data *data)
{
	if (segment_list->type == PIPE && segment_list->previous == NULL)
	{
		write(2, ERROR_INVALID_OPERATOR, ft_strlen(ERROR_INVALID_OPERATOR));
		write_invalid_operator(segment_list->type);
		write(2, "'\n", 2);
		free_segment_list(segment_head);
		data->last_exit_status = 1;
		return (1);
	}
	return (0);
}
