/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:28:33 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 09:24:09 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	is_valid_brace_expansion_syntax(char *str)
{
	int	i;

	i = 0;
	if (str == NULL || str[i] == '\0')
		return (0);
	if (ft_isalpha(str[i]) == 0 && str[i] != '_')
		return (0);
	while (str[i] != '\0')
	{
		if (ft_isalnum(str[i]) == 0 && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	exit_on_critical_expand_error(int status_code, t_data *data, \
										t_segment **head)
{
	if (status_code == 2)
		write(2, ERROR_EXPAND_MALLOC, 76);
	else if (status_code == 99)
		write(2, ERROR_INTERNAL_NULL_EXPAND, 76);
	free_segment_list(head);
	rl_clear_history();
	free_tab(data->env);
	heredoc_clean_file(&data->heredoc_files);
	if (status_code == 99)
		exit(99);
	else if (status_code == 2)
		exit (1);
}

int	exit_on_critical_expand_error_heredoc(int status_code)
{
	if (status_code == 2)
		write(2, ERROR_EXPAND_MALLOC, 76);
	else if (status_code == 99)
		write(2, ERROR_INTERNAL_NULL_EXPAND, 76);
	if (status_code == 99)
		return (99);
	else
		return (2);
}

int	handle_brace_bad_substitution(t_expansion_norm_helper *e)
{
	write(2, "minishell: ${", 13);
	write(2, e->str, ft_strlen(e->str));
	write(2, "}: bad substitution\n", 21);
	free(e->str);
	free(e->new_full_str);
	*(e->status_code) = 1;
	e->data->last_exit_status = 1;
	return (1);
}
