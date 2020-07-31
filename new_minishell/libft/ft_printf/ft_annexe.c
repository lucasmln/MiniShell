/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_annexe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 14:47:11 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/28 13:36:35 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft/libft.h"

int			ft_strlen_nb(long nb, int base)
{
	int		len;

	if (nb == 0)
		return (1);
	len = (nb < 0) ? 1 : 0;
	nb = (nb < 0) ? -nb : nb;
	while (nb >= 1)
	{
		nb /= base;
		len++;
	}
	return (len);
}

int			ft_convertible(char c)
{
	if (c == 'd' || c == 'i' || c == 'c' || c == 's' || c == 'p' || c == 'x' ||
	c == 'X' || c == 'u' || c == '%')
		return (1);
	return (0);
}

t_count		ft_init_count(int fd, t_count init, int k)
{
	if (k != 1)
	{
		init.i = 0;
		init.len = 0;
		init.str = NULL;
		init.fd = fd;
	}
	init.zero = -4294967295;
	init.space = 0;
	init.check = 0;
	return (init);
}

void		ft_strdel(char **s)
{
	if (s != NULL)
	{
		free(*s);
		*s = NULL;
	}
}
