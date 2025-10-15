/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_rays.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:11:45 by root              #+#    #+#             */
/*   Updated: 2025/10/15 10:26:24 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Casts a single ray from the player's position to detect where it 
 * 			hits a wall - used specifically for drawing the minimap.
 * 			Calculates the ray's direction and initializes all variables for
 * 			the DDA algorithm, which is used to trace the ray through the 
 * 			map's grid until it hits a wall.
 * 			- camera_x: horizontal position of the ray on the camera plane,
 * 			normalized to range [-1, 1]. Determines how far to the left/right
 * 			of the player's center view the ray is cast
 * 			- dir_x/y: direction vector of the ray, calculated by combining
 * 			the player's viewing direction and the camera plane, scaled by
 * 			camera_x
 * 			- map_x/y: grid cell where the player is currently in
 * 			- delta_dx/y: distance the ray has to travel to cross one 
 * 			vertical or horizontal grid line
 * 				- If dir_x/y is 0, avoids division by zero by using HUGE_VAL
 * 			- Initializes step directions via calc_step_dist()
 * 			- Runs DDA via perform_dda()
 * 			- The distance to the hitting point is calculated via 
 * 			calc_wall_dist_height()
 * 			- Final hit point is calculated in world coordinates (hit_x/y)
 * 			- Pixel coordinates (hit_x_px/y_px) are calculated for drawing on 
 * 			the minimap
 * 
 * @param game		Pointer to the main game structure (includes player and map)
 * @param minimap	Pointer to the minimap structure (used to scale pixel values)
 * @param i			Index of the current ray being cast
 * 
 * @return t_ray	The resulting ray structure with hit data and pixel coords
 */

static t_ray	cast_ray_minimap(t_game *game, t_minimap *minimap, int i)
{
	t_ray	ray;

	ray.camera_x = 2 * i / (double)minimap->ray_count - 1;
	ray.dir_x = game->player->dir_x + game->player->plane_x * ray.camera_x;
	ray.dir_y = game->player->dir_y + game->player->plane_y * ray.camera_x;
	ray.map_x = (int)game->player->pos_x;
	ray.map_y = (int)game->player->pos_y;
	if (ray.dir_x == 0)
		ray.delta_dx = HUGE_VAL;
	else
		ray.delta_dx = fabs(1 / ray.dir_x);
	if (ray.dir_y == 0)
		ray.delta_dy = HUGE_VAL;
	else
		ray.delta_dy = fabs(1 / ray.dir_y);
	calc_step_dist(&ray, game->player);
	perform_dda(game, &ray);
	calc_wall_dist_height(&ray, game->player);
	ray.hit_x = game->player->pos_x + ray.dir_x * ray.dist;
	ray.hit_y = game->player->pos_y + ray.dir_y * ray.dist;
	ray.hit_x_px = ray.hit_x * minimap->cell_size;
	ray.hit_y_px = ray.hit_y * minimap->cell_size;
	return (ray);
}

/**
 * @brief	Casts all rays required for the minimap view.
 * 			- Iterates over all ray slots in the minimap (ray_count)
 * 			- Calls cast_ray_minimap() for each ray, storing results 
 * 			in minimap->rays[]
 * 			Prepares all rays for later drawing via draw_rays().
 * 
 * @param game		Pointer to the main game structure
 * @param minimap	Pointer to the minimap structure (contains the rays
 * 					array)
 */

void	cast_all_rays_minimap(t_game *game, t_minimap *minimap)
{
	int	i;

	i = 0;
	while (i < minimap->ray_count)
	{
		minimap->rays[i] = cast_ray_minimap(game, minimap, i);
		i ++;
	}
}
