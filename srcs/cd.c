/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:13:18 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/14 21:06:43 by lmoulin          ###   ########.fr       */
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

void		ft_pass_redir_cd(char *buf, int *i, int *fd)
{
	while (buf[++*i] == ' ')
		;
	while (buf[*i] && buf[*i] != ' ')
		*i += 1;
	g_shell.c = buf[*i];
	buf[*i] = '\0';
	g_shell.tmp = ft_strdup(buf);
	buf[*i] = g_shell.c;
	ft_check_redir(g_shell.tmp, fd, 0);
	while (buf[*i] && buf[*i] == ' ')
		*i += 1;
}

int			ft_cd_home(char *buf)
{
	int		i;
	int		fd;

	fd = 1;
	if (buf[(i = 0)] == '>')
		ft_pass_redir_cd(buf, &i, &fd);
	if (buf[i] && buf[i] != ' ')
		ft_printf(1, "cd: error string after redirection\n");
	else
	{
		i = 0;
		while (ft_strncmp(g_shell.env[i], "HOME", 4))
			i++;
		if (!g_shell.env[i])
			ft_printf(1, "minishell: no HOME path\n");
		else
			chdir(&g_shell.env[i][5]);
	}
	if (g_shell.save != -1)
	{
		free(g_shell.output);
		g_shell.output = NULL;
		return (ft_check_parse(ft_strdup(&g_shell.save_buf[g_shell.save + 1])));
	}
	return (1);
}

int			ft_cd(char *buf)
{
	struct stat	info;
	int			i;
	int			k;
	int			redir;
	char		*tmp;

	i = 0;
	redir = 0;
	k = -1;
	if (buf[i] != '\0' && buf[i] != ' ' && buf[i] != '>')
		return (ft_pwd_error(buf, 1));
	while (buf[i] != '\0' && buf[i] == ' ' && buf[i] != '>')
		i++;
	if (!buf[i] || buf[i] == '>')
		return (ft_cd_home(&buf[i]));
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
			g_shell.fd = ft_check_redir(g_shell.tmp, g_shell.fd, 0);
			buf[i] = g_shell.c;
			while (buf[i] == ' ')
				i++;
			if (buf[i])
				redir = -1;
			buf[k] = '\0';
			break ;
		}
	g_shell.output = ft_strtrim(buf, " ");
	if (stat(g_shell.output, &info) == -1 || S_ISREG(info.st_mode))
		ft_error_cd(info, g_shell.output);
	else if (redir == -1)
		ft_printf(1, "cd: error string after redirection\n");
	else
		chdir(g_shell.output);
	if (g_shell.save != -1)
	{
		tmp = ft_strdup(&g_shell.save_buf[g_shell.save + 1]);
		free(g_shell.output);
		g_shell.output = NULL;
		return (ft_check_parse(tmp));
	}
	free(g_shell.output);
	g_shell.output = NULL;
	return (1);
}
