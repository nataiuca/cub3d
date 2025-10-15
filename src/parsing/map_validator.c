/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:37:05 by root              #+#    #+#             */
/*   Updated: 2025/10/15 16:12:05 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Measures the map dimensions based on map_raw
 * 			- Iterates through each line of map_raw to determine the map width
 * 			and height.
 * 			- Updates map->width with the length of the longest row
 * 			- Updates map->height with the total number of rows
 * 
 * @param info	Pointer to the t_info structure containing map_raw
 * @param map	Pointer to the t_map structure to store width and height values
 * @return	1
 */

int	measure_map(t_info *info, t_map *map)
{
	int		i;
	int		curr_width;

	i = 0;
	curr_width = 0;
	while (info->map_raw[i])
	{
		curr_width = (int)ft_strlen(info->map_raw[i]);
		if (curr_width > map->width)
			map->width = curr_width;
		map->height ++;
		i++;
	}
	return (1);
}

/**
 * @brief	Process a single line of the map, stores it and updates player
 * 			position and orientation
 * 			Checks each character of the line:
 * 			- Validates allowed characters ("01NSEW ")
 * 				* If the character is not valid, displays an error message
 * 				and returns 0
 * 			- If a player starting position ("NSEW") is found:
 * 				* Ensures only one player exists (if not, displays an error
 * 				message and returns 0)
 * 				* Sets player's position and direction and initializes players
 * 				orientation
 * 			- Copies the processed line into map->grid[row]
 * 
 * @param map	Pointer to the t_map structure
 * @param player	Pointer to the t_player structure
 * @param row	Row index to store the line
 * @param line	The map line string to process
 * @return	1 on success, 0 on error
 */

int	process_map_line(t_game *game, t_map *map, int row, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (!ft_strchr("01NSEWC ", line[i]))
			return (error_msg(ERR_CHAR_MAP, &line[i], 0));
		if (ft_strchr("NSEW", line[i]))
		{
			if (game->player->dir != '\0')
				return (error_msg(ERR_PLAYERS, NULL, 0));
			game->player->dir = line[i];
			game->player->pos_x = i + 0.5;
			game->player->pos_y = row + 0.5;
			init_player_orientation(game->player);
		}
		if (line[i] == 'C')
			game->sprite_count ++;
		i ++;
	}
	map->grid[row] = ft_strdup(line);
	if (!map->grid[row])
		return (error_msg(NULL, NULL, 0));
	return (1);
}

/**
 * @brief	Normalizes the map to a rectangular grid.
 * 			Pads shorter rows with spaces so that every row has the same length
 * 			as map->width. This ensures the map is rectangular.
 * 
 * @param map	Pointer to the t_map structure
 * @return	1 on success, 0 on memory allocation error
 */

static int	normalize_map(t_map *map)
{
	int		i;
	int		old_len;
	char	*new_line;

	i = 0;
	while (i < map->height)
	{
		old_len = ft_strlen(map->grid[i]);
		if (old_len < map->width)
		{
			new_line = malloc(map->width + 1);
			if (!new_line)
				return (error_msg(NULL, NULL, 0));
			ft_memset(new_line + old_len, ' ', map->width - old_len);
			ft_memcpy(new_line, map->grid[i], old_len);
			new_line[map->width] = '\0';
			free(map->grid[i]);
			map->grid[i] = new_line;
		}
		i++;
	}
	return (1);
}

/**
 * @brief	Checks if the map is fully enclosed by walls.
 * 			Iterates through the entire map grid:
 * 			- Walkable cells ("0NSEW") must not be on the map edges
 * 			- Walkable cells must not be adjacent to empty spaces (' ')
 * 			- If any of these conditions occurs, it means the map is open, so
 * 			an error message is displayed and 0 is returned.
 * 
 * @param map	Pointer to the t_map structure
 * @return	1 if map is closed, 0 if map is open (error)
 */

static int	is_map_closed(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (ft_strchr("0NSEWC", map->grid[y][x]))
			{
				if (y == 0 || y == map->height - 1
					|| x == 0 || x == map->width - 1)
					return (error_msg(ERR_OPEN_MAP, NULL, 0));
				if (map->grid[y - 1][x] == ' ' || map->grid[y + 1][x] == ' '
					|| map->grid[y][x - 1] == ' ' || map->grid[y][x + 1] == ' ')
					return (error_msg(ERR_OPEN_MAP, NULL, 0));
			}
			x ++;
		}
		y ++;
	}
	return (1);
}

/**
 * @brief	Validates the parsed map and initializes map structure.
 * 			- Measures the map dimensions (width and height)
 * 			- Allocates memory for map->grid
 * 			- Processes each map line and sets player position and orientation
 * 			- Normalizes the map to a rectangular shape
 * 			- Checks if the map is fully enclosed by walls
 * 			- Ensures there is a player in the map
 * 			- Checks for trailinf content after the map
 * 
 * @param game	Pointer to the main t_game structure
 * @param info	Pointer to the file info structure containing map_raw
 * @param map	Pointer to the t_map structure to fill
 * @return	1 on success, 0 if any error occurs
 */

int	validate_map(t_game *game, t_info *info, t_map *map)
{
	int	row;

	measure_map(info, map);
	map->grid = calloc(map->height + 1, sizeof(char *));
	if (!map->grid)
		return (error_msg(NULL, NULL, 0));
	row = 0;
	while (row < map->height)
	{
		if (!process_map_line(game, map, row, info->map_raw[row]))
			return (0);
		row ++;
	}
	if (!normalize_map(map))
		return (0);
	if (!is_map_closed(map))
		return (0);
	if (game->player->dir == '\0')
		return (error_msg(ERR_NO_PLAYER, NULL, 0));
	info->cursor += map->height;
	if (skip_empty_lines(info))
		return (error_msg(ERR_AFTER_MAP, *info->cursor, 0));
	return (1);
}
