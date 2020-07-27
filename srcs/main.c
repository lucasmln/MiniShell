/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/25 18:19:55 by lmoulin          ###   ########.fr       */
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

int			ft_copy_env(const char **env)
{
	int		i;
	int		len;
	char	*tmp;

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

int			ft_print_prompt()
{
	int		i;
	int		ret;
	int		pos;

	signal(SIGQUIT, ft_get_signal);
	signal(SIGINT, ft_get_signal);
	signal(SIGTERM, ft_get_signal);
	g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
	i = ft_strlen(g_shell.dir);
	while (i >= 0 && g_shell.dir[i] != '/')
		i--;
	ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET, &g_shell.dir[i + 1]);
	ret = read(0, g_shell.buf, BUF_SIZE);
	g_shell.buf[ret] = '\0';
	i = 0;
	while (g_shell.buf[i] && g_shell.buf[i] == ' ')
		i++;
	if (!ft_strncmp(&g_shell.buf[i], "cd", ft_strlen("cd")))
		ft_cd(&g_shell.buf[i + 2]);
	else if (!ft_strncmp(&g_shell.buf[i], "pwd", ft_strlen("pwd")))
		ft_pwd(&g_shell.buf[i + 3]);
		else if (!ft_strncmp(&g_shell.buf[i], "export ", ft_strlen("export")))
			ft_export(&g_shell.buf[i + ft_strlen("export")]);
	else if (!ft_strncmp(&g_shell.buf[i], "env", ft_strlen("env")))
		ft_env(&g_shell.buf[i + ft_strlen("env")], g_shell.env);
	else if (!ft_strncmp(&g_shell.buf[i], "echo", ft_strlen("echo")))
		ft_echo(&g_shell.buf[i + ft_strlen("echo")]);
	else if (!(ft_strncmp(&g_shell.buf[i], "ls", ft_strlen("ls"))))
		ft_ls(&g_shell.buf[i + ft_strlen("ls")]);
	else if (!(ft_strncmp(&g_shell.buf[i], "unset", ft_strlen("unset"))))
		ft_unset(&g_shell.buf[i + ft_strlen("unset")]);
	else if (!ft_strncmp(g_shell.buf, "exit", ft_strlen("exit")))
		return (0);
	else
		ft_printf(1, "minishell: command not found %s", g_shell.buf);
	free(g_shell.dir);
	g_shell.dir = NULL;
	return (1);
}

int			main(int ac, char **av, const char **env)
{
	int		ret;
	int		i;
	int		pos;

	if (!(ft_copy_env(env)))
		return (-1);
	i = 0;
	while (ft_print_prompt())
		;
}
