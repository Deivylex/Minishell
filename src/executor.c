/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dzurita <dzurita@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 16:18:20 by esalmela          #+#    #+#             */
/*   Updated: 2025/01/24 11:08:53 by dzurita          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	exec(char **cmd, char **env)
{
	static char	*path;
	int			p;
	int			error;

	error = 0;
	p = 0;
	if (ft_strchr(cmd[0], '/') != 0)
		path = cmd[0];
	else
		path = get_path(cmd[0], env, &p);
	if (path && p == 0)
		error = checkpath(path);
	if (error > 0)
	{
		if (p == -1)
			free(path);
		exit (error);
	}
	if (path)
		execve(path, cmd, env);
	if (p == -1)
		free (path);
	exit(error);
}

void	parent_process(t_data *cnt)
{
	int	i;
	int	status;

	status = 0;
	i = 0;
	while (i < cnt->i_pipex)
	{
		waitpid(cnt->exec->child[i], &status, 0);
		cnt->exit_status = WEXITSTATUS(status);
		i++;
	}
	close(cnt->exec->pipesfd[cnt->exec->currentfd - 2]);
	free(cnt->exec->child);
	free(cnt->exec->pipesfd);
	free(cnt->exec);
}

void	single_command(t_data *cnt, char **args)
{
	pid_t	child;
	int		status;

	receive_signal(2);
	if (cnt->here_doc_fd > 0)
	{
		dup2(cnt->here_doc_fd, STDIN);
		close(cnt->here_doc_fd);
	}
	child = fork();
	if (child == -1)
		exit (1);
	if (child == 0)
	{
		if (cnt->parse[0].rec_file)
			if (redirect(cnt, 0) < 0)
				exit (1);
		if (args)
			exec(args, cnt->env);
	}
	if (child != 0)
	{
		waitpid(child, &status, 0);
		cnt->exit_status = WEXITSTATUS(status);
	}
}

void	backup_stdou(t_data *cnt, int fd)
{
	if (fd == 0)
	{
		dup2(cnt->stdin_backup, fd);
		close(cnt->stdin_backup);
		cnt->stdin_backup = dup(fd);
		cnt->here_doc_fd = -1;
	}
	else
	{
		dup2(cnt->stdout_backup, fd);
		close(cnt->stdout_backup);
		cnt->stdout_backup = dup(fd);
	}
}

void	executor(t_data *cnt)
{
	int	i;

	i = -1;
	if (!cnt->parse)
		return ;
		
	check_here_doc(cnt);
	if (cnt->parse[0].cmd[0] && !cnt->parse[1].cmd
		&& check_built_in(cnt->parse[0].cmd) == TRUE)
	{
		run_builtin(cnt);
		backup_stdou(cnt, STDOUT);
		return ;
	}
	if (!cnt->parse[1].cmd)
		single_command(cnt, cnt->parse[0].cmd);
	if (cnt->i_pipex > 1)
	{
		cnt->exec = init_exec_struct(cnt->i_pipex - 1);
		while (++i < cnt->i_pipex)
			piping_and_forking(cnt, i);
		parent_process(cnt);
	}
	if (cnt->here_doc_fd > 0)
		backup_stdou(cnt, STDIN);
}
