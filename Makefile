# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/25 09:44:25 by mananton          #+#    #+#              #
#    Updated: 2025/09/29 11:20:07 by mananton         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    = minishell
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
INCS    = -Iinclude

SRC     = src/main.c \
          src/utils/io.c \
          src/utils/args.c \
          src/builtins/builtins.c \
          src/builtins/builtin_pwd.c \
          src/builtins/builtin_echo.c \
          src/builtins/builtin_cd.c \
          src/builtins/builtin_env.c \
          src/builtins/builtin_export.c \
          src/builtins/builtin_unset.c \
          src/env/env_free.c \
          src/env/env_utils.c \
          src/env/env_init.c \
          src/env/env_get.c \
          src/env/env_set.c \
          src/env/env_unset.c
                        

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
