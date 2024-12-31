/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrubinst <nrubinst@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:34:33 by nrubinst          #+#    #+#             */
/*   Updated: 2024/12/31 12:43:59 by nrubinst         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*create_line(char *buffer)
{
	int		i;
	char	*line;

	i = 0;
	if (buffer == NULL || *buffer == '\0')
		return (ft_free(&buffer), NULL);
	while (buffer[i] != '\0' && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		line = (char *)malloc(i + 2 * sizeof(char));
	else
		line = (char *)malloc(i + 1 * sizeof(char));
	if (line == NULL)
		return (NULL);
	i = 0;
	while (buffer[i] != '\0' && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*append_to_buffer(char *buffer, char *new_read)
{
	char	*temp;

	temp = buffer;
	buffer = ft_strjoin(buffer, new_read);
	if (buffer == NULL)
		return (ft_free(&temp), NULL);
	ft_free(&temp);
	return (buffer);
}

char	*read_to_buffer(int fd, char *buffer)
{
	char	*new_read;
	int		bytes_read;

	new_read = (char *)malloc(BUFFER_SIZE + 1 * sizeof(char));
	if (new_read == NULL)
		return (free(buffer), NULL);
	while (1)
	{
		bytes_read = read(fd, new_read, BUFFER_SIZE);
		if (bytes_read <= 0)
			break ;
		new_read[bytes_read] = '\0';
		buffer = append_to_buffer(buffer, new_read);
		if (buffer == NULL)
			return (free(new_read), NULL);
		if (ft_strchr(new_read, '\n') != NULL)
			break ;
	}
	free(new_read);
	if (bytes_read < 0)
		return ((free(buffer)), NULL);
	if (*buffer == '\0')
		return (free(buffer), NULL);
	return (buffer);
}

char	*update_buffer(char *buffer)
{
	int		i;
	char	*remains_buffer;

	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	if (buffer[i] == '\0')
	{
		free(buffer);
		return (NULL);
	}
	remains_buffer = ft_strdup(&buffer[i + 1]);
	if (remains_buffer == NULL)
	{
		ft_free(&buffer);
		return (NULL);
	}
	free(buffer);
	return (remains_buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*new_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		ft_free(&buffer);
		return (NULL);
	}
	if (buffer == NULL)
		buffer = ft_strdup("");
	if (buffer == NULL)
		return (NULL);
	buffer = read_to_buffer(fd, buffer);
	if (buffer == NULL)
		return (NULL);
	new_line = create_line(buffer);
	if (new_line == NULL)
		return (ft_free(&buffer), NULL);
	buffer = update_buffer(buffer);
	if (buffer == NULL)
		ft_free(&buffer);
	return (new_line);
}
