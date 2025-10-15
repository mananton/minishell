/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 15:45:12 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 13:30:57 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "env/env.h"
# include "parse/lexer.h"
# include <stddef.h>

typedef struct s_expand_ctx
{
	const char		*str;
	size_t			end;
	t_env			*env;
	const char		*status;
	size_t			status_len;
}					t_expand_ctx;

typedef struct s_quote_state
{
	int				in_single;
	int				in_double;
}					t_quote_state;

typedef struct s_emit
{
	char			*out;
	size_t			*pos;
}					t_emit;

typedef struct s_len_state
{
	size_t			index;
	size_t			count;
	t_quote_state	quotes;
}					t_len_state;

typedef struct s_copy_state
{
	size_t			index;
	t_quote_state	quotes;
	int				inhibit_glob;
}					t_copy_state;

typedef struct s_build_ctx
{
	const char		*line;
	size_t			count;
	t_env			*env;
	char			**argv;
	char			*status;
	size_t			status_len;
	size_t			index;
	size_t			arg_index;
}					t_build_ctx;

typedef struct s_token_build
{
	t_expand_ctx	ctx;
	t_copy_state	copy;
	t_emit			emit;
	size_t			out_pos;
}					t_token_build;

typedef struct s_len_runner
{
	t_expand_ctx	ctx;
	t_len_state		state;
	int				*err;
	int				*quoted_any;
}					t_len_runner;

int					is_blank(char c);
int					is_dquote_escapable(char c);
int					is_var_start(char c);
int					is_var_char(char c);
size_t				var_name_len(const char *s, size_t pos, size_t end);
char				*dup_var_name(const char *s, size_t len);
size_t				var_value_len(t_env *env, const char *s, size_t len,
						int *err);
int					copy_var_value(t_emit *emit, t_env *env, const char *s,
						size_t len);
void				copy_status(char *out, size_t *j, const char *status);

size_t				dollar_literal_len(size_t *index);
int					dollar_literal_copy(const char *s, size_t *index,
						t_emit *emit);
size_t				process_dollar_len(t_expand_ctx *ctx, size_t *index,
						int in_single, int *err);
int					process_dollar_copy(t_expand_ctx *ctx, size_t *index,
						t_emit *emit, int in_single);

int					handle_quote_char(char c, t_quote_state *quotes,
						int *quoted);
void				len_escape(const t_expand_ctx *ctx, t_len_state *state);
void				len_runner_init(t_len_runner *runner,
						const t_expand_ctx *base, size_t start);
void				len_process_char(t_len_runner *runner);
size_t				len_runner_execute(t_len_runner *runner);

void				copy_escape(const t_expand_ctx *ctx, t_copy_state *state,
						t_emit *emit);
void				init_copy_state(t_copy_state *state, size_t start);
unsigned int		token_flags(int quoted_any, int inhibit_glob);
int					token_build_prepare(t_token_build *build,
						const t_expand_ctx *base, size_t start,
						int *quoted_any);
int					build_token_body(t_expand_ctx *ctx, t_copy_state *state,
						t_emit *emit);
char				*build_token(const t_expand_ctx *base, size_t start,
						int *quoted_any);

size_t				skip_blanks(const char *s, size_t i);
int					init_build_ctx(t_build_ctx *ctx, const char *line,
						size_t count, t_env *env);
void				compact_tokens(char **argv);
void				init_expand_ctx(t_expand_ctx *expand,
						const t_build_ctx *ctx, size_t end);
int					mark_token_error(t_build_ctx *ctx);
int					append_token(t_build_ctx *ctx);
int					build_tokens(t_build_ctx *ctx);
char				**split_build_argv(const char *line, size_t count,
						t_env *env);

int					skip_blanks_simple(const char *line);
int					skip_token_simple(const char *line, int index);
int					count_tokens(const char *line);
size_t				token_len(const char *s, size_t start, size_t *end);
int					split_count_tokens(const char *line, size_t *out_count);

#endif
