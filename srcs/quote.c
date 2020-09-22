/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 14:49:46 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:10:46 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char		*ft_check_quote(char *buf)
{
	int		i;
	int		d_quote;
	int		s_quote;

	ft_init_quote(&i, &d_quote, &s_quote);
	while (buf[++i])
	{
		if (!g_shell.quote[0] && (buf[i] == S_QUOTE || buf[i] == '"'))
			g_shell.quote[0] = buf[i];
		if (g_shell.quote[0] && buf[i] == g_shell.quote[0])
			g_shell.quote_pos[g_shell.i_quote++] = i;
		if (buf[i] == S_QUOTE && S_QUOTE == g_shell.quote[0])
			s_quote++;
		if (buf[i] == '"' && '"' == g_shell.quote[0])
			d_quote++;
	}
	if (s_quote % 2 != 0 || d_quote % 2 != 0)
		buf = ft_multiligne_quote(buf, s_quote, d_quote, i);
	g_shell.quote_pos[g_shell.i_quote] = -1;
	return (buf);
}
