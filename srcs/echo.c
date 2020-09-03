/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:25 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/03 15:03:54 by lmoulin          ###   ########.fr       */
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
	int		ret;
	int		d_quote;
	int		s_quote;
	char	*new;

	i = 0;
	g_shell.quote[0] = 0;
	d_quote = 0;
	s_quote = 0;
	while (buf[i])
	{
		if (!g_shell.quote[0] && (buf[i] == S_QUOTE || buf[i] == '"'))
			g_shell.quote[0] = buf[i];
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
			i = 0;
			while (g_shell.buf[i])
			{
				s_quote = g_shell.buf[i] == S_QUOTE && S_QUOTE == g_shell.quote[0] ? s_quote + 1: s_quote;
				d_quote = g_shell.buf[i] == '"' && '"' == g_shell.quote[0] ? d_quote + 1: d_quote;
				i++;
			}
			if (!(new = ft_str_add(new, g_shell.buf)))
				return (NULL);
		}
	}
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

int		ft_double_redir(char *buf, int fd, int i, int stop)
{
	int				k;
	int				save[2];
	int				start;
	int				check;
	char			file_buf[2048];

	check = 0;
	while (buf[++i] == ' ')
		;
	if (!buf[i])
		return (ft_printf(1, "minishell: no redirecton after >>\n") - 30);
	start = i;
	while (buf[i] && buf[i] != ' ')
		i++;
	save[0] = buf[i];
	buf[i] = '\0';
	if ((fd = open(&buf[start], O_CREAT | O_RDWR , S_IRUSR | S_IROTH | S_IRGRP | S_IWUSR)) == -1)
		return (-1);
	while ((k = read(fd, file_buf, BUF_SIZE)))
		;
	k = -1;
	while (++k <= stop && stop > 0 )
		write(fd, &buf[k], 1);
	if (!save[0])
		return (write(fd, "\n", 1) ? fd : fd);
	while (k < start)
		k++;
	buf[i] = save[0];
	if (!stop)
		while (buf[++i] == ' ')
			;
	write(fd, &buf[i], ft_strlen(&buf[i]));
	write(fd, "\n", 1);
	return (fd);
}

int		ft_check_redir(char *buf, int fd, int cmd)
{
	int				i;
	int				k;
	int				save;
	int				start[2];

	i = 0;
	start[0] = 0;
	start[1] = 0;
	while (buf[i])
	{
		if (buf[i] != '>' && buf[i] != ' ')
			start[0] = i;
		if (buf[i] == '>' && buf[i + 1] != '>')
		{
			while (buf[++i] == ' ')
				;
			start[1] = i;
			if (!buf[i])
			{
				ft_printf(1, "minishell: parse error after >\n");
				fd = -1;
				break;
			}
			while (buf[i] && buf[i] != ' ')
				i++;
			save = buf[i];
			buf[i] = '\0';
			if ((fd = open(&buf[start[1]], O_TRUNC | O_CREAT | O_RDWR, S_IRUSR | S_IROTH | S_IRGRP | S_IWUSR, 0640)) == -1)
				break;
			if (cmd)
				break;
			k = -1;
			while (++k <= start[0])
				if (buf[k] != '>')
					write(fd, &buf[k], 1) ? start[1] = -1 : 0;
			if (!save)
			{
				write(fd, "\n", 1);
				break ;
			}
			while (k < i)
				k++;
			buf[i] = save;
			while (buf[i] == ' ')
				i++;
			start[1] == -1 ? write(fd, " ", 1) : 0;
			write(fd, &buf[i], ft_strlen(&buf[i]));
			write(fd, "\n", 1);
		}
		else if (buf[i] == '>' && buf[i + 1] == '>')
			fd = ft_double_redir(buf, fd, ++i, start[0]);
		i++;
	}
	free(buf);
	buf = NULL;
	return (fd);
}

int		ft_echo(char *buf)
{
	int		i;
	int		flag;
	int		save;
	int		fd;
	char	*tmp;

	i = 0;
	save = -1;
	fd = 0;
	while (buf[i])
		i++;
	if (i > 0 && buf[i - 1] == '\n')
		buf[i - 1] = '\0';
	i = 0;
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
	while (buf[i] == ' ')
		i++;
	g_shell.output = ft_strdup(buf);
	i = -1;
	while (g_shell.output[++i] && save == -1)
		if (g_shell.output[i] == ';')
			save = i;
	save != -1 ? g_shell.output[save] = '\0' : 0;
	fd = ft_check_redir(ft_strdup(g_shell.output), fd, 0);
	fd == 0 ? !flag ? ft_printf(1, "%s\n", g_shell.output) : ft_printf(1, "%s", g_shell.output) : 0;
	if (save != -1)
	{
		g_shell.output[save] = ';';
		tmp = ft_strdup(&g_shell.output[save + 1]);
	}
	free(g_shell.output);
	g_shell.output = NULL;
	if (save != -1)
		return (ft_get_cmd(tmp));
	return (1);
}
