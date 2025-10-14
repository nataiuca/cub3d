/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 10:24:38 by root              #+#    #+#             */
/*   Updated: 2025/10/14 13:40:39 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Draws all rays on the minimap
 * 			- Iterates through each ray previously cast and draws a line
 * 			from the player to the ray's hit position.
 * 			- The number of rays (ray_count) is based on to the horizontal
 * 			resolution of the 3D rendering (WIN_WIDTH / 20)
 * 
 * @param game	Pointer to the main game structure
 */

void	draw_rays(t_game *game)
{
	int		i;

	i = 0;
	while (i < game->minimap->ray_count)
	{
		draw_line(game->minimap, game->minimap->rays[i], game->player);
		i ++;
	}
}

/**
 * @brief	Draws a sprite on the minimap as a filled circle
 * 			- Converts sprite's map coordinates (sprite->x/y) to pixel
 * 			coordinates in the minial (x, y) 
 * 			- Calls to draw_circle() function
 * 
 * @param minimap	Pointer to the minimap structure
 * @param sprite	Pointer to the sprite to draw
 */

void	draw_sprite_minimap(t_minimap *minimap, t_sprite *sprite)
{
	int		x;
	int		y;

	if (!sprite)
		return ;
	x = (int)(sprite->x * minimap->cell_size);
	y = (int)(sprite->y * minimap->cell_size);
	draw_circle(minimap, x, y, 0x800080FF);
}

/**
 * @brief	Draws the player as a filled circle on the minimap
 * 			- The player's position is converted from map coordinates
 * 			(player->pos_x/y) to pixe coordinates (player->px/py)
 * 			- Calls to draw_circle() function
 * 
 * @param minimap	Pointer to the minimap structure
 * @param player	Pointer to the player to draw
 */

void	draw_player(t_minimap *minimap, t_player *player)
{
	player->px = player->pos_x * minimap->cell_size;
	player->py = player->pos_y * minimap->cell_size;
	draw_circle(minimap, player->px, player->py, 0x00008BFF);
}

/**
 * @brief	Draws the entire map grid onto the minimap
 * 			- Iterates through all cells of the logical map
 * 			- For each cell:
 * 				- Determines the color using get_minimap_color()
 * 				- Draws the square on the minimap using draw_square()
 * 				- If the cell contains a sprite ('C'), it draws it using
 * 				draw_sprite_minimap()
 * 
 * @param game	Pointer to the main game structure
 */

void	draw_grid_sprites(t_game *game)
{
	int		x;
	int		y;
	char	cell;
	int		color;
	int		sprite_index;

	sprite_index = 0;
	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (x < game->map->width)
		{
			cell = game->map->grid[y][x];
			color = get_minimap_cell_color(cell);
			draw_square(game->minimap, x, y, color);
			if (cell == 'C')
			{
				draw_sprite_minimap(game->minimap, game->sprite[sprite_index]);
				sprite_index ++;
			}
			x ++;
		}
		y ++;
	}
}

/**
 * @brief	Draws the entire minimap, including grid, sprites, player, and rays
 * 			- Clears the minimap buffer before drawing
 * 			- Casts all rays used for minimap rendering
 * 			- Draws the map grid and any sprites
 * 			- Draws the player's current position
 * 			- Draws rays extending from the player to walls
 * 
 * @param game	Pointer to the game main structure
 */

void	draw_minimap(t_game *game)
{
	if (!game || !game->minimap || !game->minimap->img)
		return ;
	clear_minimap(game->minimap);
	cast_all_rays_minimap(game, game->minimap);
	draw_grid_sprites(game);
	draw_player(game->minimap, game->player);
	draw_rays(game);
}
