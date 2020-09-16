/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/16 01:57:34 by lmoulin          ###   ########.fr       */
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

char		*ft_dollars(char *buf)
{
	int		i;
	int		start;
	int		save;
	int		pos;
	char	*new;

	i = 0;
	if (!(new = malloc(sizeof(char) * 1)))
		return (NULL);
	new[0] = '\0';
	while (buf[i])
	{
		if (buf[i] == '$')
		{
			i++;
			start = i;
			if (buf[i] && (ft_isalnum(buf[i]) || buf[i] == '_'))
			{
				while (buf[i] && (ft_isalnum(buf[i]) || buf[i] == '_'))
					i++;
				save = buf[i];
				buf[i] = '\0';
				pos = ft_find_var(g_shell.sort_env, &buf[start], '\0');
				new = ft_str_add(new, ft_strchr(g_shell.sort_env[pos], '=') ? ft_strchr(g_shell.sort_env[pos], '=') + 1 : "");
				buf[i] = save;
				i--;
			}
			else if (buf[i] == '?')
			{
				new = ft_str_add(new, (g_shell.tmp = ft_itoa(g_shell.ret)));
				free(g_shell.tmp);
			}
			else
			{
				new = ft_str_add(new, "$");
				i--;
			}
		}
		else
		{
			save = buf[i + 1];
			buf[i + 1] = '\0';
			new = ft_str_add(new, &buf[i]);
			buf[i + 1] = save;
		}
		i++;
	}
	g_shell.tmp = NULL;
	free(buf);
	buf = NULL;
	return (new);
}

int			ft_copy_env(const char **env)
{
	int		i;
	int		len;

	len = 0;
	while (env[len])
		len++;
	g_shell.len_env = len;
	g_shell.len_exp = len;
	if (!(g_shell.env = malloc(sizeof(char *) * (len + 1))))
		return (0);
	if (!(g_shell.sort_env = malloc(sizeof(char *) * (len + 1))))
		return (0);
	i = -1;
	while (env[++i])
	{
		if (!(ft_strncmp(env[i], "SHLVL=", ft_strlen("SHLVL="))))
		{
			g_shell.sort_env[i] = ft_str_add(ft_strdup("SHLVL="), (g_shell.tmp = ft_itoa(ft_atoi(&env[i][ft_strlen("SHLVL=")]) + 1)));
			g_shell.env[i] = ft_strdup(g_shell.sort_env[i]);
			free(g_shell.tmp);
			g_shell.tmp = NULL;
		}
		else
		{
			g_shell.sort_env[i] = ft_strdup(env[i]);
			g_shell.env[i] = ft_strdup(env[i]);
		}
	}
	g_shell.env[i] = NULL;
	g_shell.sort_env[i] = NULL;
	ft_sort_env(g_shell.sort_env);
	return (1);
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

void		ft_free_exe(char **argv, char *cmd, char *save_path, char *binary)
{
	int		i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	free(argv);
	argv = NULL;
	free(cmd);
	cmd = NULL;
	free(save_path);
	save_path = NULL;
	if (binary)
		free(binary);
	binary = NULL;
}

char		*ft_del_redir(char *buf)
{
	char	*new;
	int		i;
	int		k;

	i = 0;
	k = 0;
	if (!(new = malloc(sizeof(char) * ft_strlen(buf) + 1)))
		return (NULL);
	while (buf[i])
		if (buf[i] == '>')
		{
			i++;
			if (buf[i] == '>')
				i++;
			while (buf[i] == ' ')
				i++;
			while (buf[i] && buf[i] != ' ')
				i++;
			while (buf[i] == ' ')
				i++;
		}
		else
			new[k++] = buf[i++];
	new[k] = '\0';
	k = -1;
	while (new[++k])
		if (new[k] == ';')
		{
			new[k] = '\0';
			break ;
		}
	free(buf);
	buf = NULL;
	return (new);
}

char		**ft_check_input(char **argv)
{
	int		i;
	int		fd;
	char	*tmp;

	i = 0;
	tmp = NULL;
	while (argv[i])
	{
		if (argv[i][0] == '<')
		{
			if (ft_strlen(argv[i]) == 1)
			{
				if ((fd = open(argv[i + 1], O_RDONLY)) < 0)
					return (NULL);
				free(argv[i]);
				argv[i] = argv[i + 1];
				argv[i + 1] = NULL;
				i--;
			}
		}
		i++;
	}
	return (argv);
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

int			ft_exe(char *buf)
{
	int				pos;
	int				save;
	int				i;
	int				k;
	int				status;
	char			*path;
	char			*save_path;
	char			*cmd;
	char			*binary;
	char			*try_path;
	char			*cmd_path;
//	char			*tmp;
	char			**argv;
	struct stat		info;
	pid_t			pid;

	pos = 0;
	binary = NULL;
	i = 0;
	g_shell.fd = ft_check_redir(ft_strdup(buf), g_shell.fd, 1);
	if (g_shell.fd[0] != -2)
		buf = ft_del_redir(ft_strdup(buf));
	g_shell.fd[0] = g_shell.fd[0] == -2 ? 1 : g_shell.fd[0];
	while (g_shell.sort_env[pos] && ft_strncmp(g_shell.sort_env[pos], "PATH=", 5))
		pos++;
	path = !g_shell.sort_env[pos] ? NULL : ft_strdup(&g_shell.sort_env[pos][5]);
	save_path = path;
	while (buf[i] && buf[i] != ' ')
		i++;
	save = buf[i];
	buf[i] = '\0';
	if (!(ft_strncmp(buf, "./", ft_strlen("./"))))
	{
		cmd = ft_strdup("./");
		binary = ft_strdup(&buf[2]);
	}
	else
		cmd = ft_strdup(buf);
	buf[i] = save;
	argv = ft_split(buf, ' ');
	argv = ft_check_input(argv);
	i = -1;
	while (1)
	{
		try_path = ft_get_path(path);
		if (cmd[0] == '/' || !try_path)
			cmd_path = ft_strdup(cmd);
		else
		{
			if (!(cmd_path = malloc(sizeof(char) * (ft_strlen(cmd) + ft_strlen(try_path) + 2))))
				return (-1);
			i = -1;
			while (try_path[++i])
				cmd_path[i] = try_path[i];
			cmd_path[i++] = '/';
			k = -1;
			while (cmd[++k])
				cmd_path[i + k] = cmd[k];
			cmd_path[i + k] = '\0';
		}
		ft_create_pipe();
		if (!stat(cmd_path, &info))
		{
			i = 0;
			while (g_shell.pip == -1 && (i < g_shell.nb_fd || (g_shell.nb_fd == 0 && i == 0)))
			{
				pid = fork();
				if (pid == 0)
				{
					dup2(g_shell.fd[i], STDOUT_FILENO);
					if (g_shell.save_pipfd[0])
						dup2(g_shell.save_pipfd[0], STDIN_FILENO);
					if (binary && !stat(binary, &info))
						exit((execve(binary, argv, g_shell.env)));
					else
						exit(execve(cmd_path, argv, g_shell.env));
				}
				else
				{
					if (g_shell.pipe_fd[0])
					{
						close(g_shell.pipe_fd[0]);
						g_shell.pipe_fd[0] = 0;
					}
					waitpid(pid, &status, 0);
					save = -1;
					if (binary && stat(binary, &info))
						save = 0;
				}
				i++;
			}
			if (g_shell.pip != -1)
			{
				pid = fork();
				if (pid == 0)
				{
					close(g_shell.pipe_fd[0]);
					dup2(g_shell.pipe_fd[1], STDOUT_FILENO);
					if (g_shell.save_pipfd[0])
						dup2(g_shell.save_pipfd[0], STDIN_FILENO);
					if (binary && !stat(binary, &info))
						exit((execve(binary, argv, g_shell.env)));
					else
						exit(execve(cmd_path, argv, g_shell.env));
				}
				else
				{
					waitpid(pid, &status, 0);
					save = -1;
					close(g_shell.pipe_fd[1]);
					g_shell.pipe_fd[1] = 0;
					if (g_shell.save_pipfd[0])
						close(g_shell.save_pipfd[0]);
					g_shell.save_pipfd[0] = g_shell.pipe_fd[0];
				}
			}
		}
		free(cmd_path);
		cmd_path = NULL;
		if (!try_path)
			break ;
		free(try_path);
		try_path = NULL;
		if (save == -1)
			break ;
		i = 0;
		while (path[i] && path[i] != ':')
			i++;
		path = &path[i + 1];
	}
	ft_free_exe(argv, cmd, save_path, binary);
	g_shell.ret = save == -1 ? status / 256 : 0;
	if (save != -1)
		ft_printf(1, "minishell: command not found: %s\n", buf);
	free(buf);
	buf = NULL;
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (1);
}

int			ft_ispipe_is_ptvirgule()
{
	char	*tmp;

	tmp = g_shell.save != -1 ? ft_strdup(&g_shell.save_buf[g_shell.save + 1]):
								ft_strdup(&g_shell.save_buf[g_shell.pip + 1]);;
	free(g_shell.save_buf);
	g_shell.save_buf = NULL;
	return (ft_check_parse(tmp));
}

int			ft_get_cmd(char *buf)
{
	int		i;

	i = 0;
	g_shell.ret = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (!ft_strncmp(&buf[i], "cd", ft_strlen("cd")))
		g_shell.ret = ft_cd(&buf[i + 2]);
//	else if (!ft_strncmp(&buf[i], "pwd", ft_strlen("pwd")))
//		g_shell.ret = ft_exe(&buf[i]);//ft_pwd(&buf[i + ft_strlen("pwd")]);
	else if (!ft_strncmp(&buf[i], "export ", ft_strlen("export")))
		g_shell.ret = ft_export(&buf[i + ft_strlen("export")]);
/*	else if (!ft_strncmp(&buf[i], "env", ft_strlen("env")))
**		g_shell.ret = ft_exe(&buf[i]);//  ft_env(&buf[i + ft_strlen("env")], g_shell.env);
**	else if (!ft_strncmp(&buf[i], "echo", ft_strlen("echo")))
**		g_shell.ret = ft_exe(&buf[i]);//  ft_echo(&buf[i + ft_strlen("echo")]);
*/	else if (!(ft_strncmp(&buf[i], "unset", ft_strlen("unset"))))
		g_shell.ret = ft_unset(&buf[i + ft_strlen("unset")]);
	else if (!ft_strncmp(buf, "exit", ft_strlen("exit")))
		exit(g_shell.ret);
	else if (!ft_exe(&buf[i]))
		;
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

	i = -1;
	if (g_shell.save_buf)
	{
		free(g_shell.save_buf);
		g_shell.save_buf = NULL;
	}
	buf = ft_set_check_parse(buf);
	while (buf[++i] && g_shell.save == -1 && g_shell.pip == -1)
	{
		if (g_shell.quote[0])
			ft_cond_parse(buf, i);
		else
		{
			if (buf[i] == ';')
				g_shell.save = i;
			else if (buf[i] == '|')
				g_shell.pip = i;
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
	char	*tmp;

	g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
	i = ft_strlen(g_shell.dir);
	tmp = NULL;
	buf = NULL;
	while (i >= 0 && g_shell.dir[i] != '/')
		i--;
	ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET,
														&g_shell.dir[i + 1]);
	ret = read(0, g_shell.buf, BUF_SIZE);
	if (g_shell.buf[ret - 1] == 0)
		exit(0);
	g_shell.buf[ret] = '\0';
	buf = ft_str_add(buf, g_shell.buf);
	g_shell.save_pipfd[0] = 0;
	return (ft_check_parse(buf));
}

int			*ft_init_fd_tab(int *tab, int len)
{
	int		i;

	i = 0;
	tab[i] = 1;
	while (++i < len)
		tab[i] = -2;
	return (tab);
}

int			*ft_close_fd(int *fd)
{
	int		i;

	i = 0;
	while (i < 512)
	{
		if (fd[i] > 1)
			close(fd[i]);
		fd[i] = -2;
		i++;
	}
	fd = ft_init_fd_tab(fd, 512);
	return (fd);
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
