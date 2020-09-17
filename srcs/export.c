/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 15:15:40 by lmoulin           #+#    #+#             */
/*   Updated: 2020/09/17 10:55:42 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int			ft_env(char *buf, char **env)
{
	int		i;
	char	*tmp;
	int		fd;
	int		k;

	i = 0;
	fd = 1;
	tmp = NULL;
	k = 0;
	while (buf[i] == ' ')
		i++;
	if (buf[i] != ';' && buf[i] != '\0' && buf[i] != '>')
	{
		if (i == 0)
			ft_printf(1, "minishell: command not found env%s\n", buf);
		else
			ft_printf(1, "env: take no argument\n");
		return (2);
	}
	if (buf[i] == ';')
		tmp = ft_strdup(&buf[i + 1]);
	if (buf[i] == '>')
	{
		k = i;
		while (buf[++i] == ' ')
			;
		while (buf[i] && buf[i] != ' ')
			i++;
		g_shell.c = buf[i];
		buf[i] = '\0';
		g_shell.tmp = ft_strdup(&buf[k]);
		buf[i] = g_shell.c;
		g_shell.fd = ft_check_redir(g_shell.tmp, g_shell.fd, 0);
		while (buf[++i])
			if (buf[i] != ' ' && buf[i] != '\0')
				k = -1;
	}
	i = -1;
	while (k != -1 && env[++i])
	{
		write(fd, env[i], ft_strlen(env[i]));
		write(fd, "\n", 1);
	}
	if (k == -1)
		ft_printf(1, "env: take no argument\n");
	if (tmp)
		return (ft_get_cmd(tmp));
	return (1);
}

int			ft_error_export(char *buf)
{
	if (buf[0] == '=' && buf[1] == ' ')
		ft_printf(1, "minishell: bad assignment\n");
	else if (buf[0] == '=' && buf[1] != ' ')
		ft_printf(1, "minishell: %s not found\n", &buf[1]);
	else
		ft_printf(1, "export: not an identifier: %s\n", buf);
	free(g_shell.output);
	g_shell.output = NULL;
	g_shell.ret = 1;
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (2);
}

int			ft_sort_env(char **env)
{
	int		i;
	char	*tmp;

	(void)env;
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
	return (1);
}

char		**ft_add_var(char **env, char *buf, int len_env, int i)
{
	int		k;
	char	**new;
	char	*var;

	k = 0;
	if (!(var = malloc(sizeof(char) * (i + 2))))
		return (NULL);
	if (!(new = malloc(sizeof(char *) * (len_env + 1))))
		return (NULL);
	ft_strlcpy(var, buf, i + 1);
	new[len_env] = NULL;
	while (env[k])
	{
		new[k] = ft_strdup(env[k]);
		free(env[k]);
		env[k] = NULL;
		k++;
	}
	new[k] = var;
	free(env);
	return (new);
}

void		ft_del_var(char **env, int len)
{
	int		i;

	i = 0;
	while (env[i])
	{
		if (i >= len)
		{
			free(env[i]);
			env[i] = NULL;
		}
		i++;
	}
}

int			ft_find_var(char **env, char *var, char c)
{
	int		i;
	int		k;

	i = 0;
	while (env[i])
	{
		k = 0;
		while (env[i][k] && env[i][k] != c)
			k++;
		c = env[i][k];
		env[i][k] = '\0';
		if (!ft_strncmp(env[i], var, ft_strlen(env[i])))
		{
			env[i][k] = c;
			return (i);
		}
		env[i][k] = c;
		i++;
	}
	return (i);
}

int			ft_change_var(char **env, int len)
{
	int		k;

	k = 0;
	while (env[k] && ft_strncmp(env[k], env[len - 1], ft_strlen(env[len - 1]) -
								ft_strlen(ft_strchr(env[len - 1], '=')) - 1))
		k++;
	free(env[k]);
	env[k] = NULL;
	env[k] = ft_strdup(env[len - 1]);
	free(env[len - 1]);
	env[len - 1] = NULL;
	if (g_shell.sort_env == env)
		g_shell.len_exp--;
	else if (g_shell.env == env)
		g_shell.len_env--;
	return (1);
}

int			ft_get_var_7(int *i)
{
	int		k;

	if (!g_shell.output[*i + 1] || g_shell.output[*i + 1] == ' ')
		ft_printf(1, "minishell: bad assignment\n");
	else
	{
		k = *i + 1;
		while (g_shell.output[k] && g_shell.output[k] != ' ')
			k++;
		g_shell.c = g_shell.output[k];
		g_shell.output[k] = '\0';
		ft_printf(1, "minishell: %s not found\n", &g_shell.output[*i + 1]);
		g_shell.output[k] = g_shell.c;
	}
	return (0);
}

int			ft_get_var_6(int *i)
{
	int		k;

	k = g_shell.output[*i];
	g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output,
												++g_shell.len_exp, *i);
	g_shell.sort_env[g_shell.len_exp - 1] =
		ft_str_add(g_shell.sort_env[g_shell.len_exp - 1], "=''");
	if (ft_find_var(g_shell.sort_env,
	g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
		ft_del_var(g_shell.sort_env, (g_shell.len_exp -= 1));
	if (!k)
		return (0);
	g_shell.tmp = ft_strdup(&g_shell.output[*i + 1]);
	free(g_shell.output);
	g_shell.output = g_shell.tmp;
	*i = 0;
	return (1);
}

int			ft_get_var_5(int *i)
{
	int		k;

	if (g_shell.output[*i + 1 + (k = 0)] == '=')
	{
		while (g_shell.output[*i + k] && g_shell.output[*i + k] != ' ')
			k++;
		g_shell.output[*i + k] = '\0';
		ft_printf(1, "minishell: %s not found\n", &g_shell.output[*i + 2]);
		return (0);
	}
	if (g_shell.output[*i + 1] == ' ' || !g_shell.output[*i + 1])
	{
		if (!ft_get_var_3(i))
			return (0);
		while (g_shell.output[*i] && g_shell.output[*i] != ' ')
			*i += 1;
	}
	else
		ft_get_var_4(i);
	if (!g_shell.output[*i] || !(g_shell.tmp = ft_strdup(&g_shell.output[*i])))
		return (0);
	free(g_shell.output);
	g_shell.output = g_shell.tmp;
	*i = 0;
	return (1);
}

int			ft_get_var_4(int *i)
{
	char	*tmp;

	while (g_shell.output[*i] && g_shell.output[*i] != ' ')
		*i += 1;
	g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output,
														++g_shell.len_exp, *i);
	g_shell.env = ft_add_var(g_shell.env, g_shell.output,
														++g_shell.len_env, *i);
	if (ft_find_var(g_shell.sort_env,
			g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
		ft_change_var(g_shell.sort_env, g_shell.len_exp);
	if (ft_find_var(g_shell.env,
				g_shell.env[g_shell.len_env - 1], '=') != g_shell.len_env - 1)
		ft_change_var(g_shell.env, g_shell.len_env);
	else
	{
		tmp = g_shell.env[g_shell.len_env - 1];
		g_shell.env[g_shell.len_env - 1] = g_shell.env[g_shell.len_env - 2];
		g_shell.env[g_shell.len_env - 2] = tmp;
	}
	return (1);
}

int			ft_get_var_3(int *i)
{
	char	*tmp;

	g_shell.sort_env = ft_add_var(g_shell.sort_env, g_shell.output,
														++g_shell.len_exp, *i);
	g_shell.env = ft_add_var(g_shell.env, g_shell.output,
														++g_shell.len_env, *i);
	g_shell.env[g_shell.len_env - 1] =
					ft_str_add(g_shell.env[g_shell.len_env - 1], "=");
	g_shell.sort_env[g_shell.len_exp - 1] =
					ft_str_add(g_shell.sort_env[g_shell.len_exp - 1], "=''");
	if (ft_find_var(g_shell.sort_env,
			g_shell.sort_env[g_shell.len_exp - 1], '=') != g_shell.len_exp - 1)
		ft_change_var(g_shell.sort_env, g_shell.len_exp);
	if (ft_find_var(g_shell.env,
				g_shell.env[g_shell.len_env - 1], '=') != g_shell.len_env - 1)
		ft_change_var(g_shell.env, g_shell.len_env);
	else
	{
		tmp = g_shell.env[g_shell.len_env - 1];
		g_shell.env[g_shell.len_env - 1] = g_shell.env[g_shell.len_env - 2];
		g_shell.env[g_shell.len_env - 2] = tmp;
	}
	if (!g_shell.output[*i + 1])
		return (0);
	return (1);
}

int			ft_get_var_2(int *i)
{
	char	*tmp;

	g_shell.tmp = ft_strdup(&g_shell.output[*i]);
	ft_check_redir(g_shell.tmp, g_shell.fd, 0);
	if (g_shell.output[*i] == '>')
	{
		*i += 1;
		g_shell.tmp = ft_strdup(&g_shell.output[*i]);
		ft_check_redir(g_shell.tmp, g_shell.fd, 0);
		tmp = ft_del_redir(ft_strdup(g_shell.output));
		free(g_shell.output);
		g_shell.output = tmp;
	}
	return (1);
}

int			ft_get_var(int i)
{
	while (1)
	{
		if (g_shell.output[i] == '>')
			ft_get_var_2(&i);
		if (g_shell.output[i] == '=' && i == 0)
			g_shell.ret_export = ft_get_var_7(&i);
		else if (g_shell.output[i] == '=' && i > 0)
			g_shell.ret_export = ft_get_var_5(&i);
		else if (i > 0 && (!g_shell.output[i] || g_shell.output[i] == ' '))
			g_shell.ret_export = ft_get_var_6(&i);
		else if (g_shell.output[i] == ' ')
		{
			while (g_shell.output[i] == ' ')
				i++;
			g_shell.tmp = ft_strdup(&g_shell.output[i]);
			free(g_shell.output);
			g_shell.output = g_shell.tmp;
			i = 0;
		}
		else
			i++;
		if (!g_shell.output || !g_shell.ret_export)
			break ;
	}
	return (ft_sort_env(g_shell.sort_env));
}

int			ft_export_no_pipe(void)
{
	int		i;
	int		k;

	k = -1;
	while (g_shell.sort_env[++k])
	{
		i = 0;
		while (i < g_shell.nb_fd || (g_shell.nb_fd == 0 && i == 0))
		{
			write(g_shell.fd[i], g_shell.sort_env[k],
										ft_strlen(g_shell.sort_env[k]));
			write(g_shell.fd[i++], "\n", 1);
		}
	}
	return (0);
}

void		ft_export_pipe_2(void)
{
	int		i;
	int		k;

	k = -1;
	close(g_shell.pipe_fd[0]);
	dup2(g_shell.pipe_fd[1], STDOUT_FILENO);
	if (g_shell.save_pipfd[0])
		dup2(g_shell.save_pipfd[0], STDIN_FILENO);
	while (g_shell.sort_env[++k])
	{
		i = 0;
		while (i <= g_shell.nb_fd || (g_shell.nb_fd == 0 && i == 0))
		{
			write(g_shell.fd[i], g_shell.sort_env[k],
											ft_strlen(g_shell.sort_env[k]));
			write(g_shell.fd[i++], "\n", 1);
		}
	}
	exit(0);
}

int			ft_export_pipe(void)
{
	pid_t	pid;
	int		k;
	int		i;

	k = -1;
	g_shell.fd[g_shell.nb_fd] = g_shell.pipe_fd[1];
	pid = fork();
	if (pid == 0)
		ft_export_pipe_2();
	else
	{
		waitpid(pid, &i, 0);
		close(g_shell.pipe_fd[1]);
		g_shell.pipe_fd[1] = 0;
		if (g_shell.save_pipfd[0])
			close(g_shell.save_pipfd[0]);
		g_shell.save_pipfd[0] = g_shell.pipe_fd[0];
	}
	return (0);
}

int			ft_export(char *buf)
{
	int		i;

	ft_create_pipe();
	i = 0;
	while (buf[i] == ' ')
		i++;
	if (!buf[i] || buf[i] == '>')
	{
		g_shell.fd = ft_check_redir(ft_strdup(&buf[i]), g_shell.fd, 1);
		g_shell.pip == -1 ? ft_export_no_pipe() : ft_export_pipe();
		g_shell.fd = ft_close_fd(g_shell.fd);
		if (g_shell.save != -1 || g_shell.pip != -1)
			return (ft_ispipe_is_ptvirgule());
		return (1);
	}
	g_shell.output = ft_strdup(&buf[i]);
	if (!ft_isalpha(g_shell.output[(i = 0)]) && g_shell.output[i] != '_')
		return (ft_error_export(g_shell.output));
	g_shell.ret_export = 1;
	i = ft_get_var(0);
	g_shell.output ? free(g_shell.output) : 0;
	g_shell.output = NULL;
	if (g_shell.save != -1 || g_shell.pip != -1)
		return (ft_ispipe_is_ptvirgule());
	return (i);
}
