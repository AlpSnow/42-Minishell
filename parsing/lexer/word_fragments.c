/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_fragments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 05:35:37 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/15 06:09:13 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	extract_quoted_fragment_info(const char *str, \
					t_lexer_norm_helper *norm_helper, int *are_quotes_closed)
{
	if (str[norm_helper->i] == '\'')
		norm_helper->quote_type = QUOTE_SINGLE;
	else
		norm_helper->quote_type = QUOTE_DOUBLE;
	norm_helper->i = norm_helper->i + 1;
	norm_helper->start = norm_helper->i;
	while (str[norm_helper->i] != '\0')
	{
		if (norm_helper->quote_type == QUOTE_SINGLE
			&& str[norm_helper->i] == '\'')
			break ;
		if (norm_helper->quote_type == QUOTE_DOUBLE
			&& str[norm_helper->i] == '\"')
			break ;
		norm_helper->i = norm_helper->i + 1;
	}
	if (str[norm_helper->i] == '\0')
	{
		*are_quotes_closed = -1;
		return (-1);
	}
	norm_helper->len = norm_helper->i - norm_helper->start;
	norm_helper->i = norm_helper->i + 1;
	return (0);
}

static void	extract_plain_fragment_info(const char *str, \
									t_lexer_norm_helper *norm_helper)
{
	norm_helper->start = norm_helper->i;
	norm_helper->quote_type = QUOTE_NONE;
	while (str[norm_helper->i] != '\0'
		&& is_separator(str[norm_helper->i]) == 0
		&& str[norm_helper->i] != '\''
		&& str[norm_helper->i] != '\"')
	{
		norm_helper->i = norm_helper->i + 1;
	}
	norm_helper->len = norm_helper->i - norm_helper->start;
}

static int	get_next_fragment(const char *str, int *i, \
					int *are_quotes_closed, t_word_fragments **fragment)
{
	t_lexer_norm_helper	norm_helper;
	char				*fragment_str;

	norm_helper.i = *i;
	if (str[norm_helper.i] == '\'' || str[norm_helper.i] == '\"')
	{
		if (extract_quoted_fragment_info(str, &norm_helper, \
									are_quotes_closed) == -1)
		{
			*i = norm_helper.i;
			return (-1);
		}
	}
	else
		extract_plain_fragment_info(str, &norm_helper);
	fragment_str = ft_strndup(str + norm_helper.start, norm_helper.len);
	if (fragment_str == NULL)
		return (-1);
	*fragment = create_fragment(fragment_str, norm_helper.quote_type);
	if (*fragment == NULL)
		return (free(fragment_str), -1);
	*i = norm_helper.i;
	return (0);
}

t_word_fragments	*split_word_into_fragments(const char *str, \
									int *chars_read, int *are_quotes_closed)
{
	t_word_fragments	*fragment_list;
	t_word_fragments	*fragment;
	int					fragment_status;
	int					i;

	fragment_list = NULL;
	i = 0;
	while (str[i] != '\0' && is_separator(str[i]) == 0)
	{
		fragment_status = get_next_fragment(str, &i, \
									are_quotes_closed, &fragment);
		if (fragment_status == -1)
		{
			free_word_fragments(fragment_list);
			fragment_list = NULL;
			break ;
		}
		fragment_add_back(&fragment_list, fragment);
	}
	*chars_read = i;
	return (fragment_list);
}
