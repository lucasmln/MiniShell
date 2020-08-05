/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 14:34:03 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/25 16:08:29 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*pos;

	if (!s)
		return (NULL);
	pos = (char *)s;
	while (*pos != c)
	{
		if (!*pos)
			return (NULL);
		pos++;
	}
	return (pos);
}
