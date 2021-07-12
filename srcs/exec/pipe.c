/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mciupek <mciupek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 09:27:57 by mciupek           #+#    #+#             */
/*   Updated: 2021/04/26 09:27:59 by mciupek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipe2(int fdpipe[2], t_list **cmds, t_params *params,
							char *line)
{
	if (g_gbl.pid > 0)
	{
		close(fdpipe[1]);
		*cmds = (*cmds)->next;
		((t_cmd *)(*cmds)->content)->fd[READ] = get_fd(((t_cmd *)(*cmds)->content), 0, fdpipe[0], READ);
		dup2(((t_cmd *)(*cmds)->content)->fd[READ], STDIN);
		close(fdpipe[0]);
	}
	else if (!g_gbl.pid)
	{
		if (!ft_strncmp(((t_cmd *)(*cmds)->content)->cmd[0], "cat", 3) && !((t_cmd *)(*cmds)->content)->cmd[1] && ((t_cmd *)(*cmds)->content)->in)
		{
			((t_cmd *)(*cmds)->content)->fd[READ] = get_fd(((t_cmd *)(*cmds)->content), 0, fdpipe[0], READ);
			dup2(((t_cmd *)(*cmds)->content)->fd[READ], STDIN);
			close(((t_cmd *)(*cmds)->content)->fd[READ]);
		}
		close(fdpipe[0]);
		((t_cmd *)(*cmds)->content)->fd[WRITE] = get_fd(((t_cmd *)(*cmds)->content), 0644, fdpipe[1], WRITE);
		dup2(((t_cmd *)(*cmds)->content)->fd[WRITE], STDOUT);
		if (ft_exec((t_cmd *)(*cmds)->content, params->env))
			g_gbl.exit = 127;
		close(fdpipe[1]);
		free(line);
		ft_exit(*cmds, params->env, params->hist);
	}
	return (0);
}

static int			create_pipe(t_list **cmds, t_params *params,
							int *nb_wait, char *line)
{
	int		fdpipe[2];

	if (pipe(fdpipe) == -1)
	{
		ft_putstr_fd("pipe: pipe failed\n", STDERROR);
		return (-1);
	}
	(*nb_wait)++;
	g_gbl.pid = fork();
	if (g_gbl.pid < 0)
	{
		ft_putstr_fd("pipe: fork failed\n", STDERROR);
		return (-1);
	}
	else
		return (create_pipe2(fdpipe, cmds, params, line));
}

static int		final_pipe(t_list *cmds, t_list *env, int fd[2])
{
	((t_cmd *)(cmds)->content)->fd[WRITE] = get_fd(((t_cmd *)(cmds)->content), 0644, fd[1], WRITE);
	dup2(((t_cmd *)(cmds)->content)->fd[WRITE], STDOUT);
	if (ft_exec((t_cmd *)cmds->content, env))
	{
		if (errno == 2)
			g_gbl.exit = 1;
		reset_fds(fd);
		reset_fds(((t_cmd *)cmds->content)->fd);
		return (-1);
	}
	return (0);
}

int		ft_pipe(t_list **cmds, t_params *params, char *line)
{
	int	fd[2];
	int	nb_wait;
	int	ret;
	t_list	*tmp;

	nb_wait = 0;
	init_fds(&fd[READ], &fd[WRITE]);
	init_fds(&((t_cmd *)(*cmds)->content)->fd[READ], &((t_cmd *)(*cmds)->content)->fd[WRITE]);
	while (cmds)
	{
		while (cmds && ((t_cmd *)(*cmds)->content)->nb)
		{
			ret = create_pipe(cmds, params, &nb_wait, line);
			if (ret)
				break ;
		}
		if (final_pipe(*cmds, params->env, fd))
			break ;
		reset_fds(fd);
		if (!((t_cmd *)(*cmds)->content)->nb)
			break ;
		tmp = (*cmds)->next;
		ft_lstdelone(*cmds, free_t_cmd);
		*cmds = tmp;
	}
	ft_wait(nb_wait);
	return (ret);
}
