/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/15 13:51:57 by lmoulin          ###   ########.fr       */
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

void		ft_error_cd(struct stat info, char *buf)
{
	if (S_ISREG(info.st_mode))
		ft_printf(1, "cd: not a directory: %s\n", buf);
	else
		ft_printf(1, "cd: no such file or directory: %s\n", buf);
}

void		ft_cd(char *buf, char *dir)
{
	struct stat	info;
	int			i;
	int			o;
	char		*new_dir;

	i = 0;
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

void		ft_echo(char *buf)
{

}

int main(int ac, char **av, char **env)
{
	int		ret;
	int		i;
	int		pos;
	char	buf[BUF_SIZE + 1];
	char	*dir;

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
		if (!ft_strncmp(&buf[i], "cd ", ft_strlen("cd ")))
			ft_cd(&buf[i + 3], dir);
		if (!ft_strncmp(&buf[i], "echo ", ft_strlen("echo ")))
			ft_echo(&buf[i]);
	}

}
















