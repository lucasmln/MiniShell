/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/14 19:02:32 by lmoulin          ###   ########.fr       */
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

void		ft_cd(char *buf)
{
	struct stat	info;
	int			i;

	i = 2;
	while (buf[i] && buf[i] == ' ')
		i++;
	ft_strlcat(&buf[i], "c:\\", ft_strlen(&buf[i]));
	printf("stat return = %d\n", stat(buf, &info));
	if (stat(buf, &info))
		ft_printf(1, "File not found.");
	else
		chdir(&buf[i]);
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
		if (!ft_strncmp(buf, "cd ", ft_strlen("cd ")))
		{
			ft_cd(buf);
//			chdir("..");
			printf("%s| chdir\n", buf + 3);
		}
	}

}
