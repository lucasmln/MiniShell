/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:38:22 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:38:24 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int			ft_sort_env(char **env)
{
	int		i;
	char	*tmp;

	(void)env;
	i = -1;
	while (g_shell.sort_env[++i + 1])
	{
		if (ft_strcmp(g_shell.sort_env[i], g_shell.sort_env[i + 1]) > 0)
		{
			tmp = g_shell.sort_env[i + 1];
			g_shell.sort_env[i + 1] = g_shell.sort_env[i];
			g_shell.sort_env[i] = tmp;
			i = i - 1 >= -1 ? -1 : i - 1;
		}
	}
	return (1);
}

int			ft_export_no_pipe(void)
{
	int		i;
	int		k;

	k = -1;
	while (g_shell.sort_env[++k])
	{
		i = 0;
		while (i < g_shell.nb_fd || (g_shell.nb_fd == 0 && i == 0))
		{
			write(g_shell.fd[i], g_shell.sort_env[k],
										ft_strlen(g_shell.sort_env[k]));
			write(g_shell.fd[i++], "\n", 1);
		}
	}
	return (0);
}

void		ft_export_pipe_2(void)
{
	int		i;
	int		k;

	k = -1;
	close(g_shell.pipe_fd[0]);
	dup2(g_shell.pipe_fd[1], STDOUT_FILENO);
	if (g_shell.save_pipfd[0])
		dup2(g_shell.save_pipfd[0], STDIN_FILENO);
	while (g_shell.sort_env[++k])
	{
		i = 0;
		while (i <= g_shell.nb_fd || (g_shell.nb_fd == 0 && i == 0))
		{
			write(g_shell.fd[i], g_shell.sort_env[k],
											ft_strlen(g_shell.sort_env[k]));
			write(g_shell.fd[i++], "\n", 1);
		}
	}
	exit(0);
}

int			ft_export_pipe(void)
{
	pid_t	pid;
	int		i;

	g_shell.fd[g_shell.nb_fd] = g_shell.pipe_fd[1];
	pid = fork();
	if (pid == 0)
		ft_export_pipe_2();
	else
	{
		waitpid(pid, &i, 0);
		close(g_shell.pipe_fd[1]);
		g_shell.pipe_fd[1] = 0;
		if (g_shell.save_pipfd[0])
			close(g_shell.save_pipfd[0]);
		g_shell.save_pipfd[0] = g_shell.pipe_fd[0];
	}
	return (0);
}
