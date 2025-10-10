/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 09:18:39 by marvin            #+#    #+#             */
/*   Updated: 2025/10/06 09:18:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_player_orientation(t_player *player)
{
	if (player->dir == 'N')
	{
		player->dir_x = 0;
		player->dir_y = -1;  /* Norte es hacia arriba */
		player->plane_x = 0.66;  /* Plano de cámara perpendicular */
		player->plane_y = 0;
	}
	else if (player->dir == 'S')
	{
		player->dir_x = 0;
		player->dir_y = 1;   /* Sur es hacia abajo */
		player->plane_x = -0.66;
		player->plane_y = 0;
	}
	else if (player->dir == 'E')
	{
		player->dir_x = 1;   /* Este es hacia la derecha */
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = 0.66;
	}
	else if (player->dir == 'W')
	{
		player->dir_x = -1;  /* Oeste es hacia la izquierda */
		player->dir_y = 0;
		player->plane_x = 0;
		player->plane_y = -0.66;
	}
}

//DESDE DÓNDE LLAMAR A ESTA FUNCIÓN??
//ACTUALIZAR LA ORIENTACIÓN CON LAS FLECHAS! ESO EN MOVES

void	update_player(t_game *game, t_player *player) //actualizar px/py y ángulo dependiendo de los moves, no¿
{
	if (player->move_forward == true)
		move_forward(game, player);
	if (player->move_backward == true)
		move_backward(game, player);
	if (player->move_right == true)
		move_right(game, player);
	if (player->move_left == true)
		move_left(game, player);
	if (player->turn_right == true)
		rotate_player(player, RIGHT);
	if (player->turn_left == true)
		rotate_player(player, LEFT);
}
