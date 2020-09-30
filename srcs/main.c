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
	int		nb;

	nb = 0;
	if ((!ft_strncmp(buf, "exit", 4) && !buf[4] )|| !ft_strncmp(buf, "exit ", 5))
	{
		i = 5;
		while (buf[i] == ' ')
			i++;
		while (buf[i])
		{
			nb = ft_isdigit(buf[i]) ? 1 : nb;
			if (!ft_isdigit(buf[i]))
			{
				ft_printf(1,
				"minishell: exit: %s argument numérique nécessaire\n", &buf[5]);
				g_shell.ret = 2;
				return (1);
			}
			i++;
		}
		if (nb)
			g_shell.ret = ft_atoi(&buf[5]);
		return (1);
	}
	return (0);
}

int			ft_get_cmd(char *buf)
{
	int		i;

	i = 0;
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
		ft_free_exit(buf, 0);
	else
		ft_exe(&buf[i]);
	ft_strdel(&buf);
	g_shell.argv_empty ? ft_free_empty() : 0;
	if (!ft_strncmp(g_shell.buf, "\n", ft_strlen(g_shell.buf)))
		g_shell.ret = g_shell.save_ret;
	return ((g_shell.ret));
}

void		ft_get_signal(int code)
{
	int			i;

	if (code == SIGINT)
	{
		g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
		i = ft_strlen(g_shell.dir);
		g_shell.d_q = 0;
		g_shell.s_q = 0;
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
	signal(SIGQUIT, ft_get_signal);
	while (1)
	{
		ft_print_prompt();
		free(g_shell.save_buf);
		g_shell.save_buf = NULL;
		g_shell.save_ret = g_shell.ret;
	}
}
