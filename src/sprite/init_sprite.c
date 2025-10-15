/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sprite.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:20:52 by root              #+#    #+#             */
/*   Updated: 2025/10/15 11:00:57 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Initializes the world-space positions of all sprites.
 * 			Iterates through the 2D map grid and locates all sprite markers.
 * 			For each sprite found, its corresponding t_sprite structure
 * 			is assigned a posiiton in the world coordinate system.
 * 			The sprite is placed at the center of its map cell.
 * 
 * @param game	Pointer to the main game structure
 */

static void	init_sprite_position(t_game *game)
{
	int	y;
	int	x;
	int	sprite_index;

	sprite_index = 0;
	y = 0;
	while (y < game->map->height)
	{
		x = 0;
		while (game->map->grid[y][x])
		{
			if (game->map->grid[y][x] == 'C')
			{
				game->sprite[sprite_index]->x = x + 0.5;
				game->sprite[sprite_index]->y = y + 0.5;
				sprite_index++;
			}
			x++;
		}
		y++;
	}
}

/**
 * @brief	Initializes a sprite structure with safe default values.
 * 
 * @param sprite	Pointer to the sprite structure to initialize
 */

static void	fill_sprite_default(t_sprite *sprite)
{
	sprite->frames = NULL;
	sprite->frame_count = 6;
	sprite->curr_frame = 0;
	sprite->last_update = 0.0;
	sprite->frame_time = 0.5;
	sprite->x = 0;
	sprite->y = 0;
	sprite->cam_x = 0;
	sprite->cam_y = 0;
	sprite->screen_x = 0;
	sprite->draw_x = 0;
	sprite->draw_y = 0;
	sprite->height = 0;
	sprite->width = 0;
}

/**
 * @brief	Allocates and initializes all sprite structures in the game.
 * 			- Allocates memory for the array of sprite pointers (game->sprite)
 * 			- Allocates each individual t_sprite structure
 * 			- Initializes all sprite fields with default values via 
 * 			fill_sprite_default()
 * 			- Calls init_sprite_position() to set the sprite world positions
 * 
 * @param game	Pointer to the main game structure
 * @return	1 on succes, 0 on memory allocation failure
 */

int	init_sprites(t_game *game)
{
	int	i;

	game->sprite = malloc(sizeof(t_sprite *) * game->sprite_count);
	if (!game->sprite)
		return (error_msg(NULL, NULL, 0));
	i = 0;
	while (i < game->sprite_count)
	{
		game->sprite[i] = malloc(sizeof(t_sprite));
		if (!game->sprite[i])
			return (error_msg(NULL, NULL, 0));
		fill_sprite_default(game->sprite[i]);
		i++;
	}
	return (init_sprite_position(game), 1);
}
