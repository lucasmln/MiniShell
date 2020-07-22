/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdelchar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/18 11:50:23 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/18 16:11:49 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../includes/ft_printf.h"

int		ft_strdelchar(char *str, char c)
{
	char	**split;
	int		i;

	split = ft_split(str, c);
	i = 0;
	ft_printf(1, "aaaa\n");
	if (str)
		free(str);
	str = NULL;
	while (split[i])
	{
		if (!(ft_strjoin(str, split[i])))
			return (0);
		i++;
	}
	return (1);
}
