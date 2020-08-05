/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:18 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/31 17:00:04 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_error_cd(struct stat info, char *buf)
{
	if (S_ISREG(info.st_mode))
		ft_printf(1, "cd: not a directory: %s\n", buf);
	else
		ft_printf(1, "cd: no such file or directory: %s\n", buf);
	g_shell.ret = 1;
}

int		ft_cd_home()
{
	char	*dir;
	int		i;
	int		count;

	if (!(dir = malloc(sizeof(char) * BUF_SIZE + 1)))
		return (0);
	dir = getcwd(dir, BUF_SIZE);
	i = 0;
	count = 0;
	while (dir[i++])
		if (dir[i] == '/')
			count++;;
	while (count-- >= 2)
		chdir("..");
	free(dir);
	dir = NULL;
	return (1);
}

int			ft_cd(char *buf)
{
	struct stat	info;
	int			i;
	int			save;
	char		*tmp;

	i = 0;
	save = -1;
	if (buf[i] != '\n' && buf[i] != '\0' && buf[i] != ';' && buf[i] != ' ')
		return (ft_pwd_error(buf, 1));
	while (buf[i] && buf[i] == ' ' && buf[i] != ';')
		i++;
	if (buf[i] == '\n' || buf[i] == ';' || !buf[i])
		return (ft_cd_home());
	while (buf[++i] && save == -1)
	{
		if (buf[i] == ';')
			save = i;
		if (save == -1 && buf[i] == '\n' && !buf[i + 1])
			buf[i] = '\0';
	}
	save != -1 ? (buf[save] = '\0') : 0;
	g_shell.output = ft_strtrim(buf, " ");
	if (stat(g_shell.output, &info) == -1 || S_ISREG(info.st_mode))
		ft_error_cd(info, g_shell.output);
	else
		chdir(g_shell.output);
	if (save != -1)
	{
		buf[save] = ';';
		tmp = ft_strdup(&buf[save + 1]);
		free(g_shell.output);
		g_shell.output = NULL;
		return (ft_get_cmd(tmp));
	}
		free(g_shell.output);
		g_shell.output = NULL;
	return (1);
}
