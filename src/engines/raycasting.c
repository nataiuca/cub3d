/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:42:11 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/09 17:49:40 by amacarul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

/**
 * @brief	Inicializa el rayo para la comlumna número 'col' de la pantalla.
 * 			- Proyecta la columna de pantalla al rango [-1, 1], donde -1
 * 			es el extremo izquierdo de la pantalla y el 1 el derecho
 * 			HACER ESQUEMA DIBUJADO DE ESTO!
 * 			- A partir de la proyección de la columna en pantalla en rango [-1 ,1]
 * 			se calcula la dirección del rayo en el campo de visión (dir_x, dir_y)
 * 			- Usa el camera_x para ajustar la dirección del rayo a lo ancho de la 
 * 			pantalla
 * 			- Si la posición incial del player en el mapa es 0, pone delta_dist_x
 * 			en UN VALOR ALTISIMO para prevenir división por cero
 * 			- delta_dist_x/y es la distancia entre líneas verticales/horizontales
 * 			que el rayo debe recorrer.
 * 
 * @param ray	El rayo concreto a inicializar, el de la columna col
 * @param player	Puntero a estructura del jugador
 * @param col	Columna actual, número de rayo
 */
t_ray	init_ray(t_player *player, int col)
{
	t_ray	ray;

	ray.camera_x = 2 * col / (double)WIN_WIDTH - 1;
	ray.dir_x = player->dir_x + player->plane_x * ray.camera_x;
	ray.dir_y = player->dir_y + player->plane_y * ray.camera_x;
	ray.map_x = (int)player->pos_x;
	ray.map_y = (int)player->pos_y;
	if (ray.dir_x == 0)
		ray.delta_dist_x = 1e30;
	else
		ray.delta_dist_x = fabs(1 / ray.dir_x);
	if (ray.dir_y == 0)
		ray.delta_dist_y = HUGE_VAL;
	else
		ray.delta_dist_y = fabs(1 / ray.dir_y);
	return(ray);
}

/**
 * @brief	Calcula:
 * 			- step_x/y: dirección en la que el rayo se moverá (hacia
 * 			adelante o atrás). 
 * 			- side_dist_x/y: distancia desde la posición del jugadro hasta el
 * 			primer cruce de línea de celda.
 * 
 * @param game
 */
void	calc_step_dist(t_ray *ray, t_player *player)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player->pos_x - ray->map_x)* ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - player->pos_x) * ray->delta_dist_x;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player->pos_y - ray->map_y)* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - player->pos_y) * ray->delta_dist_y;
	}
}

/**
 * @brief	Checkea si hemos golpeado una pared
 * 			- Si estamos fuera del mapa: devuelve 1 para evitar segfault
 * 			- Si la celda actual del mapa contiene '1' (pared), devuelve 1.
 */
static int	check_hit(t_ray *ray, t_map *map)
{
	if (ray->map_x < 0 || ray->map_x >= map->width) //si estamos fuera de los extremos laterales del mapa
		return (1);
	if (ray->map_y < 0 || ray->map_y >= map->height) //si estamos fuera de los extremos superior e inferior del mapa
		return (1);
	if (map->grid[ray->map_y][ray->map_x] == '1')
		return (1);
	return (0);
}

/**
 * @brief	Algoritmo DDA
 * 			- Compara cuál es el menor (side_dist_x o y) para determinar si avanza
 * 			en x o y. 
 * 			- Avanza a la siguiente celda en esa dirección
 * 			- Repite hasta golpear una pared
 * 
 * 
 */
void	perform_dda(t_game *game, t_ray *ray)
{
	ray->hit = 0;
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (check_hit(ray, game->map))
			ray->hit = 1;
	}
}

/**
 * @brief	Calcula:
 * 			- Distancia perpendicular desde el player hasta la pared hitteada
 * 			- La altura de la línea a dibujar en pantalla (inversamente
 * 			proporcional a la distancia de choque)
 * 			- draw_start/draw_end: límites de esa línea vertical en la pantalla,
 * 			para no dibujar fuera de rango
 */
void	calc_wall_dist(t_ray *ray, t_player *player)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - player->pos_x + (1 - ray->step_x) / 2) / ray->dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - player->pos_y + (1 - ray->step_y) / 2) / ray->dir_y;
	if (ray->perp_wall_dist == 0)
		ray->line_height = WIN_HEIGHT;
	else
		ray->line_height = (int)(WIN_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_end >= WIN_HEIGHT)
		ray->draw_end = WIN_HEIGHT - 1;
}

void	cast_all_rays(t_game *game)
{
	printf("DEBUG: raycasting\n");
	int		i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		printf("dentro del while\n");
		game->rays[i] = init_ray(game->player, i);
		calc_step_dist(&game->rays[i], game->player);
		perform_dda(game, &game->rays[i]);
		calc_wall_dist(&game->rays[i], game->player);
		i ++;
	}
}
