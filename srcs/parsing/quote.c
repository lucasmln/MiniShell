/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 14:49:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/10/04 17:49:11 by lmoulin          ###   ########.fr       */
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
	i = ft_strlen(buf);
	if (g_shell.pip != -1 || g_shell.save != -1)
	{
		g_shell.save_buf = ft_str_add(buf, &save[g_shell.pip != -1 ?
			g_shell.pip : g_shell.save]);
		g_shell.save = g_shell.save != -1 ? i : -1;
		g_shell.pip = g_shell.pip != -1 ? i : -1;
		buf = ft_strdup(g_shell.tmp);
	}
	ft_strdel(&g_shell.tmp);
	ft_strdel(&save);
	g_shell.quote_pos[g_shell.i_quote] = -1;
	return (buf);
}
