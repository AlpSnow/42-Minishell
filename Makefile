# Dossier des fichiers objets
BUILD_DIR = build

# Fichiers sources
SRC_FILES = minishell.c \
parsing/initialization/minishell_init.c \
parsing/initialization/env_minimal_init.c \
parsing/initialization/env_completion.c \
parsing/initialization/shlvl.c \
parsing/initialization/signal.c \
parsing/prompt/prompt.c \
parsing/prompt/hostname.c \
parsing/lexer/lexer.c \
parsing/lexer/word_segments.c \
parsing/lexer/word_fragments.c \
parsing/lexer/operator_segments.c \
parsing/lexer/segment_list.c \
parsing/lexer/segment_list_free.c \
parsing/lexer/lexer_utils.c \
parsing/heredoc/exec.c \
parsing/heredoc/join_fragments.c \
parsing/heredoc/tempfile.c \
parsing/heredoc/read.c \
parsing/heredoc/heredoc_expand.c \
parsing/heredoc/heredoc_signal.c \
parsing/expansion/apply.c \
parsing/expansion/perform.c \
parsing/expansion/dispatch.c \
parsing/expansion/braces.c \
parsing/expansion/extract_braces.c \
parsing/expansion/dollar.c \
parsing/expansion/error_utils.c \
parsing/syntax/validate_syntax.c \
parsing/syntax/unsupported_symbols.c \
parsing/syntax/invalid_operators.c \
parsing/syntax/ambiguous_redirect.c \
builtins/exit.c \
builtins/export.c \
builtins/echo.c \
builtins/unset.c \
builtins/env.c \
builtins/pwd.c \
builtins/cd.c \
execution/execution.c\
execution/execution_extern.c\
execution/init_redirection.c\
execution/redirection.c\
execution/execution_utils.c\
execution/pipe_utils.c\
execution/pipe.c\
utils/minishell_utils.c \
utils/minishell_utils_extra.c \
utils/exit_minishell.c \
utils/free_helper.c \
utils/reassemble_fragments.c\
utils/clean_heredoc.c\

# Transformation des fichiers sources en fichiers objets
OBJFILES = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Compilation
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBS = -lreadline -lncurses

# Libft
LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

# Exécutable
NAME = minishell

# Compilation complète
all: $(BUILD_DIR) $(LIBFT) $(NAME)

# Création du dossier build/ si nécessaire
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compilation de Libft
$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

# Compilation de Minishell
$(NAME): $(OBJFILES) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJFILES) $(LIBFT) $(LIBS)

# Compilation des fichiers .c en .o
# PS : 42 autorise totalement ce genre de règles car elles ne détectent pas les fichiers automatiquement, elles les transforment juste.
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -rf $(BUILD_DIR)
	@$(MAKE) -C $(LIBFT_PATH) clean

# Nettoyage complet (exécutable + objets)
fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_PATH) fclean

# Recompilation complète
re: fclean all
