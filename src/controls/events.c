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

/* Funcion para implementar la rotacion de la camara con
el raton. Cuando el jugador mueve el raton horizontalente,
la camara debe rotar a la izqda o a la derecha como en un FPS,
no hace falta moverlo verticalmente. es como si el jugador
girara la cabeza.  No usamos la posición vertical (y), solo la horizontal (x)
Calcula cuánto se ha movido el ratón horizontalmente desde la última vez
Actualiza la posición del ratón anterior para la próxima llamada
Si el ratón se ha movido a la derecha, gira el jugador a la derecha
Reposiciona el ratón en el centro de la ventana para evitar que salga*/

void	handle_mouse_movement(double xpos, double ypos, void *param)
{
	t_game		*game;
	static int	last_x = WIN_WIDTH / 2;
	int			delta_x;

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
	mlx_set_mouse_pos(game->mlx, WIN_WIDTH / 2, WIN_HEIGHT / 2);
	last_x = WIN_WIDTH / 2;
}
