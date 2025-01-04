/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 20:02:51 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/04 15:40:43 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_setup_heredoc(t_cmds *cmd)
{
	int     fd_pipe[2];
	char	*line;

    if (pipe(fd_pipe) == -1)
    {
        perror("pipe");
        return -1;
    }
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, cmd->heredoc_end, ft_strlen(line) + 1) == 0)
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

void ft_handle_redirections(t_cmds *cmd, int *in_fd)
{
    int fd;
    int flags;
    int heredoc_fd;

    if (cmd->heredoc_end)
    {
        heredoc_fd = ft_setup_heredoc(cmd);
        if (heredoc_fd == -1)
            return ;
        *in_fd = heredoc_fd;
    }
    if (cmd->input_file)
    {
        fd = open(cmd->input_file, O_RDONLY);
        if (*in_fd < 0)
            perror("open input_file");
        *in_fd = fd;
    }
    if (cmd->output_file)
    {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
        if (cmd->append)
            flags = O_WRONLY | O_CREAT | O_APPEND;
        fd = open(cmd->output_file, flags, 0644);
        if (fd < 0)
            perror("open error");
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}
