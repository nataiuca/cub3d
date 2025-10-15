/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:57:59 by root              #+#    #+#             */
/*   Updated: 2025/10/15 12:58:31 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Draws the ceiling and floor for a single vertical column on screen
 * 			- Iterates over each pixel in the column:
 * 				- For pixels above the wall slice (y < ray->draw_start), set
 * 				the pixel color to the ceiling color
 * 				- For pixels below the wall slice (y > ray->draw_end), set
 * 			the pixel color to the floor color
 *	
 * @param game	Pointer to the main game structure
 * @param ray	Pointer to the ray representing the current column to draw
 * @param x		Screen X coordinate (column) being draw	
*/

static void	draw_floor_ceiling(t_game *game, t_ray *ray, int x)
{
	int	y;

	y = 0;
	while (y < ray->draw_start)
	{
		mlx_put_pixel(game->img, x, y, game->map->ceiling_color);
		y++;
	}
	y = ray->draw_end + 1;
	while (y < WIN_HEIGHT)
	{
		mlx_put_pixel(game->img, x, y, game->map->floor_color);
		y++;
	}
}

/**
 * @brief	Draws a textured wall slice for a single vertical column
 * 			- Computes the scaling factor (step) between the texture height
 * 			and the on-screen wall slice height
 * 			- Initializes tex_pos to determine the first texture row to
 * 			sample based on the vertical position of the wall slice
 * 			- Iterates over each vertical pixel y from draw_start to draw_end:
 * 				* computes the corresponding texture Y coordinate (tex_y)
 * 				proportional to the pixel's position within the wall slice
 * 				* clamps tex_y to [0, texture height - 1] to avoid 
 * 				out-of-bounds access
 * 				* retrieves the color from the texture at ray->tex_x/y
 * 				* puts the pixel to the screen image at x,y
 * 			- Updates tex_pos after each row to move to the next texture pixel
 * 
 * @param game	Pointer to the game main structure
 * @param ray	Pointer to the ray representing the current column
 * @param x		Screen X coordinate (column) being drawn
 * @param tex	Pointer to the texture image for this wall slice
 */

static void	draw_wall(t_game *game, t_ray *ray, int x, mlx_image_t *tex)
{
	int		y;
	int		color;
	double	step;
	double	tex_pos;

	step = (double)tex->height / (double)ray->line_height;
	tex_pos = (ray->draw_start - WIN_HEIGHT / 2.0 + ray->line_height / 2.0)
		* step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		ray->tex_y = (int)tex_pos;
		if (ray->tex_y < 0)
			ray->tex_y = 0;
		if (ray->tex_y >= (int)tex->height)
			ray->tex_y = tex->height - 1;
		tex_pos += step;
		color = get_tex_color(tex, ray->tex_x, ray->tex_y);
		mlx_put_pixel(game->img, x, y, color);
		y++;
	}
}

/**
 * @brief	Draws a single column of the 3D view.
 * 			- Determines the appropriate texture index for the wall hit by 
 * 			the ray via get_texture_index()
 * 			- Retrieves the texture image
 * 			- Calculates the horizontal texture coordinate for the wall slice
 * 			and stores it in ray->tex_x via calc_texture_x
 * 			- Draws floor and ceiling for the column via draw_floor_ceiling()
 * 			- Draws the textured wall slice via draw_wall()
 * 
 * @param game	Pointer to the main game structure
 * @param col	Screen X coordinate (column) being drawn
 * @param ray	Pointer ot the ray representing this column
 */

static void	draw_column(t_game *game, int col, t_ray *ray)
{
	int			tex_index;
	mlx_image_t	*tex;

	tex_index = get_texture_index(ray);
	tex = game->textures[tex_index];
	calc_texture_x(ray, game->player, tex);
	draw_floor_ceiling(game, ray, col);
	draw_wall(game, ray, col, tex);
}

/**
 * @brief	Draws the entire 3D view for the current frame.
 * 			- Iterates over every screen column from left to right
 * 			- Calls draw_column() for each ray stored in game->rays[]
 * 			- Each column draws its corresponding wall, floor, and ceiling
 * 
 * @param game	Pointer to the game main structure
 */

void	draw_3d_view(t_game *game)
{
	int	i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		draw_column(game, i, &game->rays[i]);
		i ++;
	}
}
