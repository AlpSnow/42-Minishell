/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:28:12 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 09:23:50 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	process_brace_expansion(t_expansion_norm_helper *e)
{
	if (extract_content_from_braces(e) != 0)
		return (1);
	if (e->str[0] == '\0')
		return (handle_brace_bad_substitution(e));
	if (expand_content_from_braces(e) != 0)
		return (1);
	return (0);
}

int	process_dollar_expansion(t_expansion_norm_helper *e)
{
	e->i += 1;
	if (e->full_str[e->i] == '?')
		return (handle_exit_status_expansion(e));
	if (e->full_str[e->i] == '$')
		return (handle_unsupported_pid(e));
	if (ft_isdigit(e->full_str[e->i]) == 1)
		return (handle_digit_expansion(e));
	if (e->full_str[e->i] == '\0'
		|| (ft_isalnum(e->full_str[e->i]) == 0 && e->full_str[e->i] != '_'))
		return (handle_isolated_dollar(e));
	return (handle_regular_variable_expansion(e));
}

int	process_plain_text(t_expansion_norm_helper *e)
{
	e->start = e->i;
	while (e->full_str[e->i] != '\0' && e->full_str[e->i] != '$')
		e->i++;
	e->str = ft_strndup(e->full_str + e->start, e->i - e->start);
	if (e->str == NULL)
	{
		*(e->status_code) = 2;
		return (1);
	}
	e->tmp = strjoin_safe(e->new_full_str, e->str);
	free(e->new_full_str);
	free(e->str);
	if (e->tmp == NULL)
	{
		*(e->status_code) = 2;
		return (1);
	}
	e->new_full_str = e->tmp;
	return (0);
}
