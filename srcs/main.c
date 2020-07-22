/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/14 15:37:39 by lmoulin           #+#    #+#             */
/*   Updated: 2020/07/22 18:42:16 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void		ft_env(char *buf, char **env)
{
	int		i;

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] != '\n' || (buf[i] == '\n' && buf[i + 1] != '\0'))
	{
		if (i == 0)
			return ((void)ft_printf(1, "minishell: command not found env%s", buf));
		ft_printf(1, "env: take no argument\n");
		return ;
	}
	i = 0;
	while (env[i])
		ft_printf(1, "%s\n", env[i++]);
}

void		ft_error_export(char *buf)
{
	ft_printf(1, "export: not an identifier: %s\n", buf);
}

void		ft_sort_env(char **env)
{
	int		i;
	char	*tmp;

	i = -1;
	while (g_shell.sort_env[++i + 1])
	{
		if (ft_strcmp(g_shell.sort_env[i], g_shell.sort_env[i + 1]) > 0)
		{
			tmp = g_shell.sort_env[i + 1];
			g_shell.sort_env[i + 1] = g_shell.sort_env[i];
			g_shell.sort_env[i] = tmp;
			i = i - 1 >= -1 ? -1 : i - 1;
		}
	}
}

void		ft_add_var(char *buf, int *check, int i, int indice)
{
	int		k;
	char	**new;
	char	*var;

	k = 0;
	if (indice == 1)
		while (buf[i] && buf[i] != ' ')
			i++;
		if (!(var = malloc(sizeof(char) * (i + 2))))
			return ;
		if (!(new = malloc(sizeof(char *) * (++g_shell.len_env + 1))))
			return ;
		ft_strlcpy(var, buf, i + 1);
		if (indice == 0)
			var = ft_str_add(var, "=''");
		new[g_shell.len_env] = NULL;
		while (g_shell.sort_env[k])
		{
			new[k] = ft_strdup(g_shell.sort_env[k]);
			free(g_shell.sort_env[k]);
			g_shell.sort_env[k] = NULL;
			k++;
		}
		new[k] = var;
		free(g_shell.sort_env);
		g_shell.sort_env = new;
}

void		ft_get_var(int i, int check)
{
	while (g_shell.output[i])
	{
		if (!(ft_isalpha(g_shell.output[i]) || (i == 0 &&
				ft_isdigit(g_shell.output[i])) || g_shell.output[i] == '_' || (i > 0 && g_shell.output[i] == '=')))
			check = 1;
		if (check == 0 && (g_shell.output[i] == ' ' || g_shell.output[i] == '\n'
					|| !g_shell.output[i]) && i > 0)
		{
			ft_add_var(g_shell.output, &check, i, 0);
			while (g_shell.output[i] == ' ')
				i++;
			ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(g_shell.output));
			i = -1;
		}
		else if (check == 0 && g_shell.output[i] == '=' && i > 0)
		{
			ft_printf(1, "hey\n");
			ft_add_var(g_shell.output, &check, i, 1);
		}
			ft_printf(1, "check = %d\n", check);
		i++;
	}
		if (check == 0 && (g_shell.output[i] == ' ' || g_shell.output[i] == '\n'
					|| !g_shell.output[i]) && i > 0)
			ft_add_var(g_shell.output, &check, i, 1);


}

void		ft_export(char *buf)
{
	int		i;
	int		check;
	char	quote[1];

	i = 0;
	while (buf[i] == ' ')
		i++;
	if (!buf[i] || (buf[i] == '\n' && !buf[i + 1]))
	{
		while (g_shell.sort_env[i])
			ft_printf(1, "%s\n", g_shell.sort_env[i++]);
		return ;
	}
	ft_strlcpy(buf, &buf[i], ft_strlen(&buf[i]));
	ft_check_quote(buf, quote);
	if (!ft_isalpha(g_shell.output[(i = 0)]) && g_shell.output[i] != '_')
	{
		ft_error_export(&g_shell.output[i]);
		return ;
	}
	i = 0;
	check = 0;
	ft_get_var(i, check);
/*	while (g_shell.output[i])
	{
		if (check == 0 && (g_shell.output[i] == ' ' || g_shell.output[i] == '\n'
					|| !g_shell.output[i]) && i > 0)
		{
			ft_add_var(g_shell.output, &check, i, 0);
			while (g_shell.output[i] == ' ')
				i++;
			ft_strlcpy(g_shell.output, &g_shell.output[i], ft_strlen(g_shell.output));
			i = -1;
		}
		i++;
	}
		if (check == 0 && (g_shell.output[i] == ' ' || g_shell.output[i] == '\n'
					|| !g_shell.output[i]) && i > 0)
			ft_add_var(g_shell.output, &check, i, 0);
*/}

void		ft_get_signal(int code)
{
	if (code == SIGINT)
	{
		ft_printf(1, "\n");
		ft_print_prompt();
	}//Pas d'exit, relancer une affiche de prompt
	if (code == SIGQUIT)
		ft_printf(1, "exit: signal code %d\n", code);
	exit(code);
}

int			ft_copy_env(const char **env)
{
	int		i;
	int		len;
	char	*tmp;

	len = 0;
	while (env[len])
		len++;
	g_shell.len_env = len;
	if (!(g_shell.env = malloc(sizeof(char *) * (len + 1))))
		return (0);
	if (!(g_shell.sort_env = malloc(sizeof(char *) * (len + 1))))
		return (0);
	i = -1;
	while (env[++i])
	{
		g_shell.sort_env[i] = ft_strdup(env[i]);
		g_shell.env[i] = ft_strdup(env[i]);
	}
	g_shell.env[i] = NULL;
	g_shell.sort_env[i] = NULL;
	ft_sort_env(g_shell.sort_env);
	return (1);
}

int			ft_print_prompt()
{
	int		i;
	int		ret;
	int		pos;

	signal(SIGQUIT, ft_get_signal);
	signal(SIGINT, ft_get_signal);
	signal(SIGTERM, ft_get_signal);
	g_shell.dir = getcwd(g_shell.dir, BUF_SIZE);
	i = ft_strlen(g_shell.dir);
	while (i >= 0 && g_shell.dir[i] != '/')
		i--;
	ft_printf(1, "" BOLDGREEN "➜ " RESET BOLDCYAN " %s " RESET, &g_shell.dir[i + 1]);
	ret = read(0, g_shell.buf, BUF_SIZE);
	g_shell.buf[ret] = '\0';
	i = 0;
	while (g_shell.buf[i] && g_shell.buf[i] == ' ')
		i++;
	if (!ft_strncmp(&g_shell.buf[i], "cd", ft_strlen("cd")))
		ft_cd(&g_shell.buf[i + 2]);
	else if (!ft_strncmp(&g_shell.buf[i], "pwd", ft_strlen("pwd")))
		ft_pwd(&g_shell.buf[i + 3]);
		else if (!ft_strncmp(&g_shell.buf[i], "export ", ft_strlen("export")))
			ft_export(&g_shell.buf[i + ft_strlen("export")]);
	else if (!ft_strncmp(&g_shell.buf[i], "env", ft_strlen("env")))
		ft_env(&g_shell.buf[i + ft_strlen("env")], g_shell.env);
	else if (!ft_strncmp(&g_shell.buf[i], "echo", ft_strlen("echo")))
		ft_echo(&g_shell.buf[i + ft_strlen("echo")]);
	else if (!(ft_strncmp(&g_shell.buf[i], "ls", ft_strlen("ls"))))
		ft_ls(&g_shell.buf[i + ft_strlen("ls")]);
	else if (!ft_strncmp(g_shell.buf, "exit", ft_strlen("exit")))
		return (0);
	else
		ft_printf(1, "minishell: command not found %s", g_shell.buf);
	free(g_shell.dir);
	g_shell.dir = NULL;
	return (1);
}

int			main(int ac, char **av, const char **env)
{
	int		ret;
	int		i;
	int		pos;

	if (!(ft_copy_env(env)))
		return (-1);
	i = 0;
	while (ft_print_prompt())
		;
}
