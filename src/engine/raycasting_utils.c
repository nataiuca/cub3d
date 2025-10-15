/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 17:13:43 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/15 10:35:01 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Initializes the ray's step direction and initial side distances
 * 			for the DDA algorithm
 * 			- step_x/y: indicates in which direction (positive or negative) 
 * 			the ray will step through the grid in X and Y axes
 * 				- if the ray is going left/up -> step = -1
 * 				- if the ray is going right/down -> step = 1
 * 			- side_dx/dy: distance from the player's current position to the
 * 			first grid line intersection along the X or Y axis. Used as the
 * 			starting point for the DDA algorithm.
 * 
 * @param ray	Pointer to the current ray structure
 * @param player	Pointer to the player structure
 */

void	calc_step_dist(t_ray *ray, t_player *player)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dx = (player->pos_x - ray->map_x) * ray->delta_dx;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dx = (ray->map_x + 1.0 - player->pos_x) * ray->delta_dx;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dy = (player->pos_y - ray->map_y) * ray->delta_dy;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dy = (ray->map_y + 1.0 - player->pos_y) * ray->delta_dy;
	}
}

/**
 * @brief	Performs the DDA (Digital Differential Analyzer) algorithm to
 * 			find the first wall hit by the ray in the grid map.
 * 			The DDA algoruthm simulates the ray moving through the grid,
 * 			checking which side (X or Y) it will crcoss next. It continues
 * 			untill the ray collides with a wall.
 * 			- At each step, the algorithm chooses whether to move to the 
 * 			next x-side or y-side grid cell, depending on which side distance
 * 			(side_dx/y) is smaller
 * 			- side_dx/y are incremented by delta_dx/y at each step, simulating
 * 			the ray traveling through the grid
 * 			
 * 			- If side_dx < side_dy -> move to the next x-side:
 * 				* Increment side_dx by delta_dx
 * 				* Move ray->map_x by step_x
 * 				* set ray->side = 0 (X-side)
 *			- If side_dx > side_dy -> move to the next y-side:
 * 				* Increment side_dy by delta_dy
 * 				* Move ray->map_y by step_y
 * 				* Set ray->side = 1 (Y-side)
 * 
 * 			- After each move, the algorithm checks:
 * 				* If the ray's position goes outside the map boundaries
 * 				* Or if the current cell (grid[map_y][map_x]) contains a wall
 * 				('1').
 * 			In either case, the function returns immediately, since the ray 
 * 			has reached a collision point
 * 
 * @param game	Pointer to the game structure
 * @param ray	Pointer to the ray being processed
 */

void	perform_dda(t_game *game, t_ray *ray)
{
	while (1)
	{
		if (ray->side_dx < ray->side_dy)
		{
			ray->side_dx += ray->delta_dx;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dy += ray->delta_dy;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_x >= game->map->width
			|| ray->map_y < 0 || ray->map_y >= game->map->height
			|| game->map->grid[ray->map_y][ray->map_x] == '1')
			return ;
	}
}

/**
 * @brief	Calculates the perpendicular distance from the player to the wall
 * 			for x-side hit.
 * 			Prevents division by zero using small epsilon. If the x-direction
 * 			is almost 0, returns an infinite distance.
 * 
 * @param ray	Pointer to the current ray
 * @param player	Pointer th the player
 * @return	The perpendicular distance from the player to the wall for x-side
 */

static double	calc_dist_side_x(t_ray *ray, t_player *player)
{
	double	eps;

	eps = 1e-9;
	if (fabs(ray->dir_x) < eps)
		return (HUGE_VAL);
	return ((ray->map_x - player->pos_x + (1 - ray->step_x) / 2.0)
		/ ray->dir_x);
}

/**
 * @brief	Calculates the perpendicular distance from the player to the wall
 * 			for a y-side hit.
 * 			Prevents division by zero using small epsilon. If the y-direction
 * 			is almost 0, returns an infinite distance.
 * 
 * @param ray	Pointer to the current ray
 * @param player	Pointer th the player
 * @return	The perpendicular distance from the player to the wall for y-side
 */

static double	calc_dist_side_y(t_ray *ray, t_player *player)
{
	double	eps;

	eps = 1e-9;
	if (fabs(ray->dir_y) < eps)
		return (HUGE_VAL);
	return ((ray->map_y - player->pos_y + (1 - ray->step_y) / 2.0)
		/ ray->dir_y);
}

/**
 * @brief	Caculates the perpendicular distance and the height of the wall
 * 			slice to render.
 * 			Determines how tall wall should appear on screen based on the 
 * 			distance between the player and the wall hit by the ray. It also
 * 			computes where to start and end drawing the wall slice vertically.
 * 			- Selects the correct distance funcion depending on which side
 * 			was hit (x-side or y-side hit)
 * 			- If the computed distance is invalid (infinite or <= 0), uses
 * 			full window height
 * 			- Otherwise:
 * 				* Sets the height of the wall slice on the screen as 
 * 				line_height = WIN_HEIGHT / ray->dist (inverse proportional
 * 				to distance)
 * 				* draw_start and draw_end define the vertical segment to render
 * 				* These values are clamped withing window bounds
 * 
 * @param ray	Pointer to the current ray
 * @param player	Pointer to the player structure
 */

void	calc_wall_dist_height_height(t_ray *ray, t_player *player)
{
	if (ray->side == 0)
		ray->dist = calc_dist_side_x(ray, player);
	else
		ray->dist = calc_dist_side_y(ray, player);
	if (!isfinite(ray->dist) || ray->dist <= 0.0)
		ray->line_height = WIN_HEIGHT;
	else
		ray->line_height = (int)(WIN_HEIGHT / ray->dist);
	ray->draw_start = -ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_end >= WIN_HEIGHT)
		ray->draw_end = WIN_HEIGHT - 1;
}
