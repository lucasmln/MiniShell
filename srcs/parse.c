/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:54:22 by jvaquer           #+#    #+#             */
/*   Updated: 2020/09/25 15:59:50 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char		*ft_set_check_parse(char *buf)
{
	g_shell.save = -1;
	g_shell.pip = -1;
	g_shell.fd = ft_init_fd_tab(g_shell.fd, 512);
	if (buf[ft_strlen(buf) - 1] == '\n')
		buf[ft_strlen(buf) - 1] = '\0';
	buf = ft_check_quote(buf);
	g_shell.i_quote = 0;
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
	if (!ft_strncmp(&buf[i], "echo", 4) && g_shell.quote[0])
		buf = ft_str_del_char(buf, g_shell.quote[0]);
	buf = ft_dollars(buf);
	return (ft_get_cmd(buf));
}
