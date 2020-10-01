/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:54:22 by jvaquer           #+#    #+#             */
/*   Updated: 2020/10/01 11:41:40 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char		*ft_set_check_parse(char *buf)
{
	char		*tmp;
	int			i;
	int			count;

	g_shell.save = -1;
	g_shell.pip = -1;
	g_shell.fd = ft_init_fd_tab(g_shell.fd, 512);
	if (buf[ft_strlen(buf) - 1] == '\n')
		buf[ft_strlen(buf) - 1] = '\0';
	tmp = ft_strdup(buf);
	buf = ft_check_quote(buf);
	g_shell.i_quote = 0;
	i = 0;
	while (buf[i] == ' ')
		i++;
	if (!ft_strncmp(&buf[i], "echo", 4))
	{
		i = -1;
		g_shell.pip = -1;
		g_shell.save = -1;
		count = 0;
		while (tmp[++i])
		{
			if (tmp[i] == g_shell.quote[0])
				count++;
			if (count % 2 == 0 && (tmp[i] == '|' || tmp[i] == ';'))
				break ;
		}
		g_shell.pip = tmp[i] == '|' ? i : -1;;
		g_shell.save = tmp[i] == ';' ? i : -1;;
		if (g_shell.pip != -1 || g_shell.save != -1)
		{
			tmp[i] = '\0';
			g_shell.save_buf = ft_str_add(ft_strdup(tmp), &tmp[g_shell.pip != -1 ?
				g_shell.pip + 1: g_shell.save + 1]);
			g_shell.save = g_shell.save != -1 ? i : -1;
			g_shell.pip = g_shell.pip != -1 ? i : -1;
		}
		ft_strdel(&buf);
		return (tmp);
	}
	return (buf);
}

void		ft_cond_parse(char *buf, int i)
{
	if (g_shell.quote_pos[g_shell.i_quote + 1] != -1 &&
		i == g_shell.quote_pos[g_shell.i_quote])
		g_shell.i_quote += 2;
	if (buf[i] == ';' && i > g_shell.quote_pos[g_shell.i_quote - 1] &&
		i < g_shell.quote_pos[g_shell.i_quote])
		g_shell.save = i;
	else if (g_shell.quote_pos[g_shell.i_quote] == -1 &&
			i > g_shell.quote_pos[g_shell.i_quote - 1] && buf[i] == ';')
		g_shell.save = i;
	else if (buf[i] == '|' && i > g_shell.quote_pos[g_shell.i_quote - 1] &&
			i < g_shell.quote_pos[g_shell.i_quote])
		g_shell.pip = i;
	else if (g_shell.quote_pos[g_shell.i_quote] == -1 &&
			i > g_shell.quote_pos[g_shell.i_quote - 1] && buf[i] == '|')
		g_shell.pip = i;
}

int			ft_check_parse(char *buf)
{
	int		i;

	if (g_shell.save)
		free(g_shell.save_buf);
	g_shell.save_buf = NULL;
	g_shell.fd = ft_close_fd(g_shell.fd);
	buf = ft_set_check_parse(buf);
	i = 0;
	while (buf[i] == ' ')
		i++;
	buf = ft_dollars(buf);
	return (ft_get_cmd(buf));
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
