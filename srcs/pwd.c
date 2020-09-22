/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:37 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 12:01:36 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			ft_pwd_error(char *buf, int error)
{
	if (error == 0)
		ft_printf(1, "pwd: too many arguments\n");
	else if (error == 1)
		ft_printf(1, "minishell: command not found pwd%s\n", buf);
	g_shell.ret = 1;
	return (1);
}

int			ft_pwd(char *buf)
{
	char	*pwd;
	char	*tmp;
	int		i;
	int		fd;

	i = 0;
	fd = 0;
	if (buf[i] != ' ' && buf[i] != '\n' && buf[i] != '>' && buf[i] != '\0')
		return ((ft_pwd_error(buf, 1)));
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\0' && buf[i] != '\n' && buf[i] != '>')
	{
		ft_pwd_error(buf, 0);
		if (g_shell.save != -1)
		{
			tmp = ft_strdup(&g_shell.save_buf[g_shell.save + 1]);
			free(g_shell.save_buf);
			g_shell.save_buf = NULL;
			return (ft_check_parse(tmp));
		}
	}
	if (!(pwd = malloc(sizeof(char) * BUF_SIZE + 1)))
		return ((i = 0 * ft_printf(1, "MiniShell: error malloc\n")));
	pwd = getcwd(pwd, BUF_SIZE);
	pwd[BUF_SIZE] = '\0';
	g_shell.fd = ft_check_redir(ft_strdup(&buf[i]), g_shell.fd, 1);
	i = 0;
	while (i < g_shell.nb_fd || (g_shell.nb_fd == 0 && i == 0))
	{
		ft_printf(g_shell.fd[i], "%s", pwd);
		write(g_shell.fd[i++], "\n", 1);
	}
	g_shell.fd = ft_close_fd(g_shell.fd);
	free(pwd);
	pwd = NULL;
	if (g_shell.save != -1)
	{
		tmp = ft_strdup(&g_shell.save_buf[g_shell.save + 1]);
		free(g_shell.save_buf);
		g_shell.save_buf = NULL;
		return (ft_check_parse(tmp));
	}
	return (1);
}
