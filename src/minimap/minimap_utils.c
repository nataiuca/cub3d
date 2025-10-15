/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:13:26 by root              #+#    #+#             */
/*   Updated: 2025/10/14 13:50:48 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Fills a square (cell) on the minimap with a given color.
 * 			- Each cell in the logical map corresponds to a square of size
 * 			cell_size * cell_size pixels in the map
 * 			- Nested loops iterate over all pixels witthin that square
 * 			- Calculates the actual pixel position (px, py) in the image by:
 * 				- Multiplying map coordinates by cell size 
 * 				- Adding i/j for per-pixel position inside the square
 * 
 * @param minimap	Pointer to the minimap
 * @param x		Map X coordinate of the cell
 * @param y		Map Y coordinate of the cell
 * @param color	Color to fill the square with
 * 
 */

void	draw_square(t_minimap *minimap, int x, int y, int color)
{
	int	i;
	int	j;
	int	px;
	int	py;

	i = 0;
	while (i < minimap->cell_size)
	{
		j = 0;
		while (j < minimap->cell_size)
		{
			px = x * minimap->cell_size + j;
			py = y * minimap->cell_size + i;
			mlx_put_pixel(minimap->img, px, py, color);
			j ++;
		}
		i ++;
	}
}

/**
 * @brief	Draws a filled circle on the minimap image.
 * 			- The circle is centered at the pixel coordinates (x, y)
 * 			- The radius is calculated as a quarter of the minimap's cell size
 * 			- Draws the circle using the equation dx² + dy² <= r²
 */

void	draw_circle(t_minimap *minimap, int x, int y, int color)
{
	int		radius;
	int		dy;
	int		dx;

	radius = minimap->cell_size / 4;
	dy = -radius;
	while (dy <= radius)
	{
		dx = -radius;
		while (dx <= radius)
		{
			if (dx * dx + dy * dy <= radius * radius)
				mlx_put_pixel(minimap->img, x + dx, y + dy, color);
			dx ++;
		}
		dy ++;
	}
}

/**
 * @brief	Draws a line on the minimap between the player's position and the	
 * 			point where the ray hits a wall.
 * 			Uses the DDA (Digital Differential Analyzer) algorithm to draw
 * 			a smooth line from the player's position to the ray's hit point
 * 			in pixels.
 * 			- The line starts at the player's current position (player->px/py)
 * 			and ends at the position where the ray hits a wall
 * 			(ray.hit_x_px/y_px)
 * 			- dx/dy: store the differences between the start and the end point
 * 			in x and y in pixels
 * 			- steps: determines how many pixels long the line is
 * 				-  if the line is more horizontal (dx > dy), we need more 
 * 				steps in x
 * 				- if the line is more vertical (dy > dx), we need more steps
 * 				in y
 * 				- this ensures the line progresses pixel by pixel in the longer
 * 				direction
 * 			- x_inc/y_inc: incremental steps per pixel. Represents the amount
 * 			to increment x and y at each step. These are the normalized 	
 * 			movement values in each axis. They allow the line to be drawn at 
 * 			smooth angle, regardless of direction
 * 
 * @param minimap	Pointer to the minimap structure (where the line will be 
 * 					drawn)
 * @param ray	The ray containing the hit point
 * @param player	Pointer to the player structure containing the current 
 * 					position
 */

void	draw_line(t_minimap *minimap, t_ray ray, t_player *player)
{
	float	x;
	float	y;
	int		i;
	float	dx;
	float	dy;

	dx = ray.hit_x_px - player->px;
	dy = ray.hit_y_px - player->py;
	ray.steps = fmaxf(fabsf(dx), fabsf(dy));
	if (ray.steps < 1)
		ray.steps = 1;
	ray.x_inc = dx / ray.steps;
	ray.y_inc = dy / ray.steps;
	x = player->px;
	y = player->py;
	i = 0;
	while (i <= ray.steps)
	{
		if (x >= 0 && y >= 0 && x < minimap->img->width
			&& y < minimap->img->height)
			mlx_put_pixel(minimap->img, (int)x, (int)y, 0x40E0D0FF);
		x += ray.x_inc;
		y += ray.y_inc;
		i ++;
	}
}

/**
 * @brief	Returns the color to use for a given map cell in the minimap
 * 			- For cells of type '1' (wall) sets coral color
 * 			- For cells of type '0' (floor), player or sprite, sets green color
 * 			- For other (outside) sets dark grey
 * 
 * @param cell	The character representing the cell type in the map grid
 * @return	The corresponding color in RGBA format (32-bit integer)
 */

int	get_minimap_cell_color(char cell)
{
	if (cell == '1')
		return (0xFF7F50FF);
	else if (cell == '0')
		return (0x556B2FFF);
	else if (ft_strchr("NSEWC", cell))
		return (0x556B2FFF);
	return (0x222222FF);
}

/**
 * @brief	Clears the entire minimap image buffer by setting every pixel
 * 			to transparent. This function is called before redrawing the
 * 			minimap to avoid artifacts from previous frames
 * 
 * @param minimap	Pointer to the minimap structure containing the image
 * 					buffer
 */

void	clear_minimap(t_minimap *minimap)
{
	uint32_t	x;
	uint32_t	y;

	y = 0;
	while (y < minimap->img->height)
	{
		x = 0;
		while (x < minimap->img->width)
		{
			mlx_put_pixel(minimap->img, x, y, 0x00000000);
			x ++;
		}
		y ++;
	}
}
