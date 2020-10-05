/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 15:22:35 by jvaquer           #+#    #+#             */
/*   Updated: 2020/10/01 10:40:23 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int				ft_check_end_exe(t_exe ex)
{
	free(ex.cmd_path);
	ex.cmd_path = NULL;
	if (!ex.try_path)
		return (0);
	free(ex.try_path);
	ex.try_path = NULL;
	if (ex.save == -1)
		return (0);
	return (1);
}

t_exe			ft_free_exe(t_exe ex)
{
	int			i;

	i = 0;
	while (ex.argv[i])
	{
		free(ex.argv[i]);
		ex.argv[i] = NULL;
		i++;
	}
	free(ex.argv);
	ex.argv = NULL;
	ft_strdel(&ex.cmd);
	free(ex.save_path);
	ex.save_path = NULL;
	if (ex.binary)
		free(ex.binary);
	ex.binary = NULL;
	ft_strdel(&ex.buf);
	free(g_shell.argv_empty);
	g_shell.argv_empty = NULL;
	free(ex.in);
	ex.in = 0;
	return (ex);
}

t_exe			ft_dup_sortie(t_exe ex)
{
	if (ex.in[0] > 0 && g_shell.save_pipfd[0] > 0)
	{
		ex.in[1] = dup(g_shell.save_pipfd[0]);
		g_shell.nb_input++;
		ft_add_input(ex.in, ex.fd);
		close(g_shell.save_pipfd[0]);
		g_shell.save_pipfd[0] = -2;
	}
	if (g_shell.fd[ex.i] > 0)
		dup2(g_shell.fd[ex.i], STDOUT_FILENO);
	if (g_shell.save_pipfd[0] > 0)
		dup2(g_shell.save_pipfd[0], STDIN_FILENO);
	if (ex.in[0] > 0)
		dup2(ex.in[0], STDIN_FILENO);
	return (ex);
}

char			**ft_add_empty(char **av)
{
	int			i;
	int			k;
	char		**new;

	k = 0;
	if ((i = 0) != 0 || !g_shell.argv_empty[0])
		return (av);
	while (av[k])
		k++;
	while (g_shell.argv_empty[i])
		i++;
	if (!(new = malloc(sizeof(char *) * (i + k + 1))))
		exit(1000);
	i = -1;
	while (av[++i])
		new[i] = ft_strdup(av[i]);
	k = -1;
	while (g_shell.argv_empty[++k])
		new[k + i] = ft_strdup(g_shell.argv_empty[k]);
	new[k + i] = NULL;
	ft_free_av(av);
	free(av);
	av = NULL;
	ft_free_empty();
	return (new);
}
