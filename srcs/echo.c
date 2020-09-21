/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:25 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/21 18:46:32 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		free(s1);
		s1 = NULL;
		return (new);
	}
	while (s1[len1])
		len1++;
	while (s2[len2])
		len2++;
	if (!(new = malloc(sizeof(char) * (len1 + len2 + 1))))
		return (NULL);
	len1 = -1;
	len2 = 0;
	while (s1[++len1])
		new[len1] = s1[len1];
	while (s2[len2])
	{
		new[len1 + len2] = s2[len2];
		len2++;
	}
	new[len1 + len2] = '\0';
	free(s1);
	s1 = NULL;
	s1 = new;
	return (s1);
}

char	*ft_check_quote(char *buf)
{
	int		i;
	int		k;
	int		ret;
	int		d_quote;
	int		s_quote;
	char	*new;

	i = 0;
	g_shell.quote[0] = 0;
	g_shell.i_quote = 0;
	g_shell.quote_pos[g_shell.i_quote++] = 0;
	d_quote = 0;
	s_quote = 0;
	while (buf[i])
	{
		if (!g_shell.quote[0] && (buf[i] == S_QUOTE || buf[i] == '"'))
			g_shell.quote[0] = buf[i];
		if (g_shell.quote[0] && buf[i] == g_shell.quote[0])
			g_shell.quote_pos[g_shell.i_quote++] = i;
		s_quote = buf[i] == S_QUOTE && S_QUOTE == g_shell.quote[0] ? s_quote + 1: s_quote;
		d_quote = buf[i] == '"' && '"' == g_shell.quote[0] ? d_quote + 1: d_quote;
		i++;
	}
	if (!(new = ft_strdup(buf)))
		return (NULL);
	free(buf);
	buf = NULL;
	if (s_quote % 2 != 0 || d_quote % 2 != 0)
	{
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
				s_quote = g_shell.buf[k] == S_QUOTE && S_QUOTE == g_shell.quote[0] ? s_quote + 1: s_quote;
				d_quote = g_shell.buf[k] == '"' && '"' == g_shell.quote[0] ? d_quote + 1: d_quote;
				k++;
			}
			if (!(new = ft_str_add(new, g_shell.buf)))
				return (NULL);
		}
	}
	g_shell.quote_pos[g_shell.i_quote] = -1;
	return (new);
}

char	*ft_str_del_char(char *str, char c)
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

int		ft_echo(char *buf)
{
	int		i;
	int		flag;
	int		k;
	int		save;
	char	*tmp;

	i = 0;
	k = 0;
	save = -1;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (i == 0)
	{
		g_shell.ret = 127;
		ft_printf(1, "minishell: command not found %s", buf);
		return (2);
	}
	flag = ft_strncmp(&buf[i], "-n ", 3) == 0 ? 1 : 0;
	flag == 1 ? ft_strlcpy(buf, &buf[i + 3], ft_strlen(&buf[i + 3]) + 1) :
		ft_strlcpy(buf, &buf[i], ft_strlen(&buf[i]) + 1);
	g_shell.output = ft_strdup(buf);
	g_shell.fd = ft_check_redir(ft_strdup(g_shell.output), g_shell.fd, 0);
	g_shell.output = ft_del_redir(g_shell.output);
	i = 0;
	while (i <= g_shell.nb_fd)
	{
		ft_printf(g_shell.fd[i], "%s", g_shell.output);
		if (!flag)
			write(g_shell.fd[i], "\n", 1);
		i++;
	}
	g_shell.fd = ft_close_fd(g_shell.fd);
	if (g_shell.save != -1)
	{
		tmp = ft_strdup(&g_shell.save_buf[g_shell.save + 1]);
		free(g_shell.save_buf);
		g_shell.save_buf = NULL;
	}
	free(g_shell.output);
	g_shell.output = NULL;
	if (g_shell.save != -1)
		return (ft_check_parse(tmp));
	return (1);
}
