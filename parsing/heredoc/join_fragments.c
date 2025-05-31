/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_fragments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:35:24 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 10:35:34 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*heredoc_join_fragments(t_word_fragments *fragments_list)
{
	char	*fullstr;
	char	*tmp;

	fullstr = ft_strdup("");
	if (fullstr == NULL)
	{
		return (NULL);
	}
	while (fragments_list != NULL && fullstr != NULL)
	{
		tmp = strjoin_safe(fullstr, fragments_list->str);
		free(fullstr);
		if (tmp == NULL)
		{
			return (NULL);
		}
		fullstr = tmp;
		fragments_list = fragments_list->next;
	}
	return (fullstr);
}
