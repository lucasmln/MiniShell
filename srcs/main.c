/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/17 17:17:57 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"
#include "../libft/libft/libft.h"
#include "../includes/color.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "stdio.h"
# define BUF_SIZE 2048
# define USER env[18]

void		ft_error_cd(struct stat info, char *buf)
{
	if (S_ISREG(info.st_mode))
		ft_printf(1, "cd: not a directory: %s\n", buf);
	else
		ft_printf(1, "cd: no such file or directory: %s\n", buf);
}

void		ft_cd_home(char *buf)
{
	char	*dir;
	int		i;
	int		count;

	if (!(dir = malloc(sizeof(char) * BUF_SIZE + 1)))
		return;
	dir = getcwd(dir, BUF_SIZE);
	i = 0;
	count = 0;
	while (dir[i++])
		if (dir[i] == '/')
			count++;;
	while (count-- >= 2)
		chdir("..");
}

void		ft_cd(char *buf)
{
	struct stat	info;
	int			i;

	i = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (buf[i] == '\n')
	{
		ft_cd_home(buf);
		return;
	}
	while (buf[i] && buf[i] != '\n')
		i++;
	buf[i] = '\0';
	i = 0;
	while (buf[i] && buf[i] == ' ')
		i++;
	if (stat(&buf[i], &info) == -1 || S_ISREG(info.st_mode))
	{
		ft_error_cd(info, &buf[i]);
		return;
	}
	chdir(&buf[i]);
}

void		ft_pwd_error(char *buf, int error)
{
	if (error == 0)
		ft_printf(1, "pwd: too many arguments\n");
	else if (error == 1)
		ft_printf(1, "minishell: command not found pwd%s", buf);
}

void		ft_pwd(char *buf)
{
	char	*pwd;
	int		i;

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\n')
	{
		if (i != 0)
			ft_pwd_error(buf, 0);
		else
			ft_pwd_error(buf, 1);
		return;
	}
	if (!(pwd = malloc(sizeof(char) * BUF_SIZE + 1)))
		return;
	pwd = getcwd(pwd, BUF_SIZE);
	pwd[BUF_SIZE] = '\0';
	ft_printf(1, "%s\n", pwd);
	free(pwd);
	pwd = NULL;
}
/*
char		*ft_check_export(char *buf, char **var)
{
	int		i;
	int		save;
	int		len;

	i = 0;
	len = 0;
	while (buf[i])
	{
		while (buf[i] == ' ')
			i++;
		if (ft_isprint(buf[i]))
		{
			save = i;
			while (buf[i] && ft_isalnum(buf[i]))
				i++;
			
			if (!(var = malloc(sizeof(char) * (i - save + 1))))
				return (NULL);
			ft_strlcpy(var, &buf[save], i - save);
		}
		if (buf[i] == '=' && ft_isalpha(buf[i - 1]))
			;
	}
}

void		ft_export(char *buf, char **env)
{
	int		i;
	int		k;
	char	*var;
	char	**new;

	i = 0;
	k = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] == '\n' && buf[i + 1] == '\0')
		while (env[k])
		{
			ft_printf(1, "%s\n", env[k++]);
			return ;
		}
	ft_check_export(buf, new);

}
*/
void		ft_env(char *buf, char **env)
{
	int		i;

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\n' || (buf[i] == '\n' && buf[i + 1] != '\0'))
	{
		if (i == 0)
			return ((void)ft_printf(1, "minishell: command not found env%s", buf));
		ft_printf(1, "env: take no argument\n");
		return ;
	}
	i = 0;
	while (env[i])
		ft_printf(1, "%s\n", env[i++]);
}

void		ft_get_signal(int code)
{
	if (code == SIGINT)
		//Pas d'exit, relancer une affiche de prompt
	if (code == SIGQUIT)
		ft_printf(1, "exit: signal code %d\n", code);
	exit(code);
}


int			main(int ac, char **av, char **env)
{
	int		ret;
	int		i;
	int		pos;
	char	buf[BUF_SIZE + 1];
	char	*dir;

	while (ft_strncmp(buf, "exit", ft_strlen("exit")))
	{
		signal(SIGABRT, ft_get_signal);
		signal(SIGQUIT, ft_get_signal);
		signal(SIGINT, ft_get_signal);
		signal(SIGSEGV, ft_get_signal);
		signal(SIGTERM, ft_get_signal);
		dir = getcwd(dir, BUF_SIZE);
		i = ft_strlen(dir);
		while (i >= 0 && dir[i] != '/')
			i--;
		ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET, &dir[i + 1]);
		ret = read(0, buf, BUF_SIZE);
		buf[ret] = '\0';
		i = 0;
		while (buf[i] && buf[i] == ' ')
			i++;
		if (!ft_strncmp(&buf[i], "cd", ft_strlen("cd")))
			ft_cd(&buf[i + 2]);
		else if (!ft_strncmp(&buf[i], "pwd", ft_strlen("pwd")))
			ft_pwd(&buf[i + 3]);
	//	else if (!ft_strncmp(&buf[i], "export ", ft_strlen("export")))
	//		ft_export(&buf[i + ft_strlen("export")], env);
		else if (!ft_strncmp(&buf[i], "env", ft_strlen("env")))
			ft_env(&buf[i + ft_strlen("env")], env);
		else if (!ft_strncmp(buf, "exit", ft_strlen("exit")))
			break;
		else
			ft_printf(1, "minishell: command not found %s", buf);
		free(dir);
		dir = NULL;
	}
}
















