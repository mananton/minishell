/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_meta_struct.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:22:37 by mananton          #+#    #+#             */
/*   Updated: 2025/10/15 14:22:39 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_META_STRUCT_H
# define TOKEN_META_STRUCT_H

typedef struct s_token_meta
{
	char				*str;
	unsigned int		flags;
	struct s_token_meta	*next;
}						t_token_meta;

#endif
