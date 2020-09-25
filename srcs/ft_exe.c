/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:59:04 by jvaquer           #+#    #+#             */
/*   Updated: 2020/09/25 13:00:28 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			ft_ispipe_is_ptvirgule(void)
{
	char	*tmp;

	tmp = g_shell.save != -1 ? ft_strdup(&g_shell.save_buf[g_shell.save + 1]) :
								ft_strdup(&g_shell.save_buf[g_shell.pip + 1]);
	free(g_shell.save_buf);
	g_shell.save_buf = NULL;
	return (ft_check_parse(tmp));
}

int			ft_ex_2(t_exe ex)
{
	ex.argv = ft_split(ex.buf, ' ');
	ex.argv = ft_check_input(ex.argv, ex.in);
	if (ft_strncmp(ex.buf, "/bin/echo", 9))
		ex.argv = ft_add_empty(ex.argv);
	if (g_shell.save_pipfd[0] > 0 || g_shell.nb_input > 0)
		ft_add_input(ex.in, ex.fd);
	ex.i = -1;
	while (1)
	{
		ex = ft_loop_exe(ex);
		if (!ex.cmd_path || !ex.try_path || ex.save == -1)
			break ;
	}
	g_shell.ret = ex.save == -1 ? ex.status / 256 : 0;
	if (ex.save != -1)
	{
		g_shell.ret = 127;
		g_shell.save_pipfd[0] = 0;
		ft_printf(1, "minishell: command not found: %s\n", ex.buf);
	}
	ex = ft_free_exe(ex);
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (1);
}

int			ft_exe(char *buf)
{
	t_exe			ex;

	ex.buf = ft_strdup(buf);
	ex = ft_set_fd_path(ex);
	if (!ex.in)
		return (0);
	ex.i = 0;
	while (ex.buf[ex.i] && ex.buf[ex.i] != ' ')
		ex.i++;
	ex.save = ex.buf[ex.i];
	ex.buf[ex.i] = '\0';
	if (!(ft_strncmp(ex.buf, "./", ft_strlen("./"))))
	{
		ex.cmd = ft_strdup("./");
		ex.binary = ft_strdup(&ex.buf[2]);
	}
	else
		ex.cmd = ft_strdup(ex.buf);
	ex.buf[ex.i] = ex.save;
	ex.i = ft_ex_2(ex);
	return (ex.i);
}
