/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_excute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezonda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/10 13:55:06 by ezonda            #+#    #+#             */
/*   Updated: 2019/09/26 15:36:25 by ezonda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/core.h"

int		is_builtin(t_var *data, char **cmd)
{
	if (!ft_strcmp(cmd[0], "exit"))
		exit(cmd[1] ? ft_atoi(cmd[1]) : 0);
	else if (!ft_strcmp(cmd[0], "cd"))
		return (1);
	else if (!ft_strcmp(cmd[0], "env"))
		return (1);
	else if (!ft_strcmp(cmd[0], "echo"))
		return (1);
	else if (!ft_strcmp(cmd[0], "setenv"))
		return (1);
	else if (!ft_strcmp(cmd[0], "unsetenv"))
		return (1);
	return (0);
}

int		check_exe(t_var *data, char *cmd)
{
	int		i;
	char	**split_cmd;

	i = 0;
	split_cmd = ft_strsplit_ws(cmd);
	while (split_cmd[i])
	{
		if (split_cmd[i][0] == '$')
			split_cmd[i] = get_dollar_var(split_cmd[i], data);
		i++;
	}
	if (is_builtin(data, split_cmd))
		return (1);
	return (0);
}

void	init_exec(t_var *data)
{
	int		i;
	char	*path;
	char	**bin_path;
	char	**cmds;

	i = 0;
	ft_putchar('\n');
	path = get_var("PATH=", data->environ);
	bin_path = ft_strsplit(path, ':');
	cmds = ft_strsplit(data->lex_str, ';');
	while (cmds[i])
	{
		if (!check_exe(data, cmds[i]))
			exec_cmd(bin_path, data);
		i++;
	}
}
