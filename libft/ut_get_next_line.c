/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ut_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:56:56 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/06 09:12:55 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*read_and_store(int fd, char *fullstr)
{
	char	*str;
	char	*tmp;
	int		number_read;

	str = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (str == NULL)
		return (NULL);
	number_read = 1;
	while (number_read > 0)
	{
		number_read = read(fd, str, BUFFER_SIZE);
		if (number_read == -1)
			return (free(str), free(fullstr), NULL);
		str[number_read] = '\0';
		tmp = strjoin_safe(fullstr, str);
		free(fullstr);
		if (!tmp)
			return (free(str), NULL);
		fullstr = tmp;
		if (ft_strchr(str, '\n'))
			break ;
	}
	free(str);
	return (fullstr);
}

static char	*extract_line(char *fullstr, char **line, char *stash)
{
	char	*newline_position;
	size_t	len;

	newline_position = ft_strchr(fullstr, '\n');
	if (newline_position == NULL)
	{
		*line = ft_strdup(fullstr);
		if (!*line)
			return (NULL);
		free(stash);
		stash = NULL;
	}
	else
	{
		len = newline_position - fullstr + 1;
		*line = ft_substr(fullstr, 0, len);
		if (!*line)
			return (NULL);
		stash = ft_strdup(newline_position + 1);
		if (!stash)
			return (free(*line), *line = NULL, NULL);
	}
	return (stash);
}

char	*get_next_line(int fd, int purge_stdin)
{
	static char	*stash[FD_SETSIZE];
	char		*line;
	char		*fullstr;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > (FD_SETSIZE - 1))
		return (NULL);
	if (purge_stdin == 1)
		return (free(stash[fd]), stash[fd] = NULL, NULL);
	line = NULL;
	fullstr = strjoin_safe(stash[fd], "");
	free(stash[fd]);
	stash[fd] = NULL;
	if (fullstr == NULL)
		return (NULL);
	fullstr = read_and_store(fd, fullstr);
	if (fullstr == NULL)
		return (NULL);
	else if (fullstr[0] == '\0')
	{
		free(fullstr);
		return (NULL);
	}
	stash[fd] = extract_line(fullstr, &line, stash[fd]);
	free(fullstr);
	return (line);
}
