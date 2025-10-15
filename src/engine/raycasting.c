/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 09:58:51 by root              #+#    #+#             */
/*   Updated: 2025/10/15 10:28:32 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Initializes a ray corresponding to the given screen column.
 * 			For each vertical column on the screen, a ray is cast into the
 * 			game world. This function calculates the direction of the ray
 * 			based on the player's viewing direction and camera plane, which
 * 			together define the FOV (Field Of View)
 * 
 * 			- Convert the current column number ('col') into a normalized 
 * 			coordinate 'camera_x' 
 * 				* maps the column [0, WIN_WIDTH] to the range [-1, 1]
 * 				* -1 represents the left edge of the screen, 1 the right edge
 * 			- Compute the ray direction (dir_x/y) based on how far the column	
 * 			is from the player's center view
 * 			- Determine the map cell where the player currently stands (map_x/y)
 * 			- Compute delta distances:
 * 				* delta_x/y represent how far the ray travels between
 * 				consecutive vertical (X) or horizontal (Y) grid lines
 * 				* If the ray's direction component is 0, sets delta to a very
 * 				large value to avoid division by zero
 * 
 * @param player	Pointer to the player structure
 * @param col	Current screen column (ray index)
 * @return	A fully initialized ray structure for the given column
 */

static t_ray	init_ray(t_player *player, int col)
{
	t_ray	ray;

	ray.camera_x = 2 * (double)col / (double)WIN_WIDTH - 1;
	ray.dir_x = player->dir_x + player->plane_x * ray.camera_x;
	ray.dir_y = player->dir_y + player->plane_y * ray.camera_x;
	ray.map_x = (int)player->pos_x;
	ray.map_y = (int)player->pos_y;
	if (ray.dir_x == 0)
		ray.delta_dx = HUGE_VAL;
	else
		ray.delta_dx = fabs(1 / ray.dir_x);
	if (ray.dir_y == 0)
		ray.delta_dy = HUGE_VAL;
	else
		ray.delta_dy = fabs(1 / ray.dir_y);
	return (ray);
}

/**
 * @brief	Casts all rays for the current frame - one per screen column
 * 			This function iterates through all columns of the screen,
 * 			and performs the full raycasting pipeline:
 * 			- Initializes all the rays via init_ray()
 * 			- Compute step direction and initial side distances via
 * 			calc_step_dist()
 * 			- Perform the DDA algorithm to find the wall hit using
 * 			perform_dda()
 * 			- Compute perpendicular wall distance and screen projection with
 * 			calc_wall_dist_height()
 * 
 * @param game	Pointer to the game main structure
 */

void	cast_all_rays(t_game *game)
{
	int	i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		game->rays[i] = init_ray(game->player, i);
		calc_step_dist(&game->rays[i], game->player);
		perform_dda(game, &game->rays[i]);
		calc_wall_dist_height(&game->rays[i], game->player);
		i++;
	}
}
