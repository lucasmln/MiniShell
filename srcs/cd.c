/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:18 by lmoulin           #+#    #+#             */
/*   Updated: 2020/08/31 18:14:09 by lmoulin          ###   ########.fr       */
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

int		ft_cd_home(char *buf, int save)
{
	char	*dir;
	int		i;
	int		count;
	int		fd;
	char	*tmp;

	fd = 1;
	i = 0;
	if (buf[i] == '>')
	{
		count = i;
		while (buf[++i] == ' ')
			;
		while (buf[i] && buf[i] != ' ')
			i++;
		g_shell.c = buf[i];
		buf[i] = '\0';
		g_shell.tmp = ft_strdup(buf);
		buf[i] = g_shell.c;
		ft_check_redir(g_shell.tmp, fd, 0);
		while (buf[i] && buf[i] == ' ')
			i++;
	}
	if (buf[i] && buf[i] != ' ')
		ft_printf(1, "cd: error string after redirection\n");
	else
	{
		if (!(dir = malloc(sizeof(char) * BUF_SIZE + 1)))
			return (0);
		dir = getcwd(dir, BUF_SIZE);
		i = 0;
		count = 0;
		while (dir[i++])
			if (dir[i] == '/')
				count++;
		while (count-- >= 2)
			chdir("..");
		free(dir);
		dir = NULL;
		i = 0;
	}
	if (save != -1)
	{
		buf[save] = ';';
		tmp = ft_strdup(&buf[save + 1]);
		free(g_shell.output);
		g_shell.output = NULL;
		return (ft_get_cmd(tmp));
	}
	return (1);
}

int			ft_cd(char *buf)
{
	struct stat	info;
	int			i;
	int			k;
	int			fd;
	int			save;
	int			redir;
	char		*tmp;

	i = -1;
	save = -1;
	redir = 0;
	fd = 1;
	k = -1;
	while (buf[++i] && save == -1)
		if (buf[i] == ';')
			save = i;
	i = 0;
	if (buf[i] != '\0' && buf[i] != ';' && buf[i] != ' ' && buf[i] != '>')
		return (ft_pwd_error(buf, 1));
	while (buf[i]!= '\0' && buf[i] == ' ' && buf[i] != ';' && buf[i] != '>')
		i++;
	if (buf[i] == ';' || !buf[i] || buf[i] == '>')
		return (ft_cd_home(&buf[i], save));
	save != -1 ? (buf[save] = '\0') : 0;
	i = -1;
	while (buf[++i])
		if (buf[i] == '>')
		{
			redir = 1;
			k = i;
			while (buf[++i] == ' ')
				;
			while (buf[i] && buf[i] != ' ')
				i++;
			g_shell.c = buf[i];
			buf[i] = '\0';
			g_shell.tmp = ft_strdup(&buf[k]);
			fd = ft_check_redir(g_shell.tmp, fd, 0);
			buf[i] = g_shell.c;
			while (buf[i] == ' ')
				i++;
			if (buf[i])
				redir = -1;
			buf[k] = '\0';
			break;
		}
	g_shell.output = ft_strtrim(buf, " ");
	if (stat(g_shell.output, &info) == -1 || S_ISREG(info.st_mode))
		ft_error_cd(info, g_shell.output);
	else if (redir == -1)
		ft_printf(1, "cd: error string after redirection\n");
	else
		chdir(g_shell.output);
	if (save != -1)
	{
		k != -1 ? buf[k] = '>' : 0 ;
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
