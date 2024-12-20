/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 16:00:14 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_size(t_lexems *lexes)
{
	size_t	i;

	i = 0;
	while (lexes)
	{
		if (lexes->type != PIPE)
			i++;
		lexes = lexes->next;
	}
	return (i);
}

void	ft_execute(t_lexems *lexems, char *cmd, char **envp, t_envs *envp_list)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (ft_size(lexems) + 1));
	if (!args)
		return ;
	i = 0;
	while (lexems)
	{
		args[i] = NULL;
		handle_lexem(args, i++, (char *)lexems->value, lexems->type, envp_list);
		lexems = lexems->next;
	}
	args[i] = NULL;
	if (execve(cmd, args, envp) == -1)
	{
		ft_clr(&args);
		return ;
	}
}

int	ft_check_builtin(t_lexems *lexems, char **envp, t_envs **envp_list)
{
	(void)envp;
	if (ft_changedir(lexems))
		return (1);
	if (ft_handle_export(lexems, envp_list))
		return (1);
	if (ft_unset(lexems, envp_list))
		return (1);
	return (0);
}

int	execute_commands(t_minishell **minishell, char **envp)
{
	int		i;
	char	*cmd;
	int		pid;
	bool	valid;

	i = 0;
	pid = 1;
	cmd = NULL;
	while ((*minishell)->table[i])
	{
		valid = false;
		if (ft_check_builtin((*minishell)->table[i], envp, &(*minishell)->envs))
		{
			i++;
			continue ;
		}
		cmd = ft_getpath((*minishell)->table[i]->value, envp);
		if (cmd)
		{
			pid = fork();
			if (pid == 0)
				ft_execute((*minishell)->table[i], cmd, envp,
					(*minishell)->envs);
			valid = true;
		}
		if (pid > 0)
		{
			i++;
			free(cmd);
			waitpid(pid, NULL, 0);
		}
		if (!valid)
		{
			return (printf("zsh: command not found: %s\n",
					(char *)(*minishell)->table[i - 1]->value), INVALID_CMD);
		}
	}
	return (0);
}
