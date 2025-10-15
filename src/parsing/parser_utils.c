/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:17:49 by root              #+#    #+#             */
/*   Updated: 2025/10/15 16:14:36 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Skips empty lines.
 * 			- Iterates though the file lines, advancing the cursor while the
 * 			current line is empty (only contains spaces, tabs, or newline
 * 			characters).
 * 			- If non-empty line is found, the cursor is left pointing to it and
 * 			the function returns 1.
 * 			- If the end of the file is reached without finding a valid line
 * 			0 is returned
 *
 * @param info	Pointer to the t_info structure containing the file raw data
 * 				cursor
 * @return	1 if a valid line is found, 0 if no valid line is found
 *
 */

int	skip_empty_lines(t_info *info)
{
	char	*line;

	while (*info->cursor)
	{
		line = ft_strtrim(*info->cursor, " \t\n");
		if (!line)
			return (error_msg(NULL, NULL, 0));
		if (*line)
		{
			free(line);
			return (1);
		}
		free(line);
		info->cursor++;
	}
	return (0);
}

/**
 * @brief	Counts the number of consecutive non-empty lines in a string array.
 * 			Iterates through the given array of strings until it encounters
 * 			either a NULL pointer (end of array) or an empty line (indicating
 * 			the end of the map).
 *
 * @param array	The NULL-terminated array of strings to count
 * @return	The number of non-empty lines found
 */

int	count_rows(char **array)
{
	int	count;

	count = 0;
	while (array[count])
	{
		if (!*array[count])
			break ;
		count++;
	}
	return (count);
}

/**
 * @brief	Checks if a given line is the start of the map.
 * 			The function iterates through each character of the line.
 * 			If all non-space chars belong to the valid map char set, the line
 * 			is considered the start of the map. Otherwise, is not.
 *
 * @param line	Pointer to the input string to check
 * @return	0 if the line does not represent the start of the map, 1 if the line
 * 			contains only valid map chars	
 */

int	is_map_start_line(const char *line)
{
	while (*line)
	{
		if (!ft_strchr("10NSEW", *line) && !ft_isspace(*line))
			return (0);
		line++;
	}
	return (1);
}
