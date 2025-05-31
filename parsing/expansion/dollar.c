/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:28:23 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/23 06:30:34 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	handle_exit_status_expansion(t_expansion_norm_helper *e)
{
	e->tmp = ft_itoa(e->data->last_exit_status);
	if (e->tmp == NULL)
	{
		free(e->new_full_str);
		*(e->status_code) = 2;
		return (1);
	}
	e->str = strjoin_safe(e->new_full_str, e->tmp);
	free(e->tmp);
	if (e->str == NULL)
	{
		free(e->new_full_str);
		*(e->status_code) = 2;
		return (1);
	}
	free(e->new_full_str);
	e->new_full_str = e->str;
	e->i += 1;
	return (0);
}

int	handle_unsupported_pid(t_expansion_norm_helper *e)
{
	write(2, ERROR_PID_EXPAND, 64);
	free(e->new_full_str);
	*(e->status_code) = 1;
	e->data->last_exit_status = 127;
	return (1);
}

int	handle_digit_expansion(t_expansion_norm_helper *e)
{
	char	number[2];

	number[0] = e->full_str[e->i];
	number[1] = '\0';
	e->tmp = get_env(number, e->data->env);
	if (e->tmp != NULL)
		e->str = strjoin_safe(e->new_full_str, e->tmp);
	else
		e->str = strjoin_safe(e->new_full_str, "");
	if (e->str == NULL)
	{
		free(e->new_full_str);
		*(e->status_code) = 2;
		return (1);
	}
	free(e->new_full_str);
	e->new_full_str = e->str;
	e->i += 1;
	return (0);
}

int	handle_isolated_dollar(t_expansion_norm_helper *e)
{
	e->str = strjoin_safe(e->new_full_str, "$");
	if (e->str == NULL)
	{
		free(e->new_full_str);
		*(e->status_code) = 2;
		return (1);
	}
	free(e->new_full_str);
	e->new_full_str = e->str;
	return (0);
}

int	handle_regular_variable_expansion(t_expansion_norm_helper *e)
{
	char	*env_value;

	e->start = e->i;
	while (e->full_str[e->i] != '\0'
		&& (ft_isalnum(e->full_str[e->i]) == 1 || e->full_str[e->i] == '_'))
		e->i++;
	e->str = ft_strndup(e->full_str + e->start, e->i - e->start);
	if (e->str == NULL)
		return (free(e->new_full_str), *(e->status_code) = 2, 1);
	env_value = get_env(e->str, e->data->env);
	free(e->str);
	if (env_value != NULL)
		e->str = strjoin_safe(e->new_full_str, env_value);
	else
		e->str = strjoin_safe(e->new_full_str, "");
	if (e->str == NULL)
		return (free(e->new_full_str), *(e->status_code) = 2, 1);
	free(e->new_full_str);
	e->new_full_str = e->str;
	return (0);
}
