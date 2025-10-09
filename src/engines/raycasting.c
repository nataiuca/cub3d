
#include "cub3d.h"

/*
** Inicializa un rayo para una columna X de pantalla
** Calcula dirección y posición inicial en el mapa
*/
void	init_ray(t_game *game, int x)
{
	game->ray->camera_x = 2 * x / (double)WIN_WIDTH - 1;
	game->ray->ray_dir_x = game->player->dir_x
		+ game->player->plane_x * game->ray->camera_x;
	game->ray->ray_dir_y = game->player->dir_y
		+ game->player->plane_y * game->ray->camera_x;
	game->ray->map_x = (int)game->player->pos_x;
	game->ray->map_y = (int)game->player->pos_y;
	if (game->ray->ray_dir_x == 0)
		game->ray->delta_dist_x = 1e30;
	else
		game->ray->delta_dist_x = fabs(1 / game->ray->ray_dir_x);
	if (game->ray->ray_dir_y == 0)
		game->ray->delta_dist_y = 1e30;
	else
		game->ray->delta_dist_y = fabs(1 / game->ray->ray_dir_y);
}

/*
** Calcula paso y distancia inicial al siguiente lado
** Determina en qué dirección avanzar (step) y distancia inicial (side_dist)
*/
void	calc_step_dist(t_game *game)
{
	if (game->ray->ray_dir_x < 0)
	{
		game->ray->step_x = -1;
		game->ray->side_dist_x = (game->player->pos_x - game->ray->map_x)
			* game->ray->delta_dist_x;
	}
	else
	{
		game->ray->step_x = 1;
		game->ray->side_dist_x = (game->ray->map_x + 1.0 - game->player->pos_x)
			* game->ray->delta_dist_x;
	}
	if (game->ray->ray_dir_y < 0)
	{
		game->ray->step_y = -1;
		game->ray->side_dist_y = (game->player->pos_y - game->ray->map_y)
			* game->ray->delta_dist_y;
	}
	else
	{
		game->ray->step_y = 1;
		game->ray->side_dist_y = (game->ray->map_y + 1.0 - game->player->pos_y)
			* game->ray->delta_dist_y;
	}
}

/*
** Verifica si hemos golpeado una pared
** Incluye bounds checking para evitar segfaults
*/
static int	check_hit(t_game *game)
{
	if (game->ray->map_x < 0 || game->ray->map_x >= game->map->width
		|| game->ray->map_y < 0 || game->ray->map_y >= game->map->height)
		return (1);
	if (game->map->grid[game->ray->map_y][game->ray->map_x] == '1')
		return (1);
	return (0);
}

/*
** Algoritmo DDA - Avanza el rayo hasta golpear una pared
** Optimizado: menos comprobaciones en el loop
*/
void	perform_dda(t_game *game)
{
	game->ray->hit = 0;
	while (game->ray->hit == 0)
	{
		if (game->ray->side_dist_x < game->ray->side_dist_y)
		{
			game->ray->side_dist_x += game->ray->delta_dist_x;
			game->ray->map_x += game->ray->step_x;
			game->ray->side = 0;
		}
		else
		{
			game->ray->side_dist_y += game->ray->delta_dist_y;
			game->ray->map_y += game->ray->step_y;
			game->ray->side = 1;
		}
		if (check_hit(game))
			game->ray->hit = 1;
	}
}

/*
** Calcula distancia a la pared y altura de la línea a dibujar
*/
void	calc_wall_dist(t_game *game)
{
	if (game->ray->side == 0)
		game->ray->perp_wall_dist = (game->ray->map_x - game->player->pos_x
				+ (1 - game->ray->step_x) / 2) / game->ray->ray_dir_x;
	else
		game->ray->perp_wall_dist = (game->ray->map_y - game->player->pos_y
				+ (1 - game->ray->step_y) / 2) / game->ray->ray_dir_y;
	if (game->ray->perp_wall_dist == 0)
		game->ray->line_height = WIN_HEIGHT;
	else
		game->ray->line_height = (int)(WIN_HEIGHT
				/ game->ray->perp_wall_dist);
	game->ray->draw_start = -game->ray->line_height / 2 + WIN_HEIGHT / 2;
	if (game->ray->draw_start < 0)
		game->ray->draw_start = 0;
	game->ray->draw_end = game->ray->line_height / 2 + WIN_HEIGHT / 2;
	if (game->ray->draw_end >= WIN_HEIGHT)
		game->ray->draw_end = WIN_HEIGHT - 1;
}
