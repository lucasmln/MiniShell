/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/14 17:42:11 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"
#include "../libft/libft/libft.h"
#include "../includes/color.h"
#include "unistd.h"
# define BUF_SIZE 2048

int main(int ac, char **av, char **env)
{
	int		ret;
	char	buf[BUF_SIZE + 1];
	char	*dir;

	while (ft_strncmp(buf, "exit", ft_strlen("exit")))
	{
		dir = getcwd(dir, BUF_SIZE);
		ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET, dir);
		ret = read(0, buf, BUF_SIZE);
	}

}
