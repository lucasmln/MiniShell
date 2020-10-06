/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 15:38:19 by jvaquer           #+#    #+#             */
/*   Updated: 2020/10/06 10:52:54 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_add_input(int *in, int *fd)
{
	pid_t	pid;
	int		k;
	char	buf[BUF_SIZE + 1];
	int		ret;

	pipe(fd);
	pid = fork();
	if ((k = -1) == -1 && pid == 0)
	{
		close(fd[0]);
		while (++k < g_shell.nb_input)
		{
			while ((ret = read(in[k], buf, BUF_SIZE)))
			{
				buf[ret] = '\0';
				write(fd[1], buf, ret);
			}
			close(in[k]);
		}
		close(fd[1]);
		exit(0);
	}
	waitpid(pid, &k, 0);
	close(fd[1]);
	ft_close_fd(in);
	g_shell.nb_input = 1;
	in[0] = dup(fd[0]);
	close(fd[0]);
}

t_exe		ft_set_fd_path(t_exe ex)
{
	if (!(ex.in = malloc(sizeof(int) * (512))))
		exit(-1000);
	ex.in = ft_init_fd_tab(ex.in, 512);
	ex.binary = NULL;
	ex.i = 0;
	g_shell.fd = ft_check_redir(ft_strdup(ex.buf), g_shell.fd);
	if (g_shell.fd[0] != -2)
	{
		ex.buf = ft_del_redir(ex.buf);
	}
	ex.argv = ft_del_redir_av(ex.argv);
	int		k = -1;
	while (ex.argv[++k])
		ft_printf(1, "av = %s\n", ex.argv[k]);
	g_shell.fd[0] = g_shell.fd[0] == -2 ? 1 : g_shell.fd[0];
	while (g_shell.sort_env[ex.i] &&
								ft_strncmp(g_shell.sort_env[ex.i], "PATH=", 5))
		ex.i++;
	ex.path = !g_shell.sort_env[ex.i] ? NULL :
										ft_strdup(&g_shell.sort_env[ex.i][5]);
	ex.save_path = ex.path;
	return (ex);
}
