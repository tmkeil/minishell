/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 16:00:58 by tkeil            ###   ########.fr       */
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

void	ft_execute(t_lexems *lexems, char *cmd, char **envp)
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
		handle_lexem(args, i++, (char *)lexems->value, lexems->type);
		lexems = lexems->next;
	}
	args[i] = NULL;
	if (execve(cmd, args, envp) == -1)
		clean_args(args);
	clean_args(args);
}

int is_valid_env_name(char *name) {
    int i;

	i = 0;
    if (!name || !(ft_isalpha(name[0]) || name[0] == '_'))
        return 0;
    i = 1;
    while (name[i])
	{
        if (!ft_isalnum(name[i]) && name[i] != '_') {
            return 0;
        }
        i++;
    }
    return (1);
}

void update_env_var(const char *name, const char *value)
{
	if (setenv(name, value ? value : "", 1) == -1) {
        perror("setenv");
    }
}

void ft_handle_export(t_lexems *args, char **envp)
{
	t_lexems *current;
	char **values_to_set_env;

 	if (args == NULL || args->next == NULL || ft_strchr(args->next->value, '=') == NULL)
	{
        int i = 0;
        while (envp[i]) {
            ft_printf("%s\n", envp[i]);
            i++;
        }
        return;
    }
	current = args->next;
	while(current && ft_strchr(current->value, '='))
	{
		values_to_set_env = ft_split(current->value, '=');
		if (values_to_set_env[0] && is_valid_env_name(values_to_set_env[0]))
            update_env_var(values_to_set_env[0], values_to_set_env[1]);
		else
            ft_printf("export: `%s': not a valid identifier\n", current->value);
		clean_args(values_to_set_env);
		current = current->next;
	}
}

int	execute_commands(t_exec_table *exec_table, char **envp)
{
	int		i;
	char	*cmd;
	int		pid;

	i = 0;
	pid = 0;
	cmd = ft_getpath(exec_table->lexems[i]->value, envp);
	
	if (cmd)
	{
		pid = fork();
		if (pid == 0)
			ft_execute(exec_table->lexems[i], cmd, envp);
	}
	else if(ft_strncmp(exec_table->lexems[i]->value, "export",7) == 0)
	{
		ft_handle_export(exec_table->lexems[i], envp);
	}
	free(cmd);
	waitpid(pid, NULL, 0);
	return (2);
}
