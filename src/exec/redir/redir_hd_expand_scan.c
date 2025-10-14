/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_hd_expand_scan.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mananton <telesmanuel@hotmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 13:37:18 by mananton          #+#    #+#             */
/*   Updated: 2025/10/14 13:37:18 by mananton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redir_internal.h"

int	hd_scan_single(const char *line, t_hd_scan *scan)
{
	if (scan->in_double || line[scan->i] != '\'')
		return (0);
	scan->in_single = !scan->in_single;
	scan->total++;
	scan->i++;
	return (1);
}

int	hd_scan_double(const char *line, t_hd_scan *scan)
{
	if (scan->in_single || line[scan->i] != '"')
		return (0);
	scan->in_double = !scan->in_double;
	scan->total++;
	scan->i++;
	return (1);
}

int	hd_scan_dollar(const char *line, t_hd_scan *scan, t_hd_ctx *ctx)
{
	size_t	consumed;

	if (line[scan->i] != '$' || scan->in_single)
		return (0);
	consumed = 0;
	scan->total += hd_dollar_len(line + scan->i, ctx, &consumed);
	scan->i += consumed;
	return (1);
}

size_t	hd_total_len(const char *line, t_hd_ctx *ctx)
{
	t_hd_scan	scan;

	scan.i = 0;
	scan.total = 0;
	scan.in_single = 0;
	scan.in_double = 0;
	while (line[scan.i] && (!ctx->err || *(ctx->err) == 0))
	{
		if (hd_scan_single(line, &scan))
			continue ;
		if (hd_scan_double(line, &scan))
			continue ;
		if (hd_scan_dollar(line, &scan, ctx))
			continue ;
		scan.total++;
		scan.i++;
	}
	return (scan.total);
}
