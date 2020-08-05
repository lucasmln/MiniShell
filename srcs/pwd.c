/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:37 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/31 16:56:54 by lmoulin          ###   ########.fr       */
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
	int		i;

	i = 0;
	if (buf[i] != ' ' && buf[i] != ';' && buf[i] != '\n' && buf[i] != '\0')
		return ((ft_pwd_error(buf, 1)));
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\0' && buf[i] != '\n' && buf[i] != ';')
	{
		ft_pwd_error(buf, 0);
		while (buf[i] && buf[i] != ';')
			i++;
		if (buf[i] == ';')
		{
			g_shell.tmp = ft_strdup(&buf[i + 1]);
			free(buf);
			buf = g_shell.tmp;
			return (ft_get_cmd(buf));
		}
	}
	if (!(pwd = malloc(sizeof(char) * BUF_SIZE + 1)))
		return ((i = 0 * ft_printf(1, "MiniShell: error malloc\n")));
	pwd = getcwd(pwd, BUF_SIZE);
	pwd[BUF_SIZE] = '\0';
	ft_printf(1, "%s\n", pwd);
	free(pwd);
	pwd = NULL;
	if (buf[i++] == ';')
	{
		pwd = ft_strdup(&buf[i]);
		return (ft_get_cmd(pwd));
	}
	return (1);
}
