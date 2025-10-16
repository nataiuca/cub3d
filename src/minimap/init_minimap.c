/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 17:17:04 by root              #+#    #+#             */
/*   Updated: 2025/10/16 11:05:28 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Initilializes the minimap structure.
 * 			- Calculates the cell size (tile size) based on the map dimensions 
 * 			and the MAX_MINIMAP_SIZE constraint. The smaller of the width or 
 * 			height cell size is chosen to ensure the entire map fits inside the
 * 			minimap area.
 * 			- Clamps the cell size to the allowed MIN_CELL_SIZE and 
 * 			MAX_CELL_SIZE limits
 * 			- Sets the screen offset (offset_x/y) for where the minimap will be
 * 			drawn
 * 			- Calculates the minimap's pixel dimensions (width/height) based on
 * 			map size and minimap's cell size
 * 			- Creates the minimap's image buffer (img) used to render the 
 * 			minimap
 * 			- Determines how many rays will be drawn on the minimap and 
 * 			allocates memory for them
 * 
 * @param game	Pointer to the main game structure
 * @param minimap	Pointer to the minimap structure to initialize
 * @return	1 on succes, 0 on failure
 */

int	init_minimap(t_game *game, t_minimap *minimap)
{
	int	cell_size_w;
	int	cell_size_h;

	cell_size_w = MAX_MINIMAP_SIZE / game->map->width;
	cell_size_h = MAX_MINIMAP_SIZE / game->map->height;
	if (cell_size_w < cell_size_h)
		minimap->cell_size = cell_size_w;
	else
		minimap->cell_size = cell_size_h;
	if (minimap->cell_size < MIN_CELL_SIZE)
		minimap->cell_size = MIN_CELL_SIZE;
	else if (minimap->cell_size > MAX_CELL_SIZE)
		minimap->cell_size = MAX_CELL_SIZE;
	minimap->offset_x = 20;
	minimap->offset_y = 20;
	minimap->width = game->map->width * minimap->cell_size;
	minimap->height = game->map->height * minimap->cell_size;
	minimap->img = mlx_new_image(game->mlx, minimap->width, minimap->height);
	if (!minimap->img)
		return (error_msg(NULL, NULL, 0));
	minimap->ray_count = WIN_WIDTH / 20;
	minimap->rays = malloc(sizeof(t_ray) * minimap->ray_count);
	if (!minimap->rays)
		return (error_msg(NULL, NULL, 0));
	return (1);
}
