/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 12:43:55 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/11 19:52:09 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# define BAD_PIPE 0
# define BAD_FORK 1
# define BAD_FD 2
# define BAD_ALLOCATION 3
# define BAD_EXECUTE 4
# define BAD_DUP 5
# define BAD_WAITPID 6

# include "libft.h"
# include <string.h>
# include <sys/wait.h>

void	ft_clr(char ***ptr);
void	ft_error(int errnum);
char	*ft_getpath(char *cmd, char **envp);
void	ft_execute(char *argv, char **envp);

// bonus
void	ft_dup2(int s, int d, int *fd);
void	ft_close_pipe(int *fd);
void	ft_set_out(char *argv, int *fd);
void	ft_set_in(char *argv, int *fd);

#endif
