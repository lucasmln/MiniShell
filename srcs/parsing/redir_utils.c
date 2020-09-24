/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 15:23:49 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/22 15:23:59 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		ft_len_sans_redir(char *buf)
{
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (buf[i])
	{
		if (buf[i] != '>')
			len++;
		i++;
	}
	return (len);
}

char		*ft_del_redir(char *buf)
{
	char	*new;
	int		i;
	int		k;

	i = 0;
	k = 0;
	if (!(new = malloc(sizeof(char) * ft_len_sans_redir(buf) + 1)))
		return (NULL);
	while (buf[i])
		if (buf[i] == '>')
		{
			i = buf[i + 1] == '>' ? i + 2 : i + 1;
			while (buf[i] == ' ')
				i++;
			while (buf[i] && buf[i] != ' ')
				i++;
			while (buf[i] == ' ')
				i++;
		}
		else
			new[k++] = buf[i++];
	new[k] = '\0';
	free(buf);
	buf = NULL;
	return (new);
}
