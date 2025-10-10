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
          src/utils/ft_itoa.c \
          src/utils/ft_atoi_strict.c \
          src/utils/ident.c \
          src/builtins/builtins.c \
          src/builtins/builtin_pwd.c \
          src/builtins/builtin_echo.c \
          src/builtins/builtin_cd.c \
          src/builtins/builtin_env.c \
          src/builtins/builtin_export.c \
          src/builtins/builtin_unset.c \
          src/builtins/builtin_exit.c \
          src/builtins/builtin_status.c \
          src/parse/pipeline_cmds.c \
          src/parse/pipeline_free.c \
          src/parse/pipeline_parse.c \
          src/parse/lexer_simple.c \
          src/parse/lexer_build.c \
          src/parse/redir_parse.c \
          src/parse/utils/token_utils.c \
          src/exec/child_run.c \
          src/exec/path_join.c \
          src/exec/path_find.c \
          src/exec/exec_resolve.c \
          src/exec/exec_core.c \
          src/exec/exec_errors.c \
          src/exec/pipe_run.c \
          src/exec/redir_runtime.c \
          src/signals/signals.c \
          src/env/env_free.c \
          src/env/env_utils.c \
          src/env/env_init.c \
          src/env/env_get.c \
          src/env/env_set.c \
          src/env/env_unset.c \
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
