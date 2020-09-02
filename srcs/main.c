/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/02 13:25:24 by lmoulin          ###   ########.fr       */
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

int			ft_excec(char *buf)
{
	struct stat		info;
	int				i;
	int				save;
	int				pos;
	pid_t			pid;
	int				status;
	char			**argv;

	if ((pid = fork()))
		return (-1);
	i = -1;
	save = -1;
	pos = 0;
	while (buf[++i] && save == -1)
		if (buf[i] == ';')
			save = i;
	save != -1 ? buf[save] = '\0' : 0;
	i = 0;
	while (buf[i] == ' ')
		i++;
	g_shell.output = ft_strdup(&buf[i]);
	i = 0;
	argv = ft_split(g_shell.output, ' ');
	if (stat(argv[0], &info) == 0 && S_ISDIR(info.st_mode) == 0)
	{
		if (S_IRUSR)
		{
			if (pid == 0)
				exit(execve(argv[0], argv, g_shell.env));
			waitpid(pid, &status, 0);
		}
	}
	return (1);
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
	else if (!(ft_strncmp(&buf[i], "ls", ft_strlen("ls"))))
		res = ft_ls(&buf[i + ft_strlen("ls")]);
	else if (!(ft_strncmp(&buf[i], "unset", ft_strlen("unset"))))
		res = ft_unset(&buf[i + ft_strlen("unset")]);
	else if (!ft_strncmp(&buf[i], "bash", ft_strlen("bash")))
	{
		res = ft_excec(&buf[i + ft_strlen("bash")]);
		ft_printf(1, "after\n");
	}
	else if (!ft_strncmp(buf, "exit", ft_strlen("exit")))
		res = 0;
	else
		ft_printf(1, "minishell: command not found %s\n", buf);
	ft_printf(1, "hey\n");
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
