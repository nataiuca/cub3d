/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 14:57:27 by root              #+#    #+#             */
/*   Updated: 2025/10/15 15:43:53 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * @brief	Handles a clean shutdown of the game.
 * 			Frees all resources and exits the program.
 *
 * @param game	Pointer to the main game structure.
 *
 * @return Always returns 0.
 */

int	handle_close(t_game *game)
{
	cleanup_game(game);
	exit(0);
	return (0);
}

/**
 * @brief	Processes key press events.
 * 			Enables movement and rotation flags for the player when 
 * 			corresponding keys are pressed (W, A, S, D, <-, ->).
 * 			Rendering updates are triggered only when movement occurs.
 * 			If any other key is pressed, the mouse rotation mode is toggled,
 * 			switching between free cursor and locked rotation control
 *
 * @param data	MLX key data structure containing key and action information.
 * @param param	Pointer to the main game structure.
 */

void	handle_keypress(mlx_key_data_t data, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (data.key == MLX_KEY_ESCAPE && data.action == MLX_PRESS)
		handle_close(game);
	else if (data.key == MLX_KEY_W && data.action == MLX_PRESS)
		game->player->move_forward = true;
	else if (data.key == MLX_KEY_S && data.action == MLX_PRESS)
		game->player->move_backward = true;
	else if (data.key == MLX_KEY_A && data.action == MLX_PRESS)
		game->player->move_left = true;
	else if (data.key == MLX_KEY_D && data.action == MLX_PRESS)
		game->player->move_right = true;
	else if (data.key == MLX_KEY_LEFT && data.action == MLX_PRESS)
		game->player->turn_left = true;
	else if (data.key == MLX_KEY_RIGHT && data.action == MLX_PRESS)
		game->player->turn_right = true;
	else if (data.action == MLX_PRESS)
	{
		game->mouse_rotation_enabled = !game->mouse_rotation_enabled;
		if (game->mouse_rotation_enabled)
			printf("🎮 Mouse rotation ON\n");
		else
			printf("🛑 Mouse rotation OFF\n");
	}
}

/**
 * @brief	Handles horizontal camera rotation using the mouse.
 * 			Simulates an FPS-like head rotation: only horizontal
 * 			movement (X-axis) is used. The vertical movement (Y-axis)
 * 			is ignored. After rotation, the cursor is re-centered
 * 			to prevent it from leaving the window.
 *
 * @param xpos	Current mouse X position.
 * @param ypos	Current mouse Y position (unused).
 * @param param	Pointer to the main game structure.
 */

void	handle_mouse_movement(double xpos, double ypos, void *param)
{
	t_game		*game;
	static int	last_x;
	int			delta_x;

	last_x = WIN_WIDTH / 2;
	game = (t_game *)param;
	(void)ypos;
	if (!game->mouse_rotation_enabled)
		return ;
	if (xpos < 0 || xpos > WIN_WIDTH || ypos < 0 || ypos > WIN_HEIGHT)
		return ;
	delta_x = xpos - last_x;
	last_x = xpos;
	if (delta_x > 0)
		rotate_player(game->player, RIGHT);
	else if (delta_x < 0)
		rotate_player(game->player, LEFT);
	if (game->mouse_rotation_enabled)
	{
		mlx_set_mouse_pos(game->mlx, WIN_WIDTH / 2, WIN_HEIGHT / 2);
		last_x = WIN_WIDTH / 2;
	}
}
