/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 17:48:13 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/18 20:25:45 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char *construct_env_var(const char *name, const char *value)
{
	size_t name_len;
    size_t value_len;
	size_t total_len;
    char *result;

	name_len = ft_strlen(name);
	if (value)
        value_len = ft_strlen(value);
    else
        value_len = 0;
	total_len = name_len + value_len + 2;
	result = malloc(total_len);
    if (!result)
        exit(EXIT_FAILURE);
    ft_strlcpy(result, name, total_len);
    ft_strlcat(result, "=", total_len);
    if (value) {
        ft_strlcat(result, value, total_len);
    }
    return result;
}

void update_env_var(const char *name, const char *value, char ***envp)
{
	int i;
	size_t name_len;
	char **new_envp;
	int envp_old_size;

	name_len = ft_strlen(name);
	i = 0;
	while((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], name, name_len) == 0 && (*envp)[i][name_len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = construct_env_var(name, value);
			return ;
		}
		i++;
	}
	envp_old_size = i;
	new_envp = malloc(envp_old_size + 2 * sizeof(char *));
	i = 0;
	while(i < envp_old_size)
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	(*envp)[envp_old_size] = construct_env_var(name, value);
	new_envp[envp_old_size + 1] = NULL;
	*envp = new_envp;
}

int	ft_handle_export(t_lexems *lexems, char **envp)
{
	t_lexems *current;
	char **values_to_set_env;
	int	i;

	if (ft_strncmp(lexems->value, "export", 7) == 0)
	{
		if (!lexems || !lexems->next || !ft_strchr(lexems->next->value, '='))
		{
			i = 0;
			while (envp[i])
				ft_printf("%s\n", envp[i++]);
		}
		current = lexems->next;
		while(current && ft_strchr(current->value, '='))
		{
			values_to_set_env = ft_split(current->value, '=');
			if (values_to_set_env[0] && is_valid_env_name(values_to_set_env[0]))
				update_env_var(values_to_set_env[0], values_to_set_env[1], &envp);
			else
				ft_printf("export: `%s': not a valid identifier\n", current->value);
			clean_args(values_to_set_env);
			current = current->next;
		}
		return (1);
	}
	return (0);
}

int	ft_changedir(t_lexems *lexems)
{
	char	*home;

	home = NULL;
	if (lexems->type == WORD && !ft_strncmp((char *)lexems->value, "cd", 2))
	{
		if (lexems->next)
		{
			if (chdir((char *)lexems->next->value) != 0)
				printf("cd: no such file or directory: %s\n",
					(char *)lexems->next->value);
		}
		else
		{
			home = getenv("HOME");
			if (chdir(home) != 0)
				printf("cd: no such file or directory: %s\n", home);
		}
		return (1);
	}
	return (0);
}

int	ft_unset(t_lexems *lexems, char **envp)
{
	(void)lexems;
	(void)envp;
	return (0);
}
