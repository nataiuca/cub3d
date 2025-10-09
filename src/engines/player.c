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

/**
 * @brief
 *          M_PI: num pi
 *          Conversión de dirección a la que mira el player a radianes
 *          Radianes: miden los ángulos en base a la longitud del arco de
 *          un círculo, en lugar de dividir el círculo en 360 partes como   
 *          hacen los grados. 
 *          radianes = longitud del arco / radio
 *          360º = 2pi radianes
 *          180º = pi radianes
 *          
 *          Mirar al N es -pi / 2 porque son 90º con respecto al eje x
 *          y como el 0,0 es la esquina superior izquierda, tiene que ser
 *          negativo, se invierte.
 * 
 * @param
 */

void	init_player_orientation(t_player *player)
{
	if (player->dir == 'N')
		player->angle = -PI / 2;
	else if (player->dir == 'S')
		player->angle = PI / 2;
	else if (player->dir == 'E')
		player->angle = 2 * PI;
	else if (player->dir == 'W')
		player->angle = PI;
}

//DESDE DÓNDE LLAMAR A ESTA FUNCIÓN??
//ACTUALIZAR LA ORIENTACIÓN CON LAS FLECHAS! ESO EN MOVES

void	update_player(t_game *game, t_player *player) //actualizar px/py y ángulo dependiendo de los moves, no¿
{
	if (player->move_forward == true)
		move_forward(game);
	if (player->move_backward == true)
		move_backward(game);
	if (player->move_right == true)
		move_right(game);
	if (player->move_left == true)
		move_left(game);
	if (player->turn_right == true)
		rotate_player(game, RIGHT);
	if (player->turn_left == true)
		rotate_player(game, LEFT);
}