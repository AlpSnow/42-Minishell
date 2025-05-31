/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 04:37:41 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/15 04:40:27 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	skip_white_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	return (i);
}

int	is_separator(char c)
{
	if (c == ' '
		|| c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

void	lexer_error_exit(char *origin, t_segment **segment_list, t_data *data)
{
	write(2, ERROR_LEXER, 72);
	free(origin);
	free_segment_list(segment_list);
	rl_clear_history();
	free_tab(data->env);
	exit(1);
}
