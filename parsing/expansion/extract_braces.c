/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_braces.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 06:28:44 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/23 06:28:48 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	extract_content_from_braces(t_expansion_norm_helper *e)
{
	e->i += 2;
	e->start = e->i;
	while (e->full_str[e->i] != '\0' && e->full_str[e->i] != '}')
		e->i++;
	if (e->full_str[e->i] != '}')
	{
		write(2, ERROR_MISSING_BRACE, 41);
		free(e->new_full_str);
		*(e->status_code) = 1;
		e->data->last_exit_status = 2;
		return (1);
	}
	e->str = ft_strndup(e->full_str + e->start, e->i - e->start);
	if (e->str == NULL)
	{
		free(e->new_full_str);
		*(e->status_code) = 2;
		return (1);
	}
	e->i++;
	return (0);
}
