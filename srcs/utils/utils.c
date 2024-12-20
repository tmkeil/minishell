/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:13:21 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/20 14:03:28 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	rl_redisplay();
}

void	configure_terminal(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		perror("tcgetattr");
		exit(EXIT_FAILURE);
	}
	term.c_lflag &= ~ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("tcsetattr");
		exit(EXIT_FAILURE);
	}
}

void	split_env_var(const char *env_var, char **name, char **value)
{
	char	*equal_sign;
	size_t	name_len;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		name_len = equal_sign - env_var;
		*name = malloc(name_len + 1);
		if (!*name)
			exit(EXIT_FAILURE);
		ft_memcpy(*name, env_var, name_len);
		(*name)[name_len] = '\0';
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*name = ft_strdup(env_var);
		*value = NULL;
	}
}

void	extract_envps(t_envs **envs, char **envp)
{
	char	*name;
	char	*value;
	t_envs	*current;
	int		i;
	t_envs	*new_node;

	current = NULL;
	i = 0;
	while (envp[i])
	{
		name = NULL;
		value = NULL;
		split_env_var(envp[i], &name, &value);
		new_node = malloc(sizeof(t_envs));
		if (!new_node)
			exit(EXIT_FAILURE);
		new_node->name = name;
		new_node->value = value;
		new_node->next = NULL;
		if (!*envs)
			*envs = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
}
