/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hostname.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 00:48:59 by mwallis           #+#    #+#             */
/*   Updated: 2025/04/15 00:49:15 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	clean_hostname(char *str)
{
	char	*newline;
	char	*dot;

	newline = ft_strchr(str, '\n');
	if (newline != NULL)
		*newline = '\0';
	dot = ft_strchr(str, '.');
	if (dot != NULL)
		*dot = '\0';
}

char	*get_host(void)
{
	int		fd;
	char	*str;
	int		len;

	fd = open("/etc/hostname", O_RDONLY);
	if (fd < 0)
		return (NULL);
	str = malloc(sizeof(char) * 256);
	if (str == NULL)
		return (close(fd), NULL);
	len = read(fd, str, 255);
	close(fd);
	if (len <= 0)
		return (free(str), NULL);
	str[len] = '\0';
	clean_hostname(str);
	return (str);
}
