/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 16:17:49 by root              #+#    #+#             */
/*   Updated: 2025/10/11 16:46:33 by root             ###   ########.fr       */
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
	//return (error_msg(ERR_NO_MAP, NULL, 0));
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
		count ++;
	}
	return (count);
}

/**
 * @brief
 * 
 * @param
 * @return 0 si no es inicio de mapa, 1 si solo contiene caracteres de mapa e interpretamos como inicio de mapa
 */

int	is_map_start_line(const char *line)
{
	while (*line)
	{
		if (!ft_strchr("10NSEW", *line) && !ft_isspace(*line))
			return (0); //no es inicio del mapa
		line ++;
	}
	return (1); //solo contiene caracteres de mapa
}
