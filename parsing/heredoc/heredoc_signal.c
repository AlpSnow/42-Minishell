/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:38:25 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/09 10:40:04 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	heredoc_sigint_handler(int signum)
{
	g_signal = signum;
	write(1, "^C\n", 3);
}

void	set_heredoc_sigint_handler(struct sigaction *previous_sigaction)
{
	struct sigaction	heredoc_sigaction;

	heredoc_sigaction.sa_handler = heredoc_sigint_handler;
	sigemptyset(&heredoc_sigaction.sa_mask);
	heredoc_sigaction.sa_flags = 0;
	sigaction(SIGINT, &heredoc_sigaction, previous_sigaction);
}

void	disable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	enable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
