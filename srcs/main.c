/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/14 18:30:05 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"
#include "../libft/libft/libft.h"
#include "../includes/color.h"
#include "unistd.h"
#include "stdio.h"
# define BUF_SIZE 2048

void		ft_cd(char **buf)
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
		if (!ft_strncmp(buf, "cd ", ft_strlen("cd ")))
		{
			chdir("..");
			printf("%s| chdir\n", buf + 3);
		}
	}

}
