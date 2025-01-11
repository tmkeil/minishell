/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 13:49:32 by tkeil             #+#    #+#             */
/*   Updated: 2025/01/11 14:57:02 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	ft_close_fd(int *fd)
// {
// 	if (*fd != -1)
// 	{
// 		close(*fd);
// 		*fd = -1;
// 	}
// }

// int	ft_run_builtin(t_minishell **minishell, t_cmds **cmd, int *fd_in,
// 		int *fd_pipe)
// {
// 	int		status;
// 	char	*cmd_builtin;

// 	(void)fd_in;
// 	(void)fd_pipe;
// 	status = -1;
// 	cmd_builtin = ft_is_builtin((*cmd)->cmd);
// 	if (!cmd_builtin)
// 		return (-1);
// 	status = ft_choose_builtin(minishell, cmd_builtin, (*cmd)->args);
// 	free(cmd_builtin);
// 	(*minishell)->exit_status = status;
// 	return (status);
// }

// void	ft_child(t_minishell **minishell, t_cmds *cmd, int fd_in, int *fd_pipe)
// {
// 	char	*path;

// 	ft_set_execution_sig();
// 	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next != NULL));
// 	ft_handle_redirections(cmd, &fd_in);
// 	if (fd_in != -1)
// 	{
// 		dup2(fd_in, STDIN_FILENO);
// 		close(fd_in);
// 		fd_in = -1;
// 	}
// 	if (cmd->next)
// 	{
// 		if (fd_pipe[0] != -1)
// 			close(fd_pipe[0]);
// 	}
// 	else
// 	{
// 		if (fd_pipe[1] != -1)
// 			close(fd_pipe[1]);
// 	}
// 	if (ft_run_builtin(minishell, &cmd, &fd_in, fd_pipe) >= 0)
// 		exit((*minishell)->exit_status);
// 	path = ft_getpath(cmd->cmd, (*minishell)->envps, true);
// 	if (!path)
// 	{
// 		ft_putstr_fd("bash: ", STDERR_FILENO);
// 		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
// 		ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 		exit(INVALID_CMD);
// 	}
// 	execve(path, cmd->args, (*minishell)->envps);
// 	perror("execve");
// 	exit(EXIT_FAILURE);
// }

// void	ft_execute_external(t_minishell **minishell, t_cmds *cmd, int fd_in,
// 		int *fd_pipe)
// {
// 	int		status;
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == 0)
// 		ft_child(minishell, cmd, fd_in, fd_pipe);
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			(*minishell)->exit_status = WEXITSTATUS(status);
// 		if (fd_pipe[1] != -1)
// 		{
// 			close(fd_pipe[1]);
// 			fd_pipe[1] = -1;
// 		}
// 	}
// }

// int	ft_execute_commands(t_minishell **minishell)
// {
// 	int		fd_pipe[2];
// 	int		fd_in;
// 	t_cmds	*current;

// 	fd_in = -1;
// 	current = (*minishell)->cmds;
// 	if (!current->next)
// 	{
// 		fd_pipe[0] = -1;
// 		fd_pipe[1] = -1;
// 		if (ft_is_builtin(current->cmd))
// 		{
// 			ft_handle_redirections(current, &fd_in);
// 			ft_run_builtin(minishell, &current, &fd_in, fd_pipe);
// 			if (fd_in != -1)
// 				close(fd_in);
// 		}
// 		else
// 		{
// 			ft_execute_external(minishell, current, fd_in, fd_pipe);
// 			if (fd_in != -1)
// 			{
// 				close(fd_in);
// 				fd_in = -1;
// 			}
// 		}
// 		return (1);
// 	}
// 	while (current)
// 	{
// 		if (current->next)
// 			pipe(fd_pipe);
// 		else
// 		{
// 			fd_pipe[0] = -1;
// 			fd_pipe[1] = -1;
// 		}
// 		if (ft_is_builtin(current->cmd))
// 		{
// 			ft_redirect_pipe(fd_in, fd_pipe, (current->next != NULL));
// 			ft_handle_redirections(current, &fd_in);
// 			ft_run_builtin(minishell, &current, &fd_in, fd_pipe);
// 			if (fd_pipe[1] != -1)
// 			{
// 				close(fd_pipe[1]);
// 				fd_pipe[1] = -1;
// 			}
// 			ft_close_fd(&fd_in);
// 			fd_in = fd_pipe[0];
// 			dup2((*minishell)->in_fd, STDIN_FILENO);
// 			dup2((*minishell)->out_fd, STDOUT_FILENO);
// 		}
// 		else
// 		{
// 			ft_execute_external(minishell, current, fd_in, fd_pipe);
// 			if (fd_in != -1)
// 			{
// 				close(fd_in);
// 				fd_in = -1;
// 			}
// 			fd_in = fd_pipe[0];
// 		}
// 		current = current->next;
// 	}
// 	return (1);
// }


























// void	ft_close_fd(int *fd)
// {
// 	if (*fd != -1)
// 	{
// 		close(*fd);
// 		*fd = -1;
// 	}
// }

// int	ft_run_builtin(t_minishell **minishell, t_cmds **cmd, int *fd_in, int *fd_pipe)
// {
// 	int		status;

// 	status = -1;
// 	if (!ft_strnstr(BUILTINS, (*cmd)->cmd, ft_strlen(BUILTINS)))
// 		return (-1);
// 	ft_redirect_pipe(*fd_in, fd_pipe, ((*cmd)->next != NULL));
// 	ft_handle_redirections(*cmd, fd_in);
// 	status = ft_choose_builtin(minishell, (*cmd)->cmd, (*cmd)->args);
// 	(*minishell)->exit_status = status;
// 	dup2((*minishell)->in_fd, STDIN_FILENO);
// 	dup2((*minishell)->out_fd, STDOUT_FILENO);
// 	ft_close_fd(fd_in);
// 	return (status);
// }

// void	ft_child(t_minishell **minishell, t_cmds *cmd, int fd_in, int *fd_pipe)
// {
// 	char	*path;

// 	ft_set_execution_sig();
// 	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next != NULL));
// 	ft_handle_redirections(cmd, &fd_in);
// 	if (cmd->next)
// 		ft_close_fd(&fd_pipe[0]);
// 	else
// 		ft_close_fd(&fd_pipe[1]);
// 	if (ft_run_builtin(minishell, &cmd, &fd_in, fd_pipe) >= 0)
// 		exit((*minishell)->exit_status);
// 	path = ft_getpath(cmd->cmd, (*minishell)->envps, true);
// 	if (!path)
// 	{
// 		ft_put_error_str("bash: ", cmd->cmd);
// 		ft_putstr_fd(": command not found\n", STDERR_FILENO);
// 		exit(INVALID_CMD);
// 	}
// 	execve(path, cmd->args, (*minishell)->envps);
// 	exit(EXIT_FAILURE);
// }

// void	ft_execute_external(t_minishell **minishell, t_cmds *cmd, int *fd_in,
// 		int *fd_pipe)
// {
// 	int		status;
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == 0)
// 		ft_child(minishell, cmd, *fd_in, fd_pipe);
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			(*minishell)->exit_status = WEXITSTATUS(status);
// 		ft_close_fd(&fd_pipe[1]);
// 		ft_close_fd(fd_in);
// 	}
// }

// void	ft_execute_command_in_pipeline(t_minishell **minishell,
// 			t_cmds **current, int *fd_in)
// {
// 	int	fd_pipe[2];

// 	if ((*current)->next)
// 	{
// 		if (pipe(fd_pipe) == -1)
// 			return ;
// 	}
// 	else
// 	{
// 		fd_pipe[0] = -1;
// 		fd_pipe[1] = -1;
// 	}
// 	if (ft_run_builtin(minishell, current, fd_in, fd_pipe) != -1)
// 	{
// 		ft_close_fd(&fd_pipe[1]);
// 		ft_close_fd(fd_in);
// 		*fd_in = fd_pipe[0];
// 		dup2((*minishell)->in_fd, STDIN_FILENO);
// 		dup2((*minishell)->out_fd, STDOUT_FILENO);
// 	}
// 	else
// 	{
// 		ft_execute_external(minishell, *current, fd_in, fd_pipe);
// 		ft_close_fd(fd_in);
// 		*fd_in = fd_pipe[0];
// 	}
// }

// int	ft_execute_pipeline(t_minishell **minishell, t_cmds *current)
// {
// 	int	fd_in;

// 	fd_in = -1;
// 	while (current)
// 	{
// 		ft_execute_command_in_pipeline(minishell, &current, &fd_in);
// 		current = current->next;
// 	}
// 	return (1);
// }

// int	ft_execute_single_command(t_minishell **minishell, t_cmds *cmds)
// {
// 	int	fd_in;
// 	int	fd_pipe[2];

// 	fd_in = -1;
// 	fd_pipe[0] = -1;
// 	fd_pipe[1] = -1;
// 	if (ft_run_builtin(minishell, &cmds, &fd_in, fd_pipe) == -1)
// 		ft_execute_external(minishell, cmds, &fd_in, fd_pipe);
// 	ft_close_fd(&fd_pipe[1]);
// 	ft_close_fd(&fd_pipe[0]);
// 	ft_close_fd(&fd_in);
// 	return (1);
// }

// int	ft_execute_commands(t_minishell **minishell)
// {
// 	t_cmds	*current;

// 	current = (*minishell)->cmds;
// 	if (!(*minishell)->cmds->next)
// 		return (ft_execute_single_command(minishell, current));
// 	return (ft_execute_pipeline(minishell, current));
// }





























/* -------------------------------------------------------- */
/*                HILFSFUNKTIONEN                          */
/* -------------------------------------------------------- */

/*
 * Schließt nur, wenn *fd != -1.
 */
void	ft_close_fd(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}

/*
 * Prüft, ob `cmd->cmd` (also der Befehl) einer unserer Builtins ist.
 * Du verwendest irgendwo ein Macro BUILTINS + ft_strnstr(...).
 * Hier sehr vereinfacht:
 */
bool	is_builtin_in_parent(char *cmd)
{
	// Beispiel: cd, export, unset => Parent-Builtins
	// Du kannst hier entscheiden, WELCHE Builtins im Parent laufen sollen.
	if (!ft_strncmp(cmd, "cd", 3))
		return (true);
	if (!ft_strncmp(cmd, "export", 7))
		return (true);
	if (!ft_strncmp(cmd, "unset", 6))
		return (true);
	// => Rest NICHT
	return (false);
}

bool	is_builtin_in_child(char *cmd)
{
	// Beispiel: echo, env, pwd => Child-Builtins
	if (!ft_strncmp(cmd, "echo", 5))
		return (true);
	if (!ft_strncmp(cmd, "env", 4))
		return (true);
	if (!ft_strncmp(cmd, "pwd", 4))
		return (true);
	// => Rest NICHT
	return (false);
}

/*
 * Führt den Builtin "cmd" mit den Argumenten aus.
 * Hier nutzt du ft_choose_builtin(...).
 */
int	run_the_builtin(t_minishell **ms, char *cmd_builtin, char **args)
{
	return (ft_choose_builtin(ms, cmd_builtin, args));
}

/* -------------------------------------------------------- */
/*       DATEI-FD, PIPE & HEREDOC UMLEITUNGEN               */
/* -------------------------------------------------------- */

/*
 * Leitet fd_in -> STDIN und ggf. fd_pipe[1] -> STDOUT um,
 * falls is_next (d. h. pipe existiert).
 */
void	ft_redirect_pipe(int fd_in, int *fd_pipe, bool is_next)
{
	if (fd_in != -1)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (is_next && fd_pipe[1] != -1)
	{
		dup2(fd_pipe[1], STDOUT_FILENO);
		ft_close_fd(&fd_pipe[1]);
	}
}

/*
 * Liest den Heredoc in pipefd[1], gibt pipefd[0] zurück.
 */
int	ft_setup_heredoc(t_cmds *cmd)
{
	int		fd_pipe[2];
	char	*line;

	if (pipe(fd_pipe) == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, cmd->heredoc, ft_strlen(line) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd_pipe[1]);
		ft_putstr_fd("\n", fd_pipe[1]);
		free(line);
	}
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}

/*
 * Hier fügen wir Heredoc & Input/Output zusammen.
 * => *in_fd am Ende NICHT sofort schließen, 
 *    wenn wir BUILTIN im Child ausführen wollen.
 */
void	ft_handle_redirections(t_cmds *cmd, int *in_fd)
{
	int fd;
	int flags;

	// 1) Heredoc
	if (cmd->heredoc)
	{
		int heredoc_fd = ft_setup_heredoc(cmd);
		if (heredoc_fd != -1)
			*in_fd = heredoc_fd;
	}

	// 2) Input File
	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd < 0)
			perror("open input_file");
		else
			*in_fd = fd;
	}

	// 3) Output File
	if (cmd->output_file)
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		if (cmd->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		fd = open(cmd->output_file, flags, 0644);
		if (fd < 0)
			perror("open output_file");
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}

/* -------------------------------------------------------- */
/*       BUILTINS IM PARENT / CHILD UNTERSCHIEDLICH         */
/* -------------------------------------------------------- */

/*
 * Führt Builtin im PARENT aus, + Umleitungen
 * => hinterher Standard-FDs zurücksetzen
 */
int	ft_builtin_in_parent(t_minishell **ms, t_cmds **cmd,
						 int *fd_in, int *fd_pipe)
{
	int	status;

	// Pipe & Redirections
	ft_redirect_pipe(*fd_in, fd_pipe, ((*cmd)->next != NULL));
	ft_handle_redirections(*cmd, fd_in);

	status = run_the_builtin(ms, (*cmd)->cmd, (*cmd)->args);

	// Status übernehmen
	(*ms)->exit_status = status;

	// Standard-FDs zurücksetzen
	dup2((*ms)->in_fd, STDIN_FILENO);
	dup2((*ms)->out_fd, STDOUT_FILENO);

	// Altes fd_in schließen
	ft_close_fd(fd_in);

	// evtl. fd_pipe[1] schließen
	if (fd_pipe[1] != -1)
		ft_close_fd(&fd_pipe[1]);

	return status;
}

/*
 * Führt Builtin IM CHILD aus (falls is_builtin_in_child == true).
 * => Wir gehen davon aus, dass wir HIER im Child schon
 *    ft_redirect_pipe & ft_handle_redirections gemacht haben.
 */
int	ft_builtin_in_child(t_minishell **ms, t_cmds **cmd, int *fd_in, int *fd_pipe)
{
	int status;

	(void)fd_in;
	(void)fd_pipe;
	status = run_the_builtin(ms, (*cmd)->cmd, (*cmd)->args);
	(*ms)->exit_status = status;
	return status;
}

/* -------------------------------------------------------- */
/*                        CHILD-FUNKTION                    */
/* -------------------------------------------------------- */

/*
 * Im Child:
 * 1) Pipe => STDIN/STDOUT
 * 2) Redirections => Heredoc, input_file, output_file
 * 3) builtin_in_child, falls is_builtin_in_child
 * 4) extern -> exec
 */
void	ft_child(t_minishell **ms, t_cmds *cmd, int fd_in, int *fd_pipe)
{
	char	*path;
	int		status;

	ft_set_execution_sig();

	// 1) Pipe & Redirections
	ft_redirect_pipe(fd_in, fd_pipe, (cmd->next != NULL));
	ft_handle_redirections(cmd, &fd_in);

	// 2) Schließen unbenötigter Pipe-Enden
	if (cmd->next && fd_pipe[0] != -1)
		close(fd_pipe[0]);
	else if (!cmd->next && fd_pipe[1] != -1)
		close(fd_pipe[1]);

	// 3) ggf. Builtin im Child?
	if (is_builtin_in_child(cmd->cmd))
	{
		status = ft_builtin_in_child(ms, &cmd, &fd_in, fd_pipe);
		exit(status);
	}

	// 4) Externer Command
	path = ft_getpath(cmd->cmd, (*ms)->envps, true);
	if (!path)
	{
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(cmd->cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(INVALID_CMD);
	}
	execve(path, cmd->args, (*ms)->envps);
	exit(EXIT_FAILURE);
}

/* -------------------------------------------------------- */
/*           EXTERNES KOMMANDO (ELTERNTEIL)                */
/* -------------------------------------------------------- */

void	ft_execute_external(t_minishell **ms, t_cmds *cmd, int *fd_in, int *fd_pipe)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		// CHILD
		ft_child(ms, cmd, *fd_in, fd_pipe);
	}
	else
	{
		// PARENT
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*ms)->exit_status = WEXITSTATUS(status);

		// Schreib-Ende schließen
		if (fd_pipe[1] != -1)
			ft_close_fd(&fd_pipe[1]);

		// Altes fd_in schließen
		ft_close_fd(fd_in);
	}
}

/* -------------------------------------------------------- */
/*       EIN COMMAND (OHNE PIPE) -> BUILTIN ODER EXTERN     */
/* -------------------------------------------------------- */

int	ft_execute_single_command(t_minishell **ms, t_cmds *cmd)
{
	int	fd_pipe[2];
	int	fd_in;
	int	status;

	fd_in = -1;
	fd_pipe[0] = -1;
	fd_pipe[1] = -1;

	// a) Prüfen, ob builtin im Parent
	if (is_builtin_in_parent(cmd->cmd))
	{
		status = ft_builtin_in_parent(ms, &cmd, &fd_in, fd_pipe);
		return (1);
	}

	// b) Extern oder Child-Builtin
	ft_execute_external(ms, cmd, &fd_in, fd_pipe);
	return (1);
}

/* -------------------------------------------------------- */
/*       MEHRERE KOMMANDOS (PIPE) -> BUILTIN/EXTERN        */
/* -------------------------------------------------------- */

static void	ft_execute_command_in_pipeline(t_minishell **ms,
			t_cmds **cmd, int *fd_in)
{
	int	fd_pipe[2];

	if ((*cmd)->next)
	{
		if (pipe(fd_pipe) == -1)
		{
			perror("pipe");
			return ;
		}
	}
	else
	{
		fd_pipe[0] = -1;
		fd_pipe[1] = -1;
	}

	// a) builtin in PARENT?
	if (is_builtin_in_parent((*cmd)->cmd))
	{
		ft_builtin_in_parent(ms, cmd, fd_in, fd_pipe);
		*fd_in = fd_pipe[0];
	}
	else
	{
		// => extern oder builtin_in_child
		ft_execute_external(ms, *cmd, fd_in, fd_pipe);
		*fd_in = fd_pipe[0];
	}
}

/*
 * Geht über alle Commands in der Pipeline, ruft pro Command
 *   ft_execute_command_in_pipeline(...) auf.
 */
int	ft_execute_pipeline(t_minishell **ms, t_cmds *current)
{
	int	fd_in;

	fd_in = -1;
	while (current)
	{
		ft_execute_command_in_pipeline(ms, &current, &fd_in);
		current = current->next;
	}
	return (1);
}

/* -------------------------------------------------------- */
/*         HAUPTFUNKTION: ft_execute_commands              */
/* -------------------------------------------------------- */

int	ft_execute_commands(t_minishell **ms)
{
	t_cmds	*current;

	current = (*ms)->cmds;
	if (!current->next)
		return (ft_execute_single_command(ms, current));
	return (ft_execute_pipeline(ms, current));
}
