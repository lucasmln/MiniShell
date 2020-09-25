/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 15:14:36 by jvaquer           #+#    #+#             */
/*   Updated: 2020/09/25 15:28:56 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_create_pipe(void)
{
	if (g_shell.pip != -1)
	{
		if (pipe(g_shell.pipe_fd) == -1)
			exit(ft_printf(1, "minishell: pipe: error call function\n"));
		if (!g_shell.save_pipfd[0])
			g_shell.save_pipfd[0] = dup(g_shell.pipe_fd[0]);
	}
}

char		*ft_get_path(char *path)
{
	int		i;
	int		save;
	char	*new;

	i = 0;
	if (!path)
		return (NULL);
	if (ft_strlen(path) == 0)
		return (NULL);
	while (path[i] && path[i] != ':')
		i++;
	save = path[i];
	path[i] = '\0';
	new = ft_strdup(path);
	path[i] = save;
	return (new);
}

t_exe		ft_create_cmdpath(t_exe ex)
{
	if (!(ex.cmd_path = malloc(sizeof(char) * (ft_strlen(ex.cmd) +
												ft_strlen(ex.try_path) + 2))))
		return (ex);
	ex.i = -1;
	while (ex.try_path[++ex.i])
		ex.cmd_path[ex.i] = ex.try_path[ex.i];
	ex.cmd_path[ex.i++] = '/';
	ex.k = -1;
	while (ex.cmd[++ex.k])
		ex.cmd_path[ex.i + ex.k] = ex.cmd[ex.k];
	ex.cmd_path[ex.i + ex.k] = '\0';
	return (ex);
}

t_exe		ft_exe_pipe(t_exe ex)
{
	ex.pid = fork();
	if (ex.pid == 0)
	{
		close(g_shell.pipe_fd[0]);
		dup2(g_shell.pipe_fd[1], STDOUT_FILENO);
		if (g_shell.save_pipfd[0])
			dup2(g_shell.save_pipfd[0], STDIN_FILENO);
		if (ex.binary && !stat(ex.binary, &ex.info))
			exit((execve(ex.binary, ex.argv, g_shell.env)));
		else
			exit(execve(ex.cmd_path, ex.argv, g_shell.env));
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(ex.pid, &ex.status, 0);
		signal(SIGINT, SIG_IGN);
		ex.save = -1;
		close(g_shell.pipe_fd[1]);
		g_shell.pipe_fd[1] = 0;
		if (g_shell.save_pipfd[0])
			close(g_shell.save_pipfd[0]);
		g_shell.save_pipfd[0] = g_shell.pipe_fd[0];
	}
	return (ex);
}

t_exe		ft_exe_no_pipe(t_exe ex)
{
	ex.pid = fork();
	if (ex.pid == 0)
	{
		ex = ft_dup_sortie(ex);
		if (ex.binary && !stat(ex.binary, &ex.info))
			exit((execve(ex.binary, ex.argv, g_shell.env)));
		else
			exit(execve(ex.cmd_path, ex.argv, g_shell.env));
	}
	else
	{
		if (g_shell.pipe_fd[0])
		{
			close(g_shell.pipe_fd[0]);
			g_shell.pipe_fd[0] = 0;
		}
		signal(SIGINT, SIG_IGN);
		waitpid(ex.pid, &ex.status, 0);
		signal(SIGINT, ft_get_signal);
		ex.save = -1;
		if (ex.binary && stat(ex.binary, &ex.info))
			ex.save = 0;
	}
	ex.i++;
	return (ex);
}
