
#include "../../inc/cub3d.h"

/*
** Rota al jugador (dirección y plano de cámara)
** direction: -1 = izquierda, 1 = derecha
*/
void	rotate_player(t_game *game, int direction)
{
	double	old_dir_x;
	double	old_plane_x;
	double	rot_speed;

	rot_speed = game->player->rot_speed * direction;
	old_dir_x = game->player->dir_x;
	game->player->dir_x = game->player->dir_x * cos(rot_speed)
		- game->player->dir_y * sin(rot_speed);
	game->player->dir_y = old_dir_x * sin(rot_speed)
		+ game->player->dir_y * cos(rot_speed);
	old_plane_x = game->player->plane_x;
	game->player->plane_x = game->player->plane_x * cos(rot_speed)
		- game->player->plane_y * sin(rot_speed);
	game->player->plane_y = old_plane_x * sin(rot_speed)
		+ game->player->plane_y * cos(rot_speed);
}

/*
** Mueve al jugador con detección de colisiones
** direction: 1=adelante, -1=atrás, 2=derecha, -2=izquierda
*/
void	move_player(t_game *game, int direction)
{
	if (direction == 1)
		move_forward(game);
	else if (direction == -1)
		move_backward(game);
	else if (direction == 2)
		move_right(game);
	else if (direction == -2)
		move_left(game);
}

/*
** Maneja pulsaciones de teclas
** Renderiza SOLO cuando hay movimiento (clave para el rendimiento)
*/
int	handle_keypress(int keycode, t_game *game)
{
	int	moved;

	moved = 0;
	if (keycode == KEY_ESC)
		handle_close(game);
	else if (keycode == KEY_W)
	{
		move_player(game, 1);
		moved = 1;
	}
	else if (keycode == KEY_S)
	{
		move_player(game, -1);
		moved = 1;
	}
	else if (keycode == KEY_A)
	{
		move_player(game, -2);
		moved = 1;
	}
	else if (keycode == KEY_D)
	{
		move_player(game, 2);
		moved = 1;
	}
	else if (keycode == KEY_LEFT)
	{
		rotate_player(game, -1);
		moved = 1;
	}
	else if (keycode == KEY_RIGHT)
	{
		rotate_player(game, 1);
		moved = 1;
	}
	if (moved)
		render_game(game);
	return (0);
}

/*
** Cierra el juego limpiamente
*/
int	handle_close(t_game *game)
{
	cleanup_game(game);
	exit(0);
	return (0);
}
