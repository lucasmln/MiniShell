#include "../../includes/minishell.h"

void		ft_free_empty(void)
{
	ft_free_av(g_shell.argv_empty);
	free(g_shell.argv_empty);
	g_shell.argv_empty = NULL;
}

void		ft_pass_input(char *buf, int *i)
{
	if (buf[*i] == '<')
	{
		*i += 1;
		while (buf[*i] == ' ')
			*i += 1;
		while (buf[*i] != ' ' && buf[*i])
			*i += 1;
		while (buf[*i] == ' ')
			*i += 1;
	}
}
