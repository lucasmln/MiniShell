/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 14:49:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/25 12:20:37 by jvaquer          ###   ########.fr       */
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

	while (s_quote % 2 != 0 || d_quote % 2 != 0)
	{
		s_quote > 0 ? ft_printf(1, "quote> ") : ft_printf(1, "dquote> ");
		ret = read(0, g_shell.buf, BUF_SIZE);
		g_shell.buf[ret] = '\0';
		k = 0;
		while (g_shell.buf[k])
		{
			if (g_shell.buf[k] == g_shell.quote[0])
				g_shell.quote_pos[g_shell.i_quote++] = i + k;
			s_quote = g_shell.buf[k] == S_QUOTE && S_QUOTE == g_shell.quote[0] ?
								s_quote + 1 : s_quote;
			d_quote = g_shell.buf[k] == '"' && '"' == g_shell.quote[0] ?
														d_quote + 1 : d_quote;
			k++;
		}
		if (!(buf = ft_str_add(buf, g_shell.buf)))
			return (NULL);
	}
	return (buf);
}

int			ft_create_empty(char *buf, int *i, int *l, int check)
{
	int k;

	if (buf[*i] == g_shell.quote[0])
	{
		k = 1;
		while (buf[*i + k] == ' ')
			k++;
		check = buf[*i + k] == g_shell.quote[0] ? 1 : 0;
		*i += k;
		if (check == 0 && buf[*i])
			while (buf[*i] && buf[*i] != g_shell.quote[0])
				*i += 1;
	}
	if (check)
	{
		if (!(g_shell.argv_empty[*l] = malloc(sizeof(char) * (k))))
			exit(-1000);
		g_shell.argv_empty[*l][k - 1] = '\0';
		check = 0;
		while (k - 1 > check)
			g_shell.argv_empty[*l][check++] = ' ';
		*l += 1;
		*i += 1;
	}
	return (!buf[*i] ? 0 : 1);
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

char		*ft_check_quote(char *buf)
{
	int		i;
	int		d_quote;
	int		s_quote;
	char	*save;

	ft_init_quote(&i, &d_quote, &s_quote);
	save = ft_strdup(buf);
	while (buf[++i] && g_shell.pip == -1 && g_shell.save == -1)
	{
		if (!g_shell.quote[0] && (buf[i] == S_QUOTE || buf[i] == '"'))
			g_shell.quote[0] = buf[i];
		if (g_shell.quote[0] && buf[i] == g_shell.quote[0])
			g_shell.quote_pos[g_shell.i_quote++] = i;
		if (buf[i] == S_QUOTE && S_QUOTE == g_shell.quote[0])
			s_quote++;
		if (buf[i] == '"' && '"' == g_shell.quote[0])
			d_quote++;
		if (buf[i] == ';' && s_quote % 2 == 0 && d_quote % 2 == 0)
			g_shell.save = i;
		if (buf[i] == '|' && s_quote % 2 == 0 && d_quote % 2 == 0)
			g_shell.pip = i;
	}
	buf[i - 1] = g_shell.pip != -1 || g_shell.save != -1 ? '\0' : buf[i - 1];
	g_shell.tmp = ft_strdup(buf);
	ft_check_empty(buf);
	if (g_shell.quote[0])
		buf = ft_str_del_char(buf, g_shell.quote[0]);
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
	if (s_quote % 2 != 0 || d_quote % 2 != 0)
		buf = ft_multiligne_quote(buf, s_quote, d_quote, i);
	g_shell.quote_pos[g_shell.i_quote] = -1;
	return (buf);
}
