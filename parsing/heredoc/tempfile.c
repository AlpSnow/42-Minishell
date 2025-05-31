/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tempfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:40:16 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 10:40:39 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	heredoc_clean_file(t_heredoc_file **heredoc_file_lst)
{
	t_heredoc_file	*tmp;

	while (heredoc_file_lst != NULL && *heredoc_file_lst != NULL)
	{
		tmp = (*heredoc_file_lst)->next;
		unlink((*heredoc_file_lst)->path);
		free((*heredoc_file_lst)->path);
		free(*heredoc_file_lst);
		*heredoc_file_lst = tmp;
	}
}

static int	heredoc_add_file(char *full_path, t_data *data)
{
	t_heredoc_file	*new_entry;

	new_entry = malloc(sizeof(t_heredoc_file));
	if (new_entry == NULL)
	{
		return (2);
	}
	new_entry->path = ft_strdup(full_path);
	if (new_entry->path == NULL)
	{
		free(new_entry);
		return (2);
	}
	new_entry->next = data->heredoc_files;
	data->heredoc_files = new_entry;
	return (0);
}

static int	handle_successful_heredoc_file(int fd, char **path, \
										char *full_path, t_data *data)
{
	*path = full_path;
	if (heredoc_add_file(full_path, data) == 2)
	{
		close(fd);
		unlink(full_path);
		free(full_path);
		return (2);
	}
	return (0);
}

int	heredoc_create_file(int *fd, char **path, t_data *data)
{
	static unsigned long	count = 0;
	char					*number;
	char					*full_path;

	while (1)
	{
		number = ft_itoa(count++);
		if (number == NULL)
			return (2);
		full_path = strjoin_safe("/tmp/minishell_heredoc_", number);
		free(number);
		if (full_path == NULL)
			return (2);
		*fd = open(full_path, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (*fd != -1)
			return (handle_successful_heredoc_file(*fd, path, full_path, data));
		if (errno == EEXIST)
		{
			free(full_path);
			continue ;
		}
		free(full_path);
		return (2);
	}
	return (2);
}
