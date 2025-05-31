/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:40:54 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 11:08:02 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	heredoc_write_line(char *str, int fd)
{
	size_t	len;

	if (str == NULL)
		return ;
	len = ft_strlen(str);
	write(fd, str, len);
	write(fd, "\n", 1);
}

static char	*heredoc_readline(void)
{
	char	*input_line;
	size_t	len;

	input_line = get_next_line(STDIN_FILENO, 0);
	if (input_line == NULL)
		return (NULL);
	len = ft_strlen(input_line);
	if (len != 0 && input_line[len - 1] == '\n')
		input_line[len - 1] = '\0';
	return (input_line);
}

static int	handle_heredoc_special_cases(char *input_line, char *delimiter,
	int fd, t_data *data)
{
	if (input_line == NULL && g_signal == SIGINT)
	{
		g_signal = 0;
		data->last_exit_status = 130;
		return (free(delimiter), close(fd), 10);
	}
	if (input_line == NULL)
	{
		write(2, ERROR_HEREDOC_CTRLD_BEGIN, \
			ft_strlen(ERROR_HEREDOC_CTRLD_BEGIN));
		write(2, delimiter, ft_strlen(delimiter));
		write(2, ERROR_HEREDOC_CTRLD_END, ft_strlen(ERROR_HEREDOC_CTRLD_END));
		return (11);
	}
	if (ft_strcmp(input_line, delimiter) == 0)
		return (free(input_line), 12);
	return (0);
}

static int	handle_expand_or_fail(char **input_line, t_data *data, \
									t_heredoc_norm_helper *h)
{
	int	status_malloc;

	status_malloc = 0;
	*input_line = heredoc_expand_if_needed(*input_line, data, \
									h->allow_expand, &status_malloc);
	if (*input_line == NULL)
	{
		g_signal = 0;
		get_next_line(STDIN_FILENO, 1);
		free(h->delimiter);
		close(h->fd);
		if (status_malloc == 1)
			return (1);
		else
			return (2);
	}
	return (0);
}

int	read_and_write_heredoc_lines(t_data *data, t_heredoc_norm_helper *h)
{
	char	*input_line;
	int		status_readline;
	int		status_expand;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		input_line = heredoc_readline();
		status_readline = handle_heredoc_special_cases(input_line, \
												h->delimiter, h->fd, data);
		if (status_readline == 10)
			return (1);
		else if (status_readline == 11)
			break ;
		else if (status_readline == 12)
			return (0);
		status_expand = handle_expand_or_fail(&input_line, data, h);
		if (status_expand != 0)
			return (status_expand);
		heredoc_write_line(input_line, h->fd);
		free(input_line);
	}
	return (0);
}
