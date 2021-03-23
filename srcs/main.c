/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mciupek <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 13:41:53 by mciupek           #+#    #+#             */
/*   Updated: 2021/03/19 13:41:55 by mciupek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cmd(char **cmd)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid > 0)
    {
		waitpid(pid, &status, 0);
		kill(pid, SIGTERM);
	}
    else
    {
		if (execve(cmd[0], cmd, NULL) == -1)
			perror("shell");
		error(CMD_ERR);
	}
}

static void	get_path(t_cmd *cmd)
{
	char	*path;
	char	*bin;
	char	**path_split;
	int		i;

	path = ft_strdup(getenv("PATH"));
	if (!path)
		path = ft_strdup("/bin:/usr/local/bin:/usr/bin:/bin:/usr/local/sbin");
	if (cmd->cmd[0][0] != '/' && ft_strncmp(cmd->cmd[0], "./", 2))
	{
		path_split = ft_split(path, ':');
		free(path);
		i = 0;
		while (path_split[i])
		{
			bin = (char *)ft_calloc(sizeof(char), (ft_strlen(path_split[i]) + ft_strlen((cmd->cmd)[0]) + 2));
			if (!bin)
				break ;
			ft_strlcat(bin, path_split[i], ft_strlen(path_split[i]) + ft_strlen(bin) + 1);
			ft_strlcat(bin, "/",  + ft_strlen(bin) + 2);
			ft_strlcat(bin, (cmd->cmd)[0], ft_strlen((cmd->cmd)[0]) + ft_strlen(bin) + 1);
			if (!access(bin, F_OK))
				break ;
			free(bin);
			i++;
		}
		free_array(path_split);
		free(cmd->cmd[0]);
		cmd->cmd[0] = bin;
	}
	else
		free(path);
}

int			main(void)
{
    char	*line;
    //t_cmds	cmds;
    t_cmd	cmd;
	int		ret;

    write(1, "$> ", 3);
	ret = get_next_line(0, &line);
    while (ret > 0) 
    {
        cmd.cmd = ft_split(line, ' ');
        get_path(&cmd);
		printf("Command : %s\n", cmd.cmd[0]);
        if (!cmd.cmd[0])
            printf("Command not found\n");
        else if (is_built_in(cmd.cmd[0]) == 0)
            exec_cmd(cmd.cmd);
		else
			exec_built_in(cmd.cmd);
		write(1, "$> ", 3);
		free_array(cmd.cmd);
		free(line);
		ret = get_next_line(0, &line);
	}
	if (ret < 0)
		error(RD_ERR);
    free(line);
    return (0);
}
