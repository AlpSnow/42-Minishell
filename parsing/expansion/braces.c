/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   braces.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:28:01 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/23 06:28:05 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	handle_brace_digit_expansion(t_expansion_norm_helper *e)
{
	int	j;

	if (ft_isdigit(e->str[0]) == 0)
		return (-1);
	j = 1;
	while (e->str[j] != '\0')
	{
		if (ft_isdigit(e->str[j]) == 0)
			return (handle_brace_bad_substitution(e));
		j++;
	}
	return (0);
}

static int	handle_brace_special_variable_expansion(t_expansion_norm_helper *e)
{
	int	expansion_brace_status;

	if (e->str[0] == '?' && e->str[1] == '\0')
	{
		e->tmp = ft_itoa(e->data->last_exit_status);
		if (e->tmp == NULL)
			return (free(e->str), free(e->new_full_str), \
					*(e->status_code) = 2, 1);
		return (2);
	}
	if (e->str[0] == '$' && e->str[1] == '\0')
	{
		write(2, ERROR_PID_EXPAND, 64);
		free(e->str);
		free(e->new_full_str);
		*(e->status_code) = 1;
		e->data->last_exit_status = 127;
		return (1);
	}
	expansion_brace_status = handle_brace_digit_expansion(e);
	if (expansion_brace_status != -1)
		return (expansion_brace_status);
	if (is_valid_brace_expansion_syntax(e->str) == 0)
		return (handle_brace_bad_substitution(e));
	return (0);
}

static int	handle_brace_regular_variable_expansion(t_expansion_norm_helper *e)
{
	char	*env_value;

	env_value = get_env(e->str, e->data->env);
	if (env_value != NULL)
		e->tmp = ft_strdup(env_value);
	else
		e->tmp = ft_strdup("");
	if (e->tmp == NULL)
	{
		free(e->str);
		free(e->new_full_str);
		*(e->status_code) = 2;
		return (1);
	}
	return (0);
}

static int	finalize_brace_expansion(t_expansion_norm_helper *e)
{
	free(e->str);
	e->str = strjoin_safe(e->new_full_str, e->tmp);
	free(e->new_full_str);
	free(e->tmp);
	if (e->str == NULL)
	{
		*(e->status_code) = 2;
		return (1);
	}
	e->new_full_str = e->str;
	return (0);
}

int	expand_content_from_braces(t_expansion_norm_helper *e)
{
	int	expansion_brace_status;

	expansion_brace_status = handle_brace_special_variable_expansion(e);
	if (expansion_brace_status != 0)
	{
		if (expansion_brace_status == 2)
			return (finalize_brace_expansion(e));
		return (1);
	}
	if (handle_brace_regular_variable_expansion(e) != 0)
		return (1);
	return (finalize_brace_expansion(e));
}
