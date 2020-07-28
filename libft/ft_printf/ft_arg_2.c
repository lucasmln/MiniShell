/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arg_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 14:47:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/28 13:36:46 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "../libft/libft.h"

t_count		ft_is_p(t_count cmp, long long nb)
{
	char	*min;
	int		size;

	min = "0123456789abcdef";
	nb = (nb < 0) ? -nb : nb;
	size = ft_strlen_nb(nb, 16) + 2;
	cmp.str[0] = '0';
	cmp.str[1] = 'x';
	while (--size >= 2)
	{
		cmp.str[size] = min[nb % 16];
		nb /= 16;
	}
	return (cmp);
}

t_count		ft_is_x(t_count cmp, long long nb, char c)
{
	char	*min;
	char	*maj;
	int		size;

	min = "0123456789abcdef";
	maj = "0123456789ABCDEF";
	nb = (nb < 0) ? -nb : nb;
	size = ft_strlen_nb(nb, 16);
	while (--size >= 0)
	{
		if (c == 'x')
			cmp.str[size] = min[nb % 16];
		else
			cmp.str[size] = maj[nb % 16];
		nb /= 16;
	}
	return (cmp);
}

t_count		ft_arg_xp(va_list aux, t_count cmp, char c)
{
	long long	nb;
	int			size;

	nb = (c == 'p') ? va_arg(aux, long long) : va_arg(aux, unsigned int);
	size = ft_strlen_nb(nb, 16);
	size = (c == 'p') ? size + 2 : size;
	if (!(cmp.str = malloc(sizeof(char) * (size + 1))))
	{
		cmp.str = NULL;
		return (cmp);
	}
	cmp.str[size] = '\0';
	if (c == 'p')
		cmp = ft_is_p(cmp, nb);
	else
		cmp = ft_is_x(cmp, nb, c);
	return (cmp);
}
