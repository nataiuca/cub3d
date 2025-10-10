
#include "../../inc/cub3d.h"

/*
** Cierra el juego limpiamente
*/
int	handle_close(t_game *game)
{
	cleanup_game(game);
	exit(0);
	return (0);
}

/*
** Maneja pulsaciones de teclas
** Renderiza SOLO cuando hay movimiento (clave para el rendimiento)
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
}

