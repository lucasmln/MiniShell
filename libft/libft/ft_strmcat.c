/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/18 11:31:35 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/18 14:45:00 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../../includes/ft_printf.h"

char		*ft_strmcat(char *s1, char const *s2)
{
	char	*new;
	int		len1;
	int		len2;

	len1 = 0;
	len2 = 0;
	if (!s1)
		return (NULL);
	ft_printf(1, "new = %s\n", s1);
	if (!s2)
		return (s1);
	while (s1[len1])
		len1++;
	while (s2[len2])
		len2++;
	if (!(new = malloc(sizeof(char) * (len1 + len2 + 1))))
		return (NULL);
	len1 = -1;
	len2 = 0;
	while (s1[++len1])
		new[len1] = s1[len1];
	while (s2[len2])
	{
		new[len1 + len2] = s2[len2];
		len2++;
	}
	new[len1 + len2] = '\0';
	free(s1);
	s1 = new;
	return (new);
}
