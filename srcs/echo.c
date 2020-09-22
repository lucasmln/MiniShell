/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:25 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 11:59:47 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char		*ft_create_new(char *new, char *s1, char const *s2)
{
	int		len1;
	int		len2;

	len1 = -1;
	len2 = -1;
	while (s1[++len1])
		new[len1] = s1[len1];
	while (s2[++len2])
		new[len1 + len2] = s2[len2];
	new[len1 + len2] = '\0';
	return (new);
}

char		*ft_str_add(char *s1, char const *s2)
{
	char	*new;
	int		len1;
	int		len2;

	len1 = 0;
	len2 = 0;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
	{
		new = ft_strdup(s1);
		ft_strdel(&s1);
		return (new);
	}
	while (s1[len1])
		len1++;
	while (s2[len2])
		len2++;
	if (!(new = malloc(sizeof(char) * (len1 + len2 + 1))))
		return (NULL);
	new = ft_create_new(new, s1, s2);
	ft_strdel(&s1);
	s1 = new;
	return (s1);
}

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

char		*ft_str_del_char(char *str, char c)
{
	int		i;
	int		k;

	i = 0;
	k = 0;
	while (str[i])
	{
		if (str[i] == c)
		{
			k = i;
			while (str[k] && str[k] == c)
				k++;
			str[i] = str[k];
			if (str[k] != '\0')
				str[k] = c;
		}
		i++;
	}
	return (str);
}
