/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwallis <mwallis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:58:58 by mwallis           #+#    #+#             */
/*   Updated: 2025/05/30 20:58:52 by mwallis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stddef.h>
# include <limits.h>
# include <errno.h>
# include <termios.h>
# include <sys/stat.h>
# include "./libft/libft.h"

extern int	g_signal;

typedef enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_word_fragments
{
	char					*str;
	t_quote_type			quote_type;
	struct s_word_fragments	*next;
	struct s_word_fragments	*previous;
}	t_word_fragments;

typedef enum e_segment_type
{
	SEGMENT_UNSET,
	WORD,
	PIPE,
	INPUT_REDIRECTION,
	OUTPUT_REDIRECTION,
	APPEND,
	HEREDOC,
	END_OF_FILE,
	ERROR_QUOTE
}	t_segment_type;

typedef struct s_segment
{
	int					index;
	t_word_fragments	*content;
	t_segment_type		type;
	int					are_there_quotes_inside;
	struct s_segment	*next;
	struct s_segment	*previous;
}	t_segment;

typedef struct s_cmd
{
	int				n_redir;
	char			**filename_list;
	t_segment_type	*type_list;
	char			**arg;
}	t_cmd;

typedef struct s_heredoc_file
{
	char					*path;
	struct s_heredoc_file	*next;
}	t_heredoc_file;

typedef struct s_data
{
	char			**env;
	int				last_exit_status;
	t_heredoc_file	*heredoc_files;
	int				n_pipe;
	t_segment		*seg_list;
	t_cmd			**cmds;
	int				(*pipes)[2];
	pid_t			*pids;
}	t_data;

typedef struct s_lexer_norm_helper
{
	int				i;
	int				start;
	int				len;
	t_quote_type	quote_type;
}	t_lexer_norm_helper;

typedef struct s_heredoc_norm_helper
{
	char				*delimiter;
	int					fd;
	char				*heredoc_path;
	int					allow_expand;
	struct sigaction	previous_sigaction;
}	t_heredoc_norm_helper;

typedef struct s_expansion_norm_helper
{
	t_data	*data;
	char	*full_str;
	char	*new_full_str;
	int		i;
	int		start;
	char	*str;
	char	*tmp;
	int		*status_code;
}	t_expansion_norm_helper;

typedef struct s_error_str
{
	char	*arg;
	char	*cmd;
	char	*error;
}	t_error_str;

# define RED "\033[31m"
# define GRN "\033[32m"
# define YEL "\033[33m"
# define BLU "\033[34m"
# define MAG "\033[35m"
# define CYN "\033[36m"
# define WHT "\033[37m"
# define RESET "\033[0m"
# define RED_PROMPT "\001\033[31m\002"
# define GRN_PROMPT "\001\033[32m\002"
# define BLU_PROMPT "\001\033[34m\002"
# define RESET_PROMPT "\001\033[0m\002"

# define ERROR_ARGC "\033[31m minishell: this program does not accept \
arguments\033[0m\n"
# define ERROR_ENV "\033[31mminishell: failed to initialize environment: \
memory exhausted\033[0m\n"
# define ERROR_PROMPT "\033[31mminishell: failed to generate prompt: \
memory exhausted\033[0m\n"
# define ERROR_LEXER "\033[31mminishell: unable to tokenize input \
(lexer): memory exhausted\033[0m\n"
# define ERROR_INTERNAL_NULL_EXPAND "\033[31mminishell: internal \
error: unexpected NULL in expand_env_variables\033[0m\n"
# define ERROR_EXPAND_MALLOC "\033[31mminishell: failed to expand \
environment variable: memory exhausted\033[0m\n"
# define ERROR_HEREDOC_MALLOC "\033[31mminishell: failed to read \
heredoc input: memory exhausted\033[0m\n"
# define ERROR_REASSEMBLE "\033[31mminishell: failed to reassemble \
fragments: memory exhausted\033[0m\n"
# define DEFAULT_PATH "PATH=/usr/local/sbin:/usr/local/bin:\
/usr/sbin:/usr/bin:/sbin:/bin"
# define ERROR_SHLVL_INVALID "minishell: warning: shell level \
(SHLVL) invalid, resetting to 1\n"
# define ERROR_SHLVL_TOO_HIGH "minishell: warning: shell level \
(SHLVL) too high or invalid, resetting to 1\n"
# define ERROR_MISSING_BRACE "minishell: bad substitution: \
missing `}'\n"
# define ERROR_PID_EXPAND "minishell: failed to retrieve \
PID: not supported on this system\n"
# define ERROR_UNCLOSED_QUOTES "minishell: syntax error: \
unexpected EOF while looking for matching quote\n"
# define ERROR_UNSUPPORTED_SYMBOL "minishell: syntax error: \
unsupported Bash operator or special character : "
# define ERROR_INVALID_OPERATOR "minishell: syntax error near \
unexpected token `"
# define ERROR_OPERATOR_EOF "minishell: syntax error near \
unexpected token `newline'\n"
# define ERROR_HEREDOC_CTRLD_BEGIN "\nminishell: warning: \
here-document delimited by end-of-file (wanted `"
# define ERROR_HEREDOC_CTRLD_END "')\n"
# define ERROR_ARG "Error: This function does not take any \
arguments.\n"
# define ERROR_OPTION "Error: invalid option\n"
# define W_ID "not a valid identifier\n"

//fail return value :
# define MALLOC_FAIL 1
# define GETCWD_FAIL 1
# define WRITE_FAIL 1
# define FORK_FAIL 1

# define ECHOCTL 0001000

/* ********************************************************************** */
/*                                Parsing                                 */
/* ********************************************************************** */

// --------------------------- Initialization --------------------------- //
int					minishell_init(char **envp, t_data *data);
char				**init_minimal_env(void);
char				**complete_env_if_needed(char **env);
char				*increase_shlvl(char *old_shell_lvl);
void				sigint_handler(int signum);
// ------------------------------- Prompt ------------------------------- //
char				*get_prompt(t_data *data);
char				*get_host(void);
// ------------------------------- Lexer -------------------------------- //
t_segment			*lexer(char *input_line, t_data *data);
int					add_word_segment(char *str, t_segment **segment_list, \
									char *origin, t_data *data);
t_word_fragments	*split_word_into_fragments(const char *str, \
								int *chars_read, int *are_quotes_closed);
int					add_operator_segment(char *str, t_segment **segment_list, \
										char *origin, t_data *data);
t_segment			*create_segment(void);
void				segment_add_back(t_segment **segment_list, \
									t_segment *new_segment);
t_word_fragments	*create_fragment(char *str, t_quote_type quote_type);
void				fragment_add_back(t_word_fragments **fragments_list, \
									t_word_fragments *new_fragment);
void				free_segment_list(t_segment **segment);
void				free_word_fragments(t_word_fragments *fragment);
int					skip_white_space(char *str);
int					is_separator(char c);
void				lexer_error_exit(char *origin, \
					t_segment **segment_list, t_data *data);

// ----------------------------- Expansion ------------------------------ //
int					env_expansion_apply(t_segment *segment_list, t_data *data, \
					int is_heredoc);
int					env_expansion_apply_to_fragment(t_word_fragments *current, \
					t_data *data, t_segment **head, int is_heredoc);
int					process_brace_expansion(t_expansion_norm_helper *e);
int					process_dollar_expansion(t_expansion_norm_helper *e);
int					process_plain_text(t_expansion_norm_helper *e);
int					extract_content_from_braces(t_expansion_norm_helper *e);
int					expand_content_from_braces(t_expansion_norm_helper *e);
int					handle_exit_status_expansion(t_expansion_norm_helper *e);
int					handle_unsupported_pid(t_expansion_norm_helper *e);
int					handle_digit_expansion(t_expansion_norm_helper *e);
int					handle_isolated_dollar(t_expansion_norm_helper *e);
int					handle_regular_variable_expansion(\
					t_expansion_norm_helper *e);
int					is_valid_brace_expansion_syntax(char *str);
void				exit_on_critical_expand_error(int status_code, \
									t_data *data, t_segment **head);
int					exit_on_critical_expand_error_heredoc(int status_code);
int					handle_brace_bad_substitution(t_expansion_norm_helper *e);
// ------------------------------ Syntax -------------------------------- //
int					validate_syntax(t_segment *segment_list, t_data *data);
int					detect_unsupported_symbols(t_segment *segment_list, \
									t_segment **segment_head, t_data *data);
int					detect_invalid_operator_combination(\
			t_segment *segment_list, t_segment **segment_head, t_data *data);
int					detect_invalid_pipe_combination(t_segment *segment_list, \
			t_segment **segment_head, t_data *data);
int					detect_unexpected_eof_after_operator(\
			t_segment *segment_list, t_segment **segment_head, t_data *data);
int					detect_start_operator(t_segment *segment_list, \
									t_segment **segment_head, t_data *data);
int					ambiguous_redirect(t_segment *segment_list, t_data *data);
void				reassemble_fragments(t_segment *segment_list, t_data *data);

// ------------------------------ Heredoc ------------------------------- //
int					handle_heredoc(t_segment *segment_list, t_data *data);
char				*heredoc_join_fragments(t_word_fragments *fragments_list);
int					heredoc_create_file(int *fd, char **path, t_data *data);
void				heredoc_clean_file(t_heredoc_file **heredoc_file_lst);
int					read_and_write_heredoc_lines(t_data *data, \
					t_heredoc_norm_helper *h);
char				*heredoc_expand_if_needed(char *input_line, \
					t_data *data, int should_expand, int *status_malloc);
void				heredoc_sigint_handler(int signum);
void				set_heredoc_sigint_handler(struct sigaction \
					*previous_sigaction);
void				disable_echoctl(void);
void				enable_echoctl(void);
void				heredoc_unlink(t_heredoc_file **heredoc_file_lst);
void				heredoc_free(t_heredoc_file **heredoc_file_lst);

/*Exit ,Error & Free*/

void				exit_minishell(char *cmd, char *error, int ex, t_data *dt);
void				put_error(char *cmd, char *arg, char *error);
void				free_cmds(t_cmd **cmds);
void				free_all(t_data *data, int child);
void				error_perror(char *arg);

/*Environement*/

int					is_valid_var_name(char *arg);
char				*get_env(char *val, char **env);

/*Execution*/

int					built_in_exec(t_cmd *cmd, t_data *data);
int					is_build_in(char *name);
int					exec_command(t_cmd *cmd, t_data *data);
void				exec_extern(t_cmd *cmd, int i, t_data *data);
int					exec_minishell(t_cmd *cmd, t_data *data);
void				init_exec_extern(t_cmd *cmd, t_data *data);
char				**add_line_cmd(char **tab, char *str, t_data *data);
void				free_cmd(t_cmd *cmd);
t_cmd				*get_command(t_segment *segment_list, t_data *data);
void				handle_child_exit(int status, int *last_exit_status);
void				child_exec_extern(t_cmd *cmd, t_data *data);

/*pipe*/

int					exec_piped(t_segment *seg_list, t_data *data);
int					count_pipe(t_segment *seg_list);
void				close_unused_fd(t_data *data);
void				make_pipes(t_data *data);
void				sig_int_quit_ign(void);
void				sig_int_quit_reset(void);

/*redirection*/

int					is_redirection(t_segment *seg);
int					set_redirection(t_cmd *cmd, t_data *data, int i);
void				set_redirection_parent(t_cmd *cmd, t_data *data);
void				free_cmd(t_cmd *cmd);
int					open_target(int type, const char *name);
void				apply_redir(t_data *d, int type, int fd);
void				permission_denied(char *path, \
					char **env_paths, t_data *data);

/*Built-in functions*/

/****Exit */

void				bi_exit(char **arg, t_data *data);
void				free_tmp(t_data *data);
void				exit_exec_extern(char *cmd, char *f, \
					char **path, t_data *dt);

/****Export*/

int					bi_export(char **arg, t_data *data);
char				**modify_env(char **tab, char *line, t_data *data);
int					var_exist(char **tab, char *line, t_data *data);
void				print_export(char **tab);

/****Echo*/

int					bi_echo(char **arg, int i, t_data *data);
int					ft_putstrs_fd(char *s, int fd);

/****unset */

char				**delete_line(int line, char **tab, t_data *data);
int					find_index(char *arg, char **env);
int					bi_unset(char **arg, t_data *data);

/****env */

int					bi_env(char **arg, t_data *data);

/****pwd */

int					bi_pwd(char **arg, t_data *data);

/****cd */

int					bi_cd(char **arg, t_data *data);

/*Utils*/

char				**secure_split(char *line, char sep, t_data *data);
int					ft_is_zero(int n);
char				*ft_str_realoc(char *s1, char const *s2);
int					count_line(char **tab);
char				*add_chr(unsigned char buf, char *str);
void				free_tab(char **tab);
int					rev_strlcmp(const char *str, const char *suffix);

#endif
