/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezonda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 14:57:19 by ezonda            #+#    #+#             */
/*   Updated: 2019/10/29 11:54:24 by ezonda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/execute.h"

void		cmd_pipe(t_cmd *cmd, t_var *data)
{
	t_pipe_cmd		*pcmd;
	int				pipes[2];
	int				pid[2];

	pcmd = (t_pipe_cmd *)cmd;
	if (pipe(pipes) != 0)
		ft_putendl_fd("\n21sh: pipe: syntax error", 2);
	else if ((pid[0] = fork()) == 0)
	{
//		ft_printf("\nget type left1\n");
//		ft_printf("\npipe0 : %d - pipe1 : %d\n", pipes[0], pipes[1]);
		dup2(pipes[1], STDOUT_FILENO);
		close(pipes[0]);
//		ft_printf("\nget type left2\n");
		get_cmd_type(pcmd->left, data);
		exit(0);
	}
	if ((pid[1] = fork()) == 0)
	{
//		ft_printf("\nget type right1\n");
		dup2(pipes[0], STDIN_FILENO);
		close(pipes[1]);
//		ft_printf("\nget type right2\n");
		get_cmd_type(pcmd->right, data);
		exit(0);
	}
	close(pipes[0]);
	close(pipes[1]);
	waitpid(-1, 0, 0);
	waitpid(-1, 0, 0);
}

void		cmd_redir(t_cmd *cmd, t_var *data)
{
	t_redirection_cmd	*rcmd;
	int					new_fd;
	int					back_fd;

	rcmd = (t_redirection_cmd *)cmd;
//	ft_printf("\nfile: |%s| - mode: |%d| - fd: |%d|", rcmd->file, rcmd->mode, rcmd->fd);
	new_fd = open(rcmd->file, rcmd->mode, S_IRUSR | S_IWUSR);
	back_fd = dup(rcmd->fd);
	dup2(new_fd, rcmd->fd);
	close(new_fd);
	ft_putchar('\n');
	get_cmd_type(rcmd->cmd, data);
	dup2(back_fd, rcmd->fd);
}

void		cmd_basic(t_cmd *cmd, t_var *data)
{
	int			i;
	char		**split;
	t_list		*cur;
	t_exec_cmd	*ecmd;

	i = 0;
	ecmd = (t_exec_cmd *)cmd;
	data->argv = malloc(sizeof(char**) * (ft_lstcount(ecmd->argv) + 1));
//	data->argv = (char**)malloc(sizeof(char*) * BUFF_SHELL);
	cur = ecmd->argv;
	while (cur)
	{
		data->argv[i] = cur->content;
//		ft_printf("\nargv: %s\n", cur->content);
		cur = cur->next;
		i++;
	}
	data->argv[i] = NULL;
//	split = ft_strsplit(data->cmds[data->cmd_index], ' ');
//	while (split[i])
//	{
//		data->argv[i] = ft_strdup(split[i]);
//		i++;
//	}
//	data->argv[i] = NULL;
	init_exec(data);
}

void		get_cmd_type(t_cmd *cmd, t_var *data)
{
	int		i;
	char	*path;
	char	**bin_path;

	i = 0;
//	ft_printf("\n\nin cmd_type\n\n");
	while (is_whitespaces(data->lex_str[i]))
		i++;
	if (i == ft_strlen(data->lex_str))
	{
		ft_putchar('\n');
		return;
	}
	path = get_var("PATH=", data->environ);
	bin_path = ft_strsplit(path, ':');
	if (cmd->type == PIPE)
	{
//		if (cmd->single_pipe == 1)
//			ft_printf("\nSINGLE PIPE\n");
//		ft_printf("\nPIPE\n");
		cmd_pipe(cmd, data);
	}
	else if (cmd->type == REDIR)
	{
//		ft_printf("\nREDIR\n");
		cmd_redir(cmd, data);
	}
	else if (cmd->type == BASIC)
	{
//		ft_printf("\nBASIC\n");
		cmd_basic(cmd, data);
	}
//	else
//		ft_printf("\nNO CMD\n");
}
