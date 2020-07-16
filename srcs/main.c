/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/16 20:26:58 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"
#include "../libft/libft/libft.h"
#include "../includes/color.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
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
			return;
		}
	ft_check_export(buf, new);

}

int			main(int ac, char **av, char **env)
{
	int		ret;
	int		i;
	int		pos;
	char	buf[BUF_SIZE + 1];
	char	*dir;

	i = -1;
	while (env[++i])
		printf("env[%d] = %s\n", i, env[i]);
	i = 0;

	while (ft_strncmp(buf, "exit", ft_strlen("exit")))
	{
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
		else if (!ft_strncmp(&buf[i], "export ", ft_strlen("export")))
			ft_export(&buf[i + ft_strlen("export")], env);
		else if (!ft_strncmp(buf, "exit", ft_strlen("exit")))
			break;
		else
			ft_printf(1, "minishell: command not found %s", buf);
		free(dir);
		dir = NULL;
	}
}
















