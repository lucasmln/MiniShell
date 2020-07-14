/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/14 17:33:54 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"
#include "../libft/libft/libft.h"

#include "unistd.h"
# define BUF_SIZE 2048

#define BOLDRED     "\033[1m\033[31m"
#define BOLDCYAN    "\033[1m\033[36m"
#define RESET		"\033[0m"

int main(int ac, char **av, char **env)
{
	int		i;
	char	buf[BUF_SIZE + 1];
	char	*dir;

	while (ft_strncmp(buf, "exit", ft_strlen(buf)))
	{
		dir = getcwd(dir, BUF_SIZE);
		ft_printf(1, "" BOLDRED "➜ " RESET BOLDCYAN " %s " RESET, dir);
	}
}
