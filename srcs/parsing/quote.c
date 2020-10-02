/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 14:49:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/10/01 11:01:54 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void		ft_init_quote(int *i, int *d_quote, int *s_quote)
{
	g_shell.quote[0] = 0;
	g_shell.i_quote = 0;
	g_shell.quote_pos[g_shell.i_quote++] = 0;
	*d_quote = 0;
	*s_quote = 0;
	*i = -1;
}

char		*ft_multiligne_quote(char *buf, int s_quote, int d_quote, int i)
{
	int		k;
	int		ret;

	g_shell.s_q = s_quote;
	g_shell.d_q = d_quote;
	while (g_shell.s_q % 2 != 0 || g_shell.d_q % 2 != 0)
	{
		g_shell.s_q > 0 ? ft_printf(1, "quote> ") : ft_printf(1, "dquote> ");
		ret = read(0, g_shell.buf, BUF_SIZE);
		g_shell.buf[ret - 1] = '\0';
		k = 0;
		while (g_shell.buf[k])
		{
			if (g_shell.buf[k] == g_shell.quote[0])
				g_shell.quote_pos[g_shell.i_quote++] = i + k;
			g_shell.s_q = g_shell.buf[k] == S_QUOTE && S_QUOTE == g_shell.quote[0] ?
								g_shell.s_q + 1 : g_shell.s_q;
			g_shell.d_q = g_shell.buf[k] == '"' && '"' == g_shell.quote[0] ?
														g_shell.d_q + 1 : g_shell.d_q;
			k++;
		}
		if (!(buf = ft_str_add(buf, g_shell.buf)))
			return (NULL);
	}
	ft_strdel(&buf);
	return (ft_strdup(g_shell.buf));
}

void		ft_check_empty(char *buf)
{
	int	i;
	int	l;
	int	check;

	i = -1;
	l = -1;
	if (!(g_shell.argv_empty = malloc(sizeof(char *) *
		(g_shell.i_quote / 2 + 1))))
		exit(-1000);
	l = 0;
	g_shell.argv_empty[g_shell.i_quote / 2] = NULL;
	while (buf[++i])
	{
		ft_cond_parse(buf, i);
		if (g_shell.pip != -1 || g_shell.save != -1)
			break ;
		check = 0;
		if (!ft_create_empty(buf, &i, &l, check))
			break ;
	}
	g_shell.argv_empty[l] = NULL;
}

void		ft_loop_quote(char *buf, int *i, int *s_quote, int *d_quote)
{
	while (buf[(*i += 1)] && g_shell.pip == -1 && g_shell.save == -1)
	{
		if (!g_shell.quote[0] && (buf[*i] == S_QUOTE || buf[*i] == '"'))
			g_shell.quote[0] = buf[*i];
		if (g_shell.quote[0] && buf[*i] == g_shell.quote[0])
			g_shell.quote_pos[g_shell.i_quote++] = *i;
		if (buf[*i] == S_QUOTE && S_QUOTE == g_shell.quote[0])
			*s_quote += 1;
		if (buf[*i] == '"' && '"' == g_shell.quote[0])
			*d_quote += 1;
		if (buf[*i] == ';' && *s_quote % 2 == 0 && *d_quote % 2 == 0)
			g_shell.save = *i;
		if (buf[*i] == '|' && *s_quote % 2 == 0 && *d_quote % 2 == 0)
			g_shell.pip = *i;
	}
	buf[*i - 1] = g_shell.pip != -1 || g_shell.save != -1 ? '\0' : buf[*i - 1];
	g_shell.tmp = ft_strdup(buf);
	ft_check_empty(buf);
}

char		*ft_check_quote(char *buf)
{
	int		i;
	int		d_quote;
	int		s_quote;
	char	*save;

	ft_init_quote(&i, &d_quote, &s_quote);
	save = ft_strdup(buf);
	ft_loop_quote(buf, &i, &s_quote, &d_quote);
//	buf = g_shell.quote[0] ? ft_str_del_char(buf, g_shell.quote[0]) : buf;
	i = ft_strlen(buf);
	if (g_shell.pip != -1 || g_shell.save != -1)
	{
		g_shell.save_buf = ft_str_add(buf, &save[g_shell.pip != -1 ?
			g_shell.pip : g_shell.save]);
		g_shell.save = g_shell.save != -1 ? i : -1;
		g_shell.pip = g_shell.pip != -1 ? i : -1;
		buf = ft_strdup(g_shell.tmp);
	}
//	buf = g_shell.quote[0] ? ft_str_del_char(buf, g_shell.quote[0]) : buf;
	ft_strdel(&g_shell.tmp);
	ft_strdel(&save);
	g_shell.quote_pos[g_shell.i_quote] = -1;
	return (buf);
}
