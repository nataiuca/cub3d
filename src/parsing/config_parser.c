/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:21:52 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/14 16:25:03 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Parses a color configurtion line
 * 			- Removes leading/trailing withespaces and newlines via
 * 			ft_strtrim()
 * 			- Replaces the commas with spaces to normalize color argument
 * 			separation
 * 			- Stores the result
 * 			- Marks it as set
 *
 * @param line	The raw line containing a color configuration.
 * @param color_raw	Pointer to store the raw color string
 * @param color_set	Pointer to a bool that tracks if color was already set
 * @return	1 on succes, 0 on failure
 */

static int	parse_color(char *line, char **color_raw, bool *color_set)
{
	char	*trimmed;
	char	*processed;

	if (*color_set)
		return (error_msg(ERR_KEY, line, 0));
	trimmed = ft_strtrim(line, " \t\n");
	if (!trimmed)
		return (error_msg(NULL, NULL, 0));
	processed = ft_strreplace(trimmed, ',', ' ');
	free(trimmed);
	if (!processed)
		return (error_msg(NULL, NULL, 0));
	*color_raw = processed;
	*color_set = true;
	return (1);
}

/**
 * @brief	Extracts and trims a texture path from a configuration line.
 * 			- Removes leading/trailing whitespaces and newlines
 * 			- Memory for the trimmed string must be freed by the caller
 *
 * @param line	Raw configuration line containing the texture path
 * @param dst	Pointer to where the trimmed texture path will be stored
 * @return 1 in success, 0 on failure (memory allocation error)
 */

static int	extract_texture_path(char *line, char **dst)
{
	*dst = ft_strtrim(line, " \t\n");
	if (!*dst)
		return (error_msg(NULL, NULL, 0));
	return (1);
}

/**
 * @brief	Processes a configuration line and updates the game configuration
 * 			- Identifies the configuration key (textures or colors)
 * 			- For texture keys ("NO", "SO", "EA", "WE"), extracts the texture
 * 			path and stores it in map structure
 * 			- For color keys ("C", "F"), extracts and stores the raw color
 * 			in info structure
 * 			- Validates that keys are not repeated and ensures required
 * 			configuration keys are present.
 *
 * @param line	Configuration line to process
 * @param info	Pointer to the game structure to update
 * @param configs_found	Number of configuration keys already processed
 * @return 1 in success, 0 on failure (missing keys, invalid key, or memory
 * 			error)
 */

static int	process_line(char *line, t_map *map, t_info *info, int keys_found)
{
	if (ft_strncmp(line, "NO ", 3) == 0 && !map->no_texture)
		return (extract_texture_path(line + 3, &map->no_texture));
	else if (ft_strncmp(line, "SO ", 3) == 0 && !map->so_texture)
		return (extract_texture_path(line + 3, &map->so_texture));
	else if (ft_strncmp(line, "EA ", 3) == 0 && !map->ea_texture)
		return (extract_texture_path(line + 3, &map->ea_texture));
	else if (ft_strncmp(line, "WE ", 3) == 0 && !map->we_texture)
		return (extract_texture_path(line + 3, &map->we_texture));
	else if (ft_strncmp(line, "C ", 2) == 0 && !info->c_color_set)
		return (parse_color(line + 2, &info->c_color_raw, &info->c_color_set));
	else if (ft_strncmp(line, "F ", 2) == 0 && !info->f_color_set)
		return (parse_color(line + 2, &info->f_color_raw, &info->f_color_set));
	if (keys_found < 6 && is_map_start_line(line))
		return (error_msg(ERR_MISSING_CONF_KEYS, NULL, 0));
	return (error_msg(ERR_KEY, line, 0));
}

/**
 * @brief	Parses configuration lines in the map file.
 * 			- Reads and processes configuration keys (textures and colors)
 * 			until all required keys are found
 * 			- Skips empty lines and trims whitespace before processing
 * 			- Uses handle_config_line() to process each valid line
 *
 * @param game	Pointer to the game main structure
 * @param info	Poinetr to the info structure containing raw file data
 * @return	1 in success, 0 on failure (missing keys or invalid configuration
 * 			line)
 */

int	parse_config(t_game *game, t_info *info)
{
	int		keys_found;
	char	*line;

	keys_found = 0;
	while (*info->cursor && keys_found < 6)
	{
		if (!skip_empty_lines(info) && keys_found == 6)
			return (error_msg(ERR_NO_MAP, NULL, 0));
		if (!skip_empty_lines(info) && keys_found != 6)
			return (error_msg(ERR_MISSING_CONF_KEYS, NULL, 0));
		line = ft_strtrim(*info->cursor, " \t\n");
		if (!line)
			return (error_msg(NULL, NULL, 0));
		if (*line)
		{
			if (process_line(line, game->map, info, keys_found))
				free(line);
			else
				return (free(line), 0);
			keys_found++;
		}
		info->cursor++;
	}
	return (1);
}
