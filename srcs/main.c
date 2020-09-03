/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/03 13:29:23 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_get_signal(int code)
{
	if (code == SIGINT)
	{
		ft_printf(1, "\n");
		ft_print_prompt();
	}//Pas d'exit, relancer une affiche de prompt
	if (code == SIGQUIT)
		ft_printf(1, "exit: signal code %d\n", code);
	exit(code);
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
		g_shell.sort_env[i] = ft_strdup(env[i]);
		g_shell.env[i] = ft_strdup(env[i]);
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

char		*ft_get_path_cmd(char *buf)
{
	int		i;
	char	*s1;
	char	*s2;

	i = 0;
	s2 = NULL;
	s1 = NULL;
	while (buf[i])
	{
		if (buf[i] == '>')
		{
			buf[i] = '\0';
			if (!(ft_strncmp("./", buf, 2)))
				s1 = ft_strdup("./");
			else
				s1 = ft_strdup(buf);
			buf[i] = '>';
			if (buf[++i] == '>')
				i++;
			while (buf[i] == ' ')
				i++;
			while (buf[i] && buf[i] != ' ')
				i++;
			while (buf[i] == ' ')
				i++;
			if (buf[i])
				s2 = ft_strdup(&buf[i]);
		}
		i++;
	}
	if (!s1)
		s1 = ft_strdup(buf);
	return (ft_str_add(s1, s2));
}

int			ft_exe(char *buf)
{
	int				pos;
	int				save[2];
	int				i;
	int				fd;
	int				k;
	int				status;
	char			*path;
	char			*save_path;
	char			*cmd;
	char			*binary;
	char			*try_path;
	char			*cmd_path;
	char			*tmp;
	char			**argv;
	struct stat		info;
	pid_t			pid;

	i = -1;
	pos = 0;
	save[1] = -1;
	binary = NULL;
	fd = 1;
	while (buf[++i])
		if (buf[i] == ';')
			save[1] = i;
	fd = ft_check_redir(ft_strdup(buf), fd, 0);
	i = 0;
	if (save[1] != -1)
		buf[save[1]] = '\0';
	while (ft_strncmp(g_shell.sort_env[pos], "PATH=", 5))
		pos++;
	path = ft_strdup(&g_shell.sort_env[pos][5]);
	save_path = path;
	while (buf[i] && buf[i] != ' ')
		i++;
	cmd = ft_get_path_cmd(buf);
	if (!(ft_strncmp(buf, "./", ft_strlen("./"))))
		binary = ft_strdup(&cmd[2]);
	ft_printf(1, "binary = %s, cmd  = %s\n\n", binary, cmd);
/*	save[0] = buf[i];
	buf[i] = '\0';
	if (!(ft_strncmp(buf, "./", ft_strlen("./"))))
	{
		cmd = ft_strdup("./");
		binary = ft_strdup(&buf[2]);
	}
	else
		cmd = ft_strdup(buf);
	buf[i] = save[0];
*/	argv = ft_split(buf, ' ');
	while ((try_path = ft_get_path(path)) != NULL)
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
		if (!stat(cmd_path, &info))
		{
			pid = fork();
			if (pid == 0)
			{
				dup2(fd, STDOUT_FILENO);
				dup2(fd, STDERR_FILENO);
				if (binary && !stat(binary, &info))
				{
					exit((execve(binary, argv, g_shell.env)));
				}
				else
					exit(execve(cmd_path, argv, g_shell.env));
			
			}
			else
			{
				waitpid(pid, &status, 0);
				save[0] = -1;
				if (binary && stat(binary, &info))
					save[0] = 0;
			}
		}
		free(try_path);
		try_path = NULL;
		free(cmd_path);
		cmd_path = NULL;
		if (save[0] == -1)
			break ;
		i = 0;
		while (path[i] && path[i] != ':')
			i++;
			path = &path[i + 1];
	}
	ft_free_exe(argv, cmd, save_path, binary);
	if (save[1] != -1)
	{
		tmp = ft_strdup(&buf[save[1] + 1]);
		buf[save[1]] = ';';
		ft_get_cmd(tmp);
	}
	return (save[0] == -1 ? 1 : -1);
}

int			ft_get_cmd(char *buf)
{
	int		i;
	int		res;

	i = 0;
	g_shell.ret = 0;
	res = 1;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (!ft_strncmp(&buf[i], "cd", ft_strlen("cd")))
		res = ft_cd(&buf[i + 2]);
	else if (!ft_strncmp(&buf[i], "pwd", ft_strlen("pwd")))
		res = ft_pwd(&buf[i + ft_strlen("pwd")]);
	else if (!ft_strncmp(&buf[i], "export ", ft_strlen("export")))
		res = ft_export(&buf[i + ft_strlen("export")]);
	else if (!ft_strncmp(&buf[i], "env", ft_strlen("env")))
		res = ft_env(&buf[i + ft_strlen("env")], g_shell.env);
	else if (!ft_strncmp(&buf[i], "echo", ft_strlen("echo")))
		res = ft_echo(&buf[i + ft_strlen("echo")]);
//	else if (!(ft_strncmp(&buf[i], "ls", ft_strlen("ls"))))
//		res = ft_ls(&buf[i + ft_strlen("ls")]);
	else if (!(ft_strncmp(&buf[i], "unset", ft_strlen("unset"))))
		res = ft_unset(&buf[i + ft_strlen("unset")]);
/*	else if (!ft_strncmp(&buf[i], "./", ft_strlen("./")))
	{
		res = ft_exe("a.out"&buf[i + ft_strlen("./")]);
		ft_printf(1, "after\n");
	}
*/	else if (!ft_strncmp(buf, "exit", ft_strlen("exit")))
		res = 0;
	else if ((res = ft_exe(&buf[i])) == -1 )
		ft_printf(1, "minishell: command not found %s\n", buf);
	free(buf);
	buf = NULL;
	return (res);
}

int			ft_print_prompt()
{
	int		i;
	int		ret;
	int		res;
	char	*buf;

	g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
	i = ft_strlen(g_shell.dir);
	while (i >= 0 && g_shell.dir[i] != '/')
		i--;
	ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET, &g_shell.dir[i + 1]);
	ret = read(0, g_shell.buf, BUF_SIZE);
	g_shell.buf[ret] = '\0';
	buf = ft_strdup(g_shell.buf);
	buf = ft_check_quote(buf);
	g_shell.tmp = ft_dollars(buf);
	free(buf);
	buf = g_shell.tmp;
	if (g_shell.quote[0] == S_QUOTE)
		buf = ft_str_del_char(buf, S_QUOTE);
	if (g_shell.quote[0] == '"')
		buf = ft_str_del_char(buf, '"');
	if (buf[ft_strlen(buf) - 1] == '\n')
		buf[ft_strlen(buf) - 1] = '\0';
	res = ft_get_cmd(buf);
	return (res);
}

int			main(int ac, char **av, const char **env)
{
	(void)ac;
	(void)av;
	if (!(ft_copy_env(env)))
		return (-1);
	signal(SIGINT, ft_get_signal);
	signal(SIGQUIT, ft_get_signal);
	ft_printf(1, "start\n");
	while (ft_print_prompt())
		;
}
