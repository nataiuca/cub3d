/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:14:22 by root              #+#    #+#             */
/*   Updated: 2025/10/14 16:35:05 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Counts the number of lines in a given file
 * 			Opens the specified file, reads it line by line, and counts the
 * 			total lines. Closes the file before returning.
 *
 * @param file	Path to the file to count lines in
 * @return	The number of lines in the file, or 0 on failure
 */
static int	count_file_lines(char *file)
{
	int		fd;
	int		count;
	char	*line;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (error_msg(NULL, NULL, 0));
	count = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

/**
 * @brief	Reads the contents of a file into a info structure.
 * 			Reads each line of the file and stores it as a NULL-terminated
 * 			array of strings in info->file_raw_data. Strips newline
 * 			characters from the end of the line ⚠️ NO SÉ SI ESTO ES NECESARIO
 *
 * @param info	Pointer to the info structure where file data will be
 * 					stored.
 * @param count_lines	Number of lines to read (allocated array size)
 * @return	1 on success, 0 on failure
 */

static int	read_file(t_info *info, int count_lines)
{
	int		row;
	char	*line;
	ssize_t	len;

	info->file_raw_data = ft_calloc(count_lines + 1, sizeof(char *));
	if (!info->file_raw_data)
		return (error_msg(NULL, NULL, 0));
	row = 0;
	line = get_next_line(info->fd);
	while (line != NULL)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		info->file_raw_data[row] = ft_strdup(line);
		free(line);
		if (!info->file_raw_data[row])
			return (error_msg(NULL, NULL, 0));
		line = get_next_line(info->fd);
		row++;
	}
	info->file_raw_data[row] = NULL;
	return (1);
}

/**
 * @brief	Loads a map file into a t_info structure.
 * 			Opens the file, counts its lines, reads the file into memory,
 * 			and intitalizes info->cursor to point the beginning of the
 * 			loaded data.
 *
 * @param file Path to the map file to load.
 * @param info	Pointer to the t_info structure to store the file data
 * @return	1 on success, 0 on failure
 */

int	load_info(char *file, t_info *info)
{
	int	count_lines;

	info->fd = open(file, O_RDONLY);
	if (info->fd < 0)
		return (error_msg(NULL, NULL, 0));
	count_lines = count_file_lines(file);
	if (!count_lines)
		return (0);
	if (!read_file(info, count_lines))
	{
		close(info->fd);
		return (0);
	}
	info->cursor = info->file_raw_data;
	return (1);
}
