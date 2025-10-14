/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sprite.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:20:52 by root              #+#    #+#             */
/*   Updated: 2025/10/14 16:37:39 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

/**tiene que iterar por todo el array de estructuras t_sprite */

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
		game->sprite[i]->frames = NULL;
		game->sprite[i]->frame_count = 6;
		game->sprite[i]->curr_frame = 0;
		game->sprite[i]->last_update = 0.0;
		game->sprite[i]->frame_time = 0.5;
		game->sprite[i]->x = 0;
		game->sprite[i]->y = 0;
		game->sprite[i]->cam_x = 0;
		game->sprite[i]->cam_y = 0;
		game->sprite[i]->screen_x = 0;
		game->sprite[i]->height = 0;
		game->sprite[i]->width = 0;
		i++;
	}
	return (init_sprite_position(game), 1);
}
