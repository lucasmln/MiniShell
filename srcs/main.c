/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/25 16:17:18 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char		*ft_add_path(char *buf, int *i)
{
	char	*new;
	char	*tmp;

	new = NULL;
	tmp = ft_strdup("/bin/");
	if (!ft_strncmp(&buf[*i], "pwd ", 4) ||
		(!ft_strncmp(&buf[*i], "pwd", 3) && ft_strlen(&buf[*i]) == 3))
		new = ft_str_add(tmp, &buf[*i]);
	else if (!ft_strncmp(&buf[*i], "echo ", 5))
		new = ft_str_add(tmp, &buf[*i]);
	else if (!ft_strncmp(&buf[*i], "env ", 5) |
			(!ft_strncmp(&buf[*i], "env", 3) && ft_strlen(&buf[*i]) == 3))
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
	if (!ft_strncmp(&buf[i], "pwd", 3) ||
		!ft_strncmp(&buf[i], "echo", 4) || !ft_strncmp(&buf[i], "env", 3))
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
