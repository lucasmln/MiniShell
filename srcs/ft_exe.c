/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exe.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:59:04 by jvaquer           #+#    #+#             */
/*   Updated: 2020/10/01 12:14:54 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Pour exemple : echo hey > 1 > 2 > 3
** Afin d'ecire uniquement dans le dernier fichier,
** Remplacer par ex.i = g_shell.nb_fd - 1 (l?)
** Et remplacer le while qui suit par if (g_shell.pip == -1) (l?)
*/

t_exe		ft_loop_exe(t_exe ex)
{
	ex.try_path = ft_get_path(ex.path);
	if (ex.cmd[0] == '/' || !ex.try_path)
		ex.cmd_path = ft_strdup(ex.cmd);
	else
		ex = ft_create_cmdpath(ex);
	if (!ex.cmd_path)
		return (ex);
	ft_create_pipe();
	if (!stat(ex.cmd_path, &ex.info))
	{
		ex.i = 0;
		while (g_shell.pip == -1 && (ex.i < g_shell.nb_fd ||
					(g_shell.nb_fd == 0 && ex.i == 0)))
			ex = ft_exe_no_pipe(ex);
		if (g_shell.pip != -1)
			ex = ft_exe_pipe(ex);
	}
	if (!ft_check_end_exe(ex))
		return (ex);
	ex.i = 0;
	while (ex.path[ex.i] && ex.path[ex.i] != ':')
		ex.i++;
	ex.path = &ex.path[ex.i + 1];
	return (ex);
}

int			ft_ispipe_is_ptvirgule(void)
{
	char	*tmp;

	tmp = g_shell.save != -1 ? ft_strdup(&g_shell.save_buf[g_shell.save + 1]) :
								ft_strdup(&g_shell.save_buf[g_shell.pip + 1]);
	free(g_shell.save_buf);
	g_shell.save_buf = NULL;
	return (ft_check_parse(tmp));
}

char		*ft_add_quote(char *s)
{
	char		*new;

	if (!(new = malloc(sizeof(char) * 2)))
		exit(-1000);
	new[0] = 39;
	new[1] = '\0';
	s = ft_str_add(s, new);
	new = ft_str_add(new, s);
	return (new);
}

char		**ft_add_empty_quote(char **av)
{
	int			i;
	int			k;
	int			check;

	i = -1;
	while (av[++i])
	{
		k = -1;
		check = 0;
		while (av[i][++k])
		{
			if (av[i][k] != ' ')
				check = 1;
		}
		if (!check)
			ft_add_quote(av[i]);
	}
	return(av);
}

char		**ft_clean_av(char **av, int pos)
{
	int		i;
	int		k;
	char	**new;

	i = 0;
	while (av[i])
		i++;
	if (!(new = malloc(sizeof(char *) * (i + 1))))
		exit(-1000);
	i = 0;
	k = 0;
	while (av[i])
	{
		if (i + 1 == pos && i > 0)
		{
			ft_printf(1, "hey, av = %s\n", av[i]);
			new[k] = ft_strdup(av[i]);
			new[k] = ft_str_add(new[k], av[++i]);
			k++;
			i++;
		}
		else
			new[k++] = ft_strdup(av[i++]);
	}
	new[k] = NULL;
	int j = -1;
	while (new[++j])
		ft_printf(1, "new[%d]= %s, len = %d\n", j, new[j], ft_strlen(new[j]));
	return (new);
}

int			ft_ex_2(t_exe ex)
{
	int i;
//	int	k;

	ex.argv = ft_split(ex.buf, ' ');
	ex.argv = ft_check_input(ex.argv, ex.in);
	ex.argv = ft_add_empty(ex.argv);
	if (!ft_strncmp(ex.buf, "/bin/echo", 9))
	{
		ex.buf = ft_str_del_char(ex.buf, g_shell.quote[0]);
		ft_printf(1, "buf = %s\n", ex.buf);
		i = -1;
		while (ex.argv[++i])
			ft_printf(1, "|%s|\n", ex.argv[i]);
		i = -1;
		while (ex.argv[++i])
			ex.argv[i] = ft_str_del_char(ex.argv[i], g_shell.quote[0]);
/*		while (ex.argv[i])
		{
			k = 0;
			while (ex.argv[i][k])
			{
				if (ex.argv[i][k] != ' ')
					break;
				k++;
			}
			ft_printf(1, "k = %d\n", k);
			if (!ex.argv[i][k])
				ex.argv = ft_clean_av(ex.argv, i);
			i++;
		}
*/	}
	else
		ex.argv = ft_add_empty_quote(ex.argv);
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
