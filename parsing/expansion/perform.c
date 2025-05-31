/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perform.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 09:19:33 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 09:24:46 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*perform_env_expansion(char *full_str, t_data *data, \
									int *status_code)
{
	t_expansion_norm_helper	e;

	if (full_str == NULL)
		return (*status_code = 99, NULL);
	e.data = data;
	e.full_str = full_str;
	e.i = 0;
	e.status_code = status_code;
	*status_code = 0;
	e.new_full_str = ft_strdup("");
	if (e.new_full_str == NULL)
		return (*status_code = 2, NULL);
	while (e.full_str[e.i] != '\0')
	{
		if (e.full_str[e.i] == '$' && e.full_str[e.i + 1] == '{'
			&& process_brace_expansion(&e) != 0)
			return (NULL);
		else if (e.full_str[e.i] == '$' && e.full_str[e.i + 1] != '{'
			&& process_dollar_expansion(&e) != 0)
			return (NULL);
		else if (process_plain_text(&e) != 0)
			return (NULL);
	}
	return (e.new_full_str);
}

static int	update_fragment_str(t_word_fragments *current, char *expanded)
{
	char	*new_str;

	new_str = ft_strdup(expanded);
	if (new_str == NULL)
		return (1);
	free(current->str);
	current->str = new_str;
	return (0);
}

int	env_expansion_apply_to_fragment(t_word_fragments *current,
t_data *data, t_segment **head, int is_heredoc)
{
	char	*expanded;
	int		status_code;

	status_code = 0;
	expanded = perform_env_expansion(current->str, data, &status_code);
	if (expanded == NULL)
	{
		if (status_code == 1)
			return (free_segment_list(head), 1);
		if (is_heredoc == 0)
			exit_on_critical_expand_error(status_code, data, head);
		else
			return (exit_on_critical_expand_error_heredoc(status_code));
	}
	if (update_fragment_str(current, expanded) == 1)
	{
		free(expanded);
		if (is_heredoc == 0)
			exit_on_critical_expand_error(2, data, head);
		else
			return (exit_on_critical_expand_error_heredoc(2));
	}
	free(expanded);
	return (0);
}
