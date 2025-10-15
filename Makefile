# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/25 09:44:25 by mananton          #+#    #+#              #
#    Updated: 2025/10/06 13:59:45 by mananton         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
INCS    = -Iinclude

SRC     = src/main.c \
          src/utils/io.c \
          src/utils/args.c \
          src/utils/args_split.c \
          src/utils/ft_itoa.c \
          src/utils/ft_atoi_strict.c \
          src/utils/glob_expand.c \
          src/utils/glob_expand_collect.c \
          src/utils/glob_expand_match.c \
          src/utils/glob_expand_utils.c \
          src/utils/ident.c \
          src/shell/shell_context.c \
          src/shell/shell_input.c \
          src/shell/shell_pipeline.c \
          src/shell/shell_exec.c \
          src/shell/shell_loop.c \
          src/builtins/builtins.c \
          src/builtins/builtin_pwd.c \
          src/builtins/builtin_echo.c \
          src/builtins/builtin_cd.c \
          src/builtins/builtin_env.c \
          src/builtins/export_common.c \
          src/builtins/export_print.c \
          src/builtins/export_actions.c \
          src/builtins/builtin_export.c \
          src/builtins/builtin_unset.c \
          src/builtins/builtin_exit.c \
          src/builtins/builtin_status.c \
          src/parse/pipeline_cmds.c \
          src/parse/pipeline_free.c \
          src/parse/pipeline_parse.c \
          src/parse/pipeline_split.c \
          src/parse/lexer/lexer_base.c \
          src/parse/lexer/lexer_names.c \
          src/parse/lexer/lexer_dollar.c \
          src/parse/lexer/lexer_len.c \
          src/parse/lexer/lexer_build_helpers.c \
          src/parse/lexer/lexer_token_prepare.c \
          src/parse/lexer/lexer_build_main.c \
          src/parse/lexer/lexer_build_tokens.c \
          src/parse/lexer/lexer_simple_token.c \
          src/parse/lexer/lexer_simple_utils.c \
          src/parse/lexer/lexer_split_args.c \
          src/parse/redir/redir_utils.c \
          src/parse/redir/redir_build_argv.c \
          src/parse/redir/redir_extract.c \
          src/parse/redir/redir_main.c \
          src/parse/utils/token_utils.c \
          src/exec/child/child_pipe.c \
          src/exec/child/child_io.c \
          src/exec/child/child_exec.c \
          src/exec/path_join.c \
          src/exec/path_find.c \
          src/exec/exec_resolve.c \
          src/exec/exec_core_child.c \
          src/exec/exec_core_external.c \
          src/exec/exec_errors.c \
          src/exec/pipe/pipe_context.c \
          src/exec/pipe/pipe_child.c \
          src/exec/pipe/pipe_finish.c \
          src/exec/pipe/pipe_run_main.c \
          src/exec/redir/redir_hd_signal.c \
          src/exec/redir/redir_hd_session.c \
          src/exec/redir/redir_hd_expand_utils.c \
          src/exec/redir/redir_hd_expand_value.c \
          src/exec/redir/redir_hd_expand_scan.c \
          src/exec/redir/redir_hd_expand_emit.c \
          src/exec/redir/redir_hd_expand_main.c \
          src/exec/redir/redir_hd_state.c \
          src/exec/redir/redir_fd_utils.c \
          src/exec/redir/redir_open.c \
          src/exec/redir/redir_parent.c \
          src/exec/redir/redir_parent_apply.c \
          src/signals/signals.c \
          src/env/env_free.c \
          src/env/env_utils.c \
          src/env/env_init.c \
          src/env/env_get.c \
          src/env/env_set.c \
          src/env/env_unset.c \
          src/env/env_marks.c \
          src/env/env_shlvl.c
                        

OBJ     = $(SRC:.c=.o)
LIBS    = -lreadline

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
