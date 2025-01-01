/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 20:02:51 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/01 15:27:37 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_redirections(t_lexems *lexem)
// {
// 	int	fd;
// 	if (lexem->type == SEPERATOR)
// 		lexem = lexem->next;
// 	while (lexem)
// 	{
// 		if (lexem->type == SEPERATOR)
// 			lexem = lexem->next;
// 		if (!lexem)
// 			break ;
// 		if (lexem->type == IN_REDIRECT)
// 		{
// 			fd = open((char *)lexem->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			if (fd == -1)
// 				perror("open error");
// 			dup2(fd, STDIN_FILENO);
// 			close(fd);
// 		}
// 		else if (lexem->type == APPEND)
// 		{
// 			fd = open((char *)lexem->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			if (fd == -1)
// 				perror("open error");
// 			dup2(fd, STDOUT_FILENO);
// 			close(fd);
// 		}
// 		else if (lexem->type == OUT_REDIRECT)
// 		{
// 			fd = open((char *)lexem->value, O_RDONLY);
// 			if (fd == -1)
// 				perror("open error");
// 			dup2(fd, STDIN_FILENO);
// 			close(fd);
// 		}
// 		lexem = lexem->next;
// 	}
// }

// void	ft_heredoc(t_lexems *lexem)
// {
// 	int		fd;
// 	char	*line;

// 	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 		perror("open error");
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, delimiter) == 0)
// 			break;
// 		write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		free(line);
// 	}
// 	free(line);
// 	close(fd);
// 	fd = open(".heredoc_tmp", O_RDONLY);
// 	if (fd == -1)
// 		perror("open error");
// 	dup2(fd, STDIN_FILENO);
// 	close(fd);
// 	unlink(".heredoc_tmp");
// }
