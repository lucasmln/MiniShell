/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:32:42 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:32:43 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	ft_check_redir(g_shell.tmp, fd);
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
	else if (g_shell.pip != -1)
		;
	else
	{
		i = 0;
		ft_change_env_pwd("OLDPWD=");
		while (ft_strncmp(g_shell.env[i], "HOME", 4))
			i++;
		if (!g_shell.env[i])
			ft_printf(1, "minishell: no HOME path\n");
		else
			chdir(&g_shell.env[i][5]);
		ft_change_env_pwd("PWD=");
	}
	ft_free_empty();
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (1);
}

char		*ft_del_input(char *buf)
{
	int		i;
	int		len;
	char	*new;
	int		k;

	i = -1;
	len = 0;
	while (buf[++i] && ++len)
	{
		if (buf[i] == '<')
			ft_pass_input(buf, &i);
	}
	if (!(new = malloc(sizeof(char) * (len + 1))))
		exit(-1000);
	i = -1;
	k = 0;
	while (buf[++i])
	{
		if (buf[i] == '<')
			ft_pass_input(buf, &i);
		new[k++] = buf[i];
	}
	ft_strdel(&buf);
	new[k] = '\0';
	return (new);
}

int			ft_avchr(char **av, char *s)
{
	int		i;
	int		k;
	char	c;

	i = 0;
	while (av[i])
	{
		k = 0;
		while (av[i][k] && av[i][k] != '=')
			k++;
		c = av[i][++k];
		av[i][k] = '\0';
		if (!ft_strncmp(av[i], s, ft_strlen(s)))
		{
			av[i][k] = c;
			return (i);
		}
		av[i][k] = c;
		i++;
	}
	return (-1);
}

void		ft_change_env_pwd(char *s)
{
	int		k;
	char	*tmp;

	tmp = NULL;
	tmp = getcwd(tmp, 0);
	k = ft_avchr(g_shell.env, s);
	ft_strdel(&g_shell.env[k]);
	g_shell.env[k] = ft_strdup(s);
	g_shell.env[k] = ft_str_add(g_shell.env[k], tmp);
	k = ft_avchr(g_shell.sort_env, s);
	ft_strdel(&g_shell.sort_env[k]);
	g_shell.sort_env[k] = ft_strdup(s);
	g_shell.sort_env[k] = ft_str_add(g_shell.sort_env[k], tmp);
	ft_strdel(&tmp);
}

int			ft_cd_2(int redir, char *buf)
{
	struct stat		info;

	g_shell.output = ft_strtrim(buf, " ");
	if (stat(g_shell.output, &info) == -1 || S_ISREG(info.st_mode))
		S_ISREG(info.st_mode) ?
			ft_printf(1, "cd: not a directory: %s\n", g_shell.output) :
			ft_printf(1, "cd: no such file or directory: %s\n", g_shell.output);
	else if (redir == -1)
		ft_printf(1, "cd: error string after redirection\n");
	else if (g_shell.pip != -1)
		;
	else
	{
		ft_change_env_pwd("OLDPWD=");
		chdir(g_shell.output);
		ft_change_env_pwd("PWD=");
	}
	ft_strdel(&buf);
	ft_strdel(&g_shell.output);
	ft_free_empty();
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (1);
}

int			ft_cd(char *buf)
{
	int			i;
	int			redir;

	i = 0;
	redir = 0;
	if (buf[i] != '\0' && buf[i] != ' ' && buf[i] != '>' && buf[i] != '<')
		return (ft_pwd_error(buf, 1));
	while (buf[i] != '\0' && buf[i] == ' ' && buf[i] != '>' && buf[i] != '<')
		i++;
	if (!buf[i] || buf[i] == '>' || buf[i] == '<')
		return (ft_cd_home(&buf[i]));
	g_shell.fd = ft_check_redir(ft_strdup(buf), g_shell.fd);
	if (g_shell.fd[0] != -2)
		buf = ft_del_redir(ft_strdup(buf));
	i = -1;
	while (buf[++i] && !redir)
		if (buf[i] == '>' || buf[i] == '<')
		{
			ft_check_redir(ft_strdup(buf), g_shell.fd);
			buf = ft_del_redir(ft_strdup(buf));
			buf = ft_del_input(buf);
			break ;
		}
	return (ft_cd_2(redir, buf));
}
