/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 20:02:51 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/13 18:18:52 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_setup_heredoc(t_cmds *cmd)
{
	char	*line;
	int		fd_pipe[2];

	if (pipe(fd_pipe) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, cmd->heredoc, ft_strlen(line) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(fd_pipe[1], line, ft_strlen(line));
		write(fd_pipe[1], "\n", 1);
		free(line);
	}
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}

void	ft_redirect_pipe(int *fd_in, int *fd_pipe, bool is_next)
{
	if (*fd_in != -1)
	{
		dup2(*fd_in, STDIN_FILENO);
		ft_close_fd(fd_in);
	}
	if (is_next && fd_pipe[1] != -1)
	{
		dup2(fd_pipe[1], STDOUT_FILENO);
		ft_close_fd(&fd_pipe[1]);
	}
}

static void	ft_redirect_in_out(t_cmds *cmd, int *in_fd)
{
	int	fd;
	int	flags;

	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd < 0)
			ft_sterr("bash: ", cmd->input_file, ": No such file or directory");
		else
			*in_fd = fd;
	}
	else if (cmd->output_file)
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		if (cmd->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(cmd->output_file, flags, 0644);
		if (fd < 0)
			ft_sterr("bash: ", cmd->output_file, ": No such file or directory");
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}

void	ft_handle_redirections(t_cmds *cmd, int *in_fd)
{
	int	heredoc_fd;

	if (cmd->heredoc)
	{
		heredoc_fd = ft_setup_heredoc(cmd);
		if (heredoc_fd == -1)
			return ;
		*in_fd = heredoc_fd;
	}
	if (cmd->input_file || cmd->output_file)
		ft_redirect_in_out(cmd, in_fd);
	if (*in_fd > 0)
	{
		dup2(*in_fd, STDIN_FILENO);
		close(*in_fd);
		*in_fd = -1;
	}
}
