/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 09:18:39 by marvin            #+#    #+#             */
/*   Updated: 2025/10/15 13:13:17 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Initializes the player's direction and camera plane based on the
 * 			starting direction character (N, S, E, W).
 * 			Sets dir_x, dir_y (direction vector) and plane_x, plane_y (camera
 * 			plane perpendicular to direction) according to standard raycasting
 * 			conventions.
 * 			- dir_x, dir_y: direction vector of the player, 'where is looking
 * 			to':
 * 				* N -> dir_x = 0, dir_y = -1
 * 				* S -> dir_x = 0, dir_y = 1
 * 				* E -> dir_x = 1, dir_y = 0
 * 				* W -> dir_x = -1, dir_y = 0
 * 			- plane_x, plane_y: plano de la cámara, define el campo de visión
 * 			del player y es perpendicular al vector de dirección.
 * 			Si el jugador mira hacia el norte, con vector de dirección (0, .1),
 * 			la cámara debe extenderse perpendicular a esto para cubrir la
 * 			pantalla horizontalmente, así que plane_x = 0.66 y plane_y = 0.
 * 			0.66 es el ángulo de visión. Es decir, está viendo 0.66 grados en
 * 			x y nada en y
 * 				⚠️ HACER DIBUJO PARA ENTENDER BIEN ESTO!
 * 			Como las direcciones y los planos se han inicialziado en 0, podemos
 * 			ahorrarnos esas líneas
 *
 * 			0.66 de plane_x/y está relacionado con FOV=66º, estándar Wolfenstein
 *
 * @param player	Pointer to the player structure to initialize
 */

void	init_player_orientation(t_player *player)
{
	if (player->dir == 'N')
	{
		player->dir_y = -1;
		player->plane_x = 0.66;
	}
	else if (player->dir == 'S')
	{
		player->dir_y = 1;
		player->plane_x = -0.66;
	}
	else if (player->dir == 'E')
	{
		player->dir_x = 1;
		player->plane_y = 0.66;
	}
	else if (player->dir == 'W')
	{
		player->dir_x = -1;
		player->plane_y = -0.66;
	}
}

/**
 * @brief	Updates the player's position and orientation based on movement
 * 			flags.
 * 			- Checks the movement booleans in the player structure
 * 			- Calls the corresponding movement or rotation functions to update
 * 			position and orientation/direction
 *
 * @param game	Pointer to the main game structure
 * @param player	Pointer to the player structure to update
 */

void	update_player(t_game *game, t_player *player)
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

void	handle_mouse_click(mouse_key_t button, action_t action,
		modifier_key_t mods, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	(void)mods;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS)
	{
		game->mouse_rotation_enabled = !game->mouse_rotation_enabled;
		if (game->mouse_rotation_enabled)
			printf("🎮 Mouse rotation ON\n");
		else
			printf("🛑 Mouse rotation OFF\n");
	}
}
