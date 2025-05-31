/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 22:00:39 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/30 21:07:47 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal;

static void	shutdown_shell_eof(t_data *data)
{
	write(1, "exit\n", 5);
	rl_clear_history();
	free_tab(data->env);
	exit(0);
}

static char	*prompt_and_read_line(t_data *data)
{
	char	*prompt;
	char	*input_line;

	prompt = get_prompt(data);
	if (prompt == NULL)
	{
		write(2, ERROR_PROMPT, 65);
		rl_clear_history();
		free_tab(data->env);
		exit(1);
	}
	input_line = readline(prompt);
	free(prompt);
	if (g_signal == SIGINT)
	{
		data->last_exit_status = 130;
		g_signal = 0;
	}
	if (input_line == NULL)
		shutdown_shell_eof(data);
	return (input_line);
}

static int	parse_and_validate_input(char *input_line, \
								t_segment **segment_list, t_data *data)
{
	if (input_line[0] == '\0')
		return (free(input_line), 1);
	*segment_list = lexer(input_line, data);
	if ((*segment_list)->type == END_OF_FILE)
	{
		free_segment_list(segment_list);
		free(input_line);
		return (1);
	}
	add_history(input_line);
	free(input_line);
	if (validate_syntax(*segment_list, data) == 1)
		return (1);
	if (handle_heredoc(*segment_list, data) == 1)
	{
		heredoc_clean_file(&data->heredoc_files);
		free_segment_list(segment_list);
		return (1);
	}
	if (env_expansion_apply(*segment_list, data, 0) == 1)
		return (heredoc_clean_file(&data->heredoc_files), 1);
	if (ambiguous_redirect(*segment_list, data) == 1)
		return (1);
	return (0);
}

static void	execute_and_cleanup(t_segment *segment_list, t_data *data)
{
	data->seg_list = segment_list;
	data->n_pipe = count_pipe(segment_list);
	data->last_exit_status = exec_piped(segment_list, data);
	heredoc_unlink(&data->heredoc_files);
	free_tmp(data);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input_line;
	t_segment	*segment_list;
	t_data		data;

	(void)argv;
	if (argc != 1)
		return (write (2, ERROR_ARGC, 60), 1);
	if (!isatty(1))
		return (ft_putstr_fd("Error output is not a terminal\n", 2), 1);
	if (!isatty(0))
		return (ft_putstr_fd("Error intput is not a terminal\n", 2), 1);
	if (minishell_init(envp, &data) == -1)
		return (1);
	while (1)
	{
		input_line = prompt_and_read_line(&data);
		if (parse_and_validate_input(input_line, &segment_list, &data) == 1)
			continue ;
		reassemble_fragments(segment_list, &data);
		execute_and_cleanup(segment_list, &data);
	}
	return (0);
}
