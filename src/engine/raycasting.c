#include "cub3d.h"

/**
 * @brief	Checkea si hemos golpeado una pared
 * 			- Si estamos fuera del mapa: devuelve 1 para evitar segfault
 * 			- Si la celda actual del mapa contiene '1' (pared), devuelve 1.
 */

int	check_hit(t_ray *ray, t_map *map)
{
	if (ray->map_x < 0 || ray->map_x >= map->width)
		return (1);
	if (ray->map_y < 0 || ray->map_y >= map->height)
		return (1);
	if (map->grid[ray->map_y][ray->map_x] == '1')
		return (1);
	return (0);
}

/**
 * @brief	Inicializa el rayo para la columna número 'col' de la pantalla.
 * 			- Proyecta la columna de pantalla al rango [-1, 1], donde -1
 * 			es el extremo izquierdo de la pantalla y el 1 el derecho
 * 			HACER ESQUEMA DIBUJADO DE ESTO!
 * 			- A partir de la proyección de la columna en pantalla en rango [-1 ,1]
 * 			se calcula la dirección del rayo en el campo de visión (dir_x, dir_y)
 * 			- Usa el camera_x para ajustar la dirección del rayo a lo ancho de la 
 * 			pantalla
 * 			- Si la posición incial del player en el mapa es 0, pone delta_dx
 * 			en UN VALOR ALTISIMO para prevenir división por cero
 * 			- delta_dx/y es la distancia entre líneas verticales/horizontales
 * 			que el rayo debe recorrer.
 * 
 * 			- camera_x: horizontal position of the ray on the camera plane,
 * 			normalized to range [-1, 1]. Determines how far to the left/right
 * 			of the player's center view the ray is cast
 * 			- dir_x/y: direction vector of the ray, calculated by combining
 * 			the player's viewing direction and the camera plane, scaled by
 * 			camera_x
 * 
 * @param ray	El rayo concreto a inicializar, el de la columna col
 * @param player	Puntero a estructura del jugador
 * @param col	Columna actual, número de rayo
 */

t_ray	init_ray(t_player *player, int col)
{
	t_ray	ray;

	ray.camera_x = 2 * (double)col / (double)WIN_WIDTH - 1;
	ray.dir_x = player->dir_x + player->plane_x * ray.camera_x;
	ray.dir_y = player->dir_y + player->plane_y * ray.camera_x;
	ray.map_x = (int)player->pos_x;
	ray.map_y = (int)player->pos_y;
	if (ray.dir_x == 0)
		ray.delta_dx = HUGE_VAL;
	else
		ray.delta_dx = fabs(1 / ray.dir_x);
	if (ray.dir_y == 0)
		ray.delta_dy = HUGE_VAL;
	else
		ray.delta_dy = fabs(1 / ray.dir_y);
	return (ray);
}

void	cast_all_rays(t_game *game)
{
	int	i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		game->rays[i] = init_ray(game->player, i);
		calc_step_dist(&game->rays[i], game->player);
		perform_dda(game, &game->rays[i]);
		calc_wall_dist(&game->rays[i], game->player);
		i++;
	}
}
