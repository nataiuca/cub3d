/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 15:04:08 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/15 16:02:18 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Checks for corner collisions around the player.
 * 			Prevents diagonal wall clipping by testing four surrounding
 * 			corner cells with a safety margin.
 *
 * @param game	Pointer to the main game structure.
 * @param x		Target X position to validate.
 * @param y		Target Y position to validate.
 * @param margin	Small offset applied to each corner for collision padding.
 *
 * @return	1 if all corners are free of walls, 0 otherwise.
 */

static int	check_corners(t_game *game, double x, double y, double margin)
{
	if (game->map->grid[(int)(y - margin)][(int)(x - margin)] == '1')
		return (0);
	if (game->map->grid[(int)(y - margin)][(int)(x + margin)] == '1')
		return (0);
	if (game->map->grid[(int)(y + margin)][(int)(x - margin)] == '1')
		return (0);
	if (game->map->grid[(int)(y + margin)][(int)(x + margin)] == '1')
		return (0);
	return (1);
}

/**
 * @brief	Validates a given position within the game map.
 * 			Checks whether the target coordinates correspond to a walkable
 * 			tile and ensures that nearby walls are not clipped through.
 *
 * @param game	Pointer to the main game structure.
 * @param x		X coordinate of the position to test.
 * @param y		Y coordinate of the position to test.
 *
 * @return	1 if the position is valid and not obstructed, 0 otherwise.
 */

int	is_valid_pos(t_game *game, double x, double y)
{
	int		map_x;
	int		map_y;
	double	margin;
	char	tile;

	margin = 0.3;
	map_x = (int)x;
	map_y = (int)y;
	if (map_x < 0 || map_x >= game->map->width
		|| map_y < 0 || map_y >= game->map->height)
		return (0);
	tile = game->map->grid[map_y][map_x];
	if (tile != '0' && tile != 'N' && tile != 'S'
		&& tile != 'E' && tile != 'W')
		return (0);
	return (check_corners(game, x, y, margin));
}
