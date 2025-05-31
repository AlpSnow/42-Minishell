/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmarck <lmarck@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:19:44 by lmarck            #+#    #+#             */
/*   Updated: 2025/05/07 18:10:13 by lmarck           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*str_to_export(char *name, char *val);
static int	cd_error(char *arg, char *per, int er);
static void	cd_path_error(char *arg);
static char	*cd_path(char **arg, t_data *data);

int	bi_cd(char **arg, t_data *data)
{
	char	*oldpwd;
	char	*pwd;
	char	*path;

	if (count_line(arg) > 2)
		return (put_error("cd", NULL, "too many arguments\n"), 1);
	oldpwd = str_to_export("OLDPWD", getcwd(NULL, 0));
	if (!oldpwd)
		exit_minishell(NULL, "getcwd fail", GETCWD_FAIL, data);
	pwd = NULL;
	path = cd_path(arg, data);
	if (!path)
		return (free(oldpwd), 1);
	if (chdir(path) < 0)
		return (free(oldpwd), cd_error(path, NULL, 1));
	else
	{
		data->env = modify_env(data->env, oldpwd, data);
		pwd = str_to_export("PWD", getcwd(NULL, 0));
		if (!pwd)
			return (free(oldpwd), free(pwd), GETCWD_FAIL);
		data->env = modify_env(data->env, pwd, data);
	}
	return (free(pwd), free(oldpwd), 0);
}

static char	*cd_path(char **arg, t_data *data)
{
	char	*path;

	if (!arg[1])
	{
		path = get_env("HOME", data->env);
		if (!path)
		{
			cd_path_error("HOME not set \n");
			return (NULL);
		}
	}
	else if (!strcmp(arg[1], "-"))
	{
		path = get_env("OLDPWD", data->env);
		if (!path)
		{
			cd_path_error("OLDPWD not set \n");
			return (NULL);
		}
	}
	else
		path = arg[1];
	return (path);
}

static void	cd_path_error(char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
}

static int	cd_error(char *arg, char *per, int er)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(" ", 2);
	if (er)
		perror(per);
	return (1);
}

static char	*str_to_export(char *name, char *val)
{
	char	*str;
	char	*tmp;

	if (val == NULL)
		return (NULL);
	tmp = ft_strdup(name);
	str = add_chr('=', tmp);
	free(tmp);
	str = ft_str_realoc(str, val);
	free(val);
	return (str);
}
