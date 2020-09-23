/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/23 13:29:29 by lmoulin          ###   ########.fr       */
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

char		*ft_dollars_2(int *i, char *new, char *buf, int save)
{
	int		start;
	int		pos;

	start = *i += 1;
	if (buf[*i] && (ft_isalnum(buf[*i]) || buf[*i] == '_'))
	{
		while (buf[*i] && (ft_isalnum(buf[*i]) || buf[*i] == '_'))
			*i += 1;
		save = buf[*i];
		buf[*i] = '\0';
		pos = ft_find_var(g_shell.sort_env, &buf[start], '=');
		new = ft_str_add(new, ft_strchr(g_shell.sort_env[pos], '=') ?
				ft_strchr(g_shell.sort_env[pos], '=') + 1 : "");
		buf[*i] = save;
	}
	else if (buf[*i] == '?')
	{
		g_shell.tmp = ft_itoa(g_shell.ret);
		new = ft_str_add(new, g_shell.tmp);
		free(g_shell.tmp);
		*i += 1;
	}
	else
		new = ft_str_add(new, "$");
	return (new);
}

char		*ft_dollars(char *buf)
{
	int		i;
	int		save;
	char	*new;

	i = 0;
	if (!(new = malloc(sizeof(char) * 1)))
		return (NULL);
	new[0] = '\0';
	while (buf[i])
	{
		if (buf[i] == '$')
			new = ft_dollars_2(&i, new, buf, save);
		else
		{
			save = buf[i + 1];
			buf[i + 1] = '\0';
			new = ft_str_add(new, &buf[i]);
			buf[i + 1] = save;
			i++;
		}
	}
	g_shell.tmp = NULL;
	free(buf);
	buf = NULL;
	return (new);
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

void		ft_free_exe(t_exe ex)
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
	free(ex.cmd);
	ex.cmd = NULL;
	free(ex.save_path);
	ex.save_path = NULL;
	if (ex.binary)
		free(ex.binary);
	ex.binary = NULL;
	free(ex.in);
	ex.in = 0;
	ft_strdel(&ex.buf);
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
	close(fd[1]);
	ft_close_fd(in);
	g_shell.nb_input = 1;
	in[0] = dup(fd[0]);
	close(fd[0]);
}

t_exe		ft_set_fd_path(t_exe ex)
{
	if (!(ex.in = malloc(sizeof(int) * (512))))
		return (ex);
	ex.in = ft_init_fd_tab(ex.in, 512);
	ex.binary = NULL;
	ex.i = 0;
	g_shell.fd = ft_check_redir(ft_strdup(ex.buf), g_shell.fd);
	if (g_shell.fd[0] != -2)
		ex.buf = ft_del_redir(ft_strdup(ex.buf));
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
		waitpid(ex.pid, &ex.status, 0);
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
		waitpid(ex.pid, &ex.status, 0);
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

int			ft_ex_2(t_exe ex)
{
	ex.argv = ft_split(ex.buf, ' ');
	ex.argv = ft_check_input(ex.argv, ex.in);
	g_shell.nb_input == 0 ? 0 : ft_add_input(ex.in, ex.fd);
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
		ft_printf(1, "minishell: command not found: %s\n", ex.buf);
	}
	ft_free_exe(ex);
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

int			ft_ispipe_is_ptvirgule(void)
{
	char	*tmp;

	tmp = g_shell.save != -1 ? ft_strdup(&g_shell.save_buf[g_shell.save + 1]) :
								ft_strdup(&g_shell.save_buf[g_shell.pip + 1]);
	free(g_shell.save_buf);
	g_shell.save_buf = NULL;
	return (ft_check_parse(tmp));
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
	if (new)
	{
		ft_strdel(&buf);
		*i = 0;
		return (new);
	}
	ft_strdel(&tmp);
	return (buf);
}

int			ft_get_cmd(char *buf)
{
	int		i;

	i = 0;
	g_shell.ret = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (!ft_strncmp(&buf[i], "pwd", 3) || !ft_strncmp(&buf[i], "echo", 4))
	{
		buf = ft_add_path(buf, &i);
	}
	if (!ft_strncmp(&buf[i], "cd", ft_strlen("cd")))
		g_shell.ret = ft_cd(&buf[i + 2]);
	else if (!ft_strncmp(&buf[i], "export ", ft_strlen("export")))
		g_shell.ret = ft_export(&buf[i + ft_strlen("export")]);
	else if (!(ft_strncmp(&buf[i], "unset", ft_strlen("unset"))))
		g_shell.ret = ft_unset(&buf[i + ft_strlen("unset")]);
	else if (!ft_strncmp(buf, "exit", ft_strlen("exit")))
		exit(g_shell.ret);
	else
		ft_exe(&buf[i]);
	free(buf);
	buf = NULL;
	return ((g_shell.ret));
}

char		*ft_set_check_parse(char *buf)
{
	g_shell.save = -1;
	g_shell.pip = -1;
	g_shell.fd = ft_init_fd_tab(g_shell.fd, 512);
	buf = ft_check_quote(buf);
	if (buf[ft_strlen(buf) - 1] == '\n')
		buf[ft_strlen(buf) - 1] = '\0';
	g_shell.i_quote = 0;
	g_shell.save_buf = ft_strdup(buf);
	return (buf);
}

void		ft_cond_parse(char *buf, int i)
{
	if (buf[i] == g_shell.quote[0] &&
								g_shell.quote_pos[g_shell.i_quote + 1] != -1)
		g_shell.i_quote += 2;
	if (buf[i] == ';' && i > g_shell.quote_pos[g_shell.i_quote] &&
									i < g_shell.quote_pos[g_shell.i_quote + 1])
		g_shell.save = i;
	else if (g_shell.quote_pos[g_shell.i_quote + 1] == -1 &&
						i > g_shell.quote_pos[g_shell.i_quote] && buf[i] == ';')
		g_shell.save = i;
	else if (g_shell.quote_pos[g_shell.i_quote + 1] == -1 &&
						i > g_shell.quote_pos[g_shell.i_quote] && buf[i] == '|')
		g_shell.pip = i;
	if (buf[i] == '|' && i > g_shell.quote_pos[g_shell.i_quote] &&
									i < g_shell.quote_pos[g_shell.i_quote + 1])
		g_shell.pip = '|';
}

int			ft_check_parse(char *buf)
{
	int		i;

	if (g_shell.save)
		free(g_shell.save_buf);
	g_shell.save_buf = NULL;
	i = -1;
	g_shell.fd = ft_close_fd(g_shell.fd);
	buf = ft_set_check_parse(buf);
	while (buf[++i] && g_shell.save == -1 && g_shell.pip == -1)
	{
		if (g_shell.quote[0])
			ft_cond_parse(buf, i);
		else
		{
			g_shell.save = buf[i] == ';' ? i : -1;
			g_shell.pip = buf[i] == '|' ? i : -1;
		}
	}
	g_shell.save != -1 ? buf[g_shell.save] = '\0' : 0;
	g_shell.pip != -1 ? buf[g_shell.pip] = '\0' : 0;
	buf = ft_dollars(buf);
	if (g_shell.quote[0])
		buf = ft_str_del_char(buf, g_shell.quote[0]);
	return (ft_get_cmd(buf));
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
	while (1)
	{
		ft_print_prompt();
		free(g_shell.save_buf);
		g_shell.save_buf = NULL;
	}
}
