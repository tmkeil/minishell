/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkeil <tkeil@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 15:44:02 by tkeil             #+#    #+#             */
/*   Updated: 2024/12/05 15:33:25 by tkeil            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	ft_free(char **ptr)
{
	if (*ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static char	*ft_process_line(char **rest)
{
	char	*line;
	char	*ptr_nl;
	char	*temp;

	if (!*rest || **rest == '\0')
		return (ft_free(rest), NULL);
	ptr_nl = ft_strchr(*rest, '\n');
	if (ptr_nl)
	{
		line = ft_substr(*rest, 0, ptr_nl - *rest + 1);
		if (!line)
			return (ft_free(rest), NULL);
		temp = ft_strdup(ptr_nl + 1);
		if (!temp)
			return (ft_free(&line), ft_free(rest), NULL);
		ft_free(rest);
		*rest = temp;
		return (line);
	}
	line = ft_strdup(*rest);
	return (ft_free(rest), line);
}

static char	*ft_free_and_return_null(char **rest)
{
	ft_free(rest);
	return (NULL);
}

static char	*ft_read_and_join(int fd, char **rest, char *buffer)
{
	ssize_t	bytes;
	char	*temp;

	while (!ft_strchr(*rest, '\n'))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes < 0)
			return (ft_free_and_return_null(rest));
		if (bytes == 0)
			break ;
		buffer[bytes] = '\0';
		temp = ft_strjoin(*rest, buffer);
		if (!temp)
			return (ft_free_and_return_null(rest));
		ft_free(rest);
		*rest = temp;
	}
	if (*rest)
		return (ft_process_line(rest));
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*rest;
	char		buffer[BUFFER_SIZE + 1];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!rest)
		rest = ft_strdup("");
	if (!rest)
		return (NULL);
	line = ft_read_and_join(fd, &rest, buffer);
	if (!line)
		ft_free(&rest);
	return (line);
}

//  int main(void)
//  {
//  	int		fd;
//  	char	*s;
//  	fd = open("test.txt", O_RDONLY);
//  	if (fd == -1)
//  		return (1);
//  	while ((s = get_next_line(fd)) != NULL)
//  		printf("%s", s);
//  	close(fd);
//  	return (0);
//  }