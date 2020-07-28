/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:25 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/28 11:54:52 by lmoulin          ###   ########.fr       */
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
		return (NULL);
	if (!s2)
		return (s1);
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
	return (new);
}

void	ft_check_quote(char *buf, char *type_quote)
{
	int		i;
	int		ret;
	int		d_quote;
	int		s_quote;

	i = 0;
	type_quote[0] = 0;
	d_quote = 0;
	s_quote = 0;
	while (buf[i])
	{
		if (!type_quote[0] && (buf[i] == S_QUOTE || buf[i] == '"'))
			type_quote[0] = buf[i];
		s_quote = buf[i] == S_QUOTE && S_QUOTE == type_quote[0] ? s_quote + 1: s_quote;
		d_quote = buf[i] == '"' && '"' == type_quote[0] ? d_quote + 1: d_quote;
		i++;
	}
	if (!(g_shell.output = ft_strdup(buf)))
		return ;
	buf[0] = '\0';
	if (s_quote % 2 != 0 || d_quote % 2 != 0)
	{
		while (s_quote % 2 != 0 || d_quote % 2 != 0)
		{
			s_quote > 0 ? ft_printf(1, "quote> ") : ft_printf(1, "dquote> ");
			ret = read(0, buf, BUF_SIZE);
			buf[ret] = '\0';
			i = 0;
			while (buf[i])
			{
				s_quote = buf[i] == S_QUOTE && S_QUOTE == type_quote[0] ? s_quote + 1: s_quote;
				d_quote = buf[i] == '"' && '"' == type_quote[0] ? d_quote + 1: d_quote;
				i++;
			}
			if (!(g_shell.output = ft_str_add(g_shell.output, buf)))
				return ;
		}
	}
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

void	ft_echo(char *buf)
{
	int		i;
	int		flag;
	int		save;
	char	type_quote[1];
	char	*tmp;

	i = 0;
	save = -1;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (i == 0)
	{
		ft_printf(1, "minishell: command not found %s", g_shell.buf);
		return ;
	}
	flag = ft_strncmp(&buf[i], "-n ", 3) == 0 ? 1 : 0;
	flag == 1 ? ft_strlcpy(buf, &buf[i + 3], ft_strlen(&buf[i + 3])) :
		ft_strlcpy(buf, &buf[i], ft_strlen(&buf[i]) + 1);
	while (buf[i] == ' ')
		i++;
	ft_check_quote(buf, type_quote);
	if (type_quote[0] == S_QUOTE)
		g_shell.output = ft_str_del_char(g_shell.output, S_QUOTE);
	if (type_quote[0] == '"')
		g_shell.output = ft_str_del_char(g_shell.output, '"');
	i = -1;
	while (g_shell.output[++i] && save == -1)
		if (g_shell.output[i] == ';')
			save = i;
	save != -1 ? g_shell.output[save] = '\0' : 0;
	save != -1 && !flag ? ft_printf(1, "%s\n", g_shell.output) : ft_printf(1, "%s", g_shell.output);
	if (save != -1)
	{
		g_shell.output[save] = ';';
		while (g_shell.output[++save] == ' ')
			;
		tmp = ft_strdup(&g_shell.output[save]);
		free(g_shell.output);
		g_shell.output = tmp;
		ft_str_add(g_shell.output, "\n");
		ft_get_cmd(g_shell.output);
	}
}
