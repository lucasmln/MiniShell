/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/25 12:59:59 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_get_signal(int code)
{
	int			i;

	if (code == SIGINT)
	{
		g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
		i = ft_strlen(g_shell.dir);
		while (i >= 0 && g_shell.dir[i] != '/')
			i--;
		ft_printf(1, "\n" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET,
														&g_shell.dir[i + 1]);
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

t_exe		ft_free_exe(t_exe ex)
{
	int		i;

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
	free(ex.in);
	ex.in = 0;
	ft_strdel(&ex.buf);
	free(g_shell.argv_empty);
	g_shell.argv_empty = NULL;
	return (ex);
}

int			ft_check_end_exe(t_exe ex)
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

void		ft_add_input(int *in, int *fd)
{
	pid_t	pid;
	int		k;
	char	buf[BUF_SIZE + 1];
	int		ret;

	pipe(fd);
	pid = fork();
	k = -1;
	if (pid == 0)
	{
		close(fd[0]);
		while (++k < g_shell.nb_input)
			while ((ret = read(in[k], buf, BUF_SIZE)))
			{
				buf[ret] = '\0';
				write(fd[1], buf, ret);
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
		ex.buf = ft_del_redir(ex.buf);
	g_shell.fd[0] = g_shell.fd[0] == -2 ? 1 : g_shell.fd[0];
	while (g_shell.sort_env[ex.i] &&
								ft_strncmp(g_shell.sort_env[ex.i], "PATH=", 5))
		ex.i++;
	ex.path = !g_shell.sort_env[ex.i] ? NULL :
										ft_strdup(&g_shell.sort_env[ex.i][5]);
	ex.save_path = ex.path;
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

t_exe		ft_dup_sortie(t_exe ex)
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

/*
 ** pour exemple : echo hey > 1 > 2 > 3
 ** afin d'ecire uniquement dans le dernier fichier,
 ** remplacer par ex.i = g_shell.nb_fd - 1 (l?)
 ** et remplacer le while aui suit par if (g_shell.pip == -1) (l?)
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

void			ft_free_av(char **av)
{
	int	i;

	i = -1;
	while (av[++i])
	{
		free(av[i]);
		av[i] = NULL;
	}
}

char			**ft_add_empty(char **av)
{
	int	i;
	int	k;
	char	**new;

	k = 0;
	i = 0;
	if (!g_shell.argv_empty[0])
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
	ft_free_av(g_shell.argv_empty);
	free(g_shell.argv_empty);
	g_shell.argv_empty = NULL;
	return (new);
}

char		*ft_add_path(char *buf, int *i)
{
	char	*new;
	char	*tmp;

	new = NULL;
	tmp = ft_strdup("/bin/");
	if (!ft_strncmp(&buf[*i], "pwd ", 4) || (!ft_strncmp(&buf[*i], "pwd", 3) && ft_strlen(&buf[*i]) == 3))
		new = ft_str_add(tmp, &buf[*i]);
	else if (!ft_strncmp(&buf[*i], "echo ", 5))
		new = ft_str_add(tmp, &buf[*i]);
	else if (!ft_strncmp(&buf[*i], "env ", 5) | (!ft_strncmp(&buf[*i], "env", 3) && ft_strlen(&buf[*i]) == 3))
		new = ft_str_add(ft_strdup("/usr/bin/"), &buf[*i]);
	if (new)
	{
		ft_strdel(&buf);
		*i = 0;
		return (new);
	}
	ft_strdel(&tmp);
	return (buf);
}

int			ft_check_exit(char *buf)
{
	int		i;

	if (!ft_strncmp(buf, "exit", 4))
	{
		i = 4;
		if (!buf[i])
			return (1);
		while (buf[i])
		{
			if (buf[i] != ' ')
				return (0);
			i++;
		}
		return (1);
	}
	return (0);
}

int			ft_get_cmd(char *buf)
{
	int		i;

	i = 0;
	g_shell.ret = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (!ft_strncmp(&buf[i], "pwd", 3) || !ft_strncmp(&buf[i], "echo", 4) || !ft_strncmp(&buf[i], "env", 3))
		buf = ft_add_path(buf, &i);
	if (!ft_strncmp(&buf[i], "cd", ft_strlen("cd")))
		g_shell.ret = ft_cd(&buf[i + 2]);
	else if (!ft_strncmp(&buf[i], "export ", ft_strlen("export")))
		g_shell.ret = ft_export(&buf[i + ft_strlen("export")]);
	else if (!(ft_strncmp(&buf[i], "unset", ft_strlen("unset"))))
		g_shell.ret = ft_unset(&buf[i + ft_strlen("unset")]);
	else if (ft_check_exit(&buf[i]))
		exit(g_shell.ret);
	else
		ft_exe(&buf[i]);
	free(buf);
	buf = NULL;
	return ((g_shell.ret));
}

int			ft_print_prompt(void)
{
	int		i;
	int		ret;
	char	*buf;

	g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
	i = ft_strlen(g_shell.dir);
	buf = NULL;
	while (i >= 0 && g_shell.dir[i] != '/')
		i--;
	ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET,
														&g_shell.dir[i + 1]);
	ret = read(0, g_shell.buf, BUF_SIZE);
	if (g_shell.buf[ret - 1] == 0)
		exit(0);
	g_shell.buf[ret] = '\0';
	i = 0;
	while (g_shell.buf[i] == ' ')
		i++;
	buf = ft_str_add(buf, &g_shell.buf[i]);
	g_shell.save_pipfd[0] = 0;
	return (ft_check_parse(buf));
}

int			main(int ac, char **av, const char **env)
{
	(void)ac;
	(void)av;
	if (!(ft_copy_env(env)))
		return (-1);
	if (!(g_shell.fd = malloc(sizeof(int) * 512)))
		return (-1);
	g_shell.fd = ft_init_fd_tab(g_shell.fd, 512);
	signal(SIGINT, ft_get_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		ft_print_prompt();
		free(g_shell.save_buf);
		g_shell.save_buf = NULL;
	}
}
