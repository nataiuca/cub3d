/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_cast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:11:45 by root              #+#    #+#             */
/*   Updated: 2025/10/10 12:13:05 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_minimap_cell_color(char cell)
{
	if (cell == '1') //wall
		return (0xFF7F50FF); //coral
	else if (cell == '0') //floor
		return (0x556B2FFF); //kaki
	else if (ft_strchr("NSEW", cell))
		return (0x556B2FFF); //kaki para el suelo
	return (0x222222FF); //dark grey - no deberia ocurrir - espacios
}

static t_ray	cast_ray_minimap(t_game *game, t_minimap *minimap, int i)
{
	t_ray	ray;

	ray.camera_x = 2 * i / (double)minimap->ray_count - 1;
	ray.dir_x = game->player->dir_x + game->player->plane_x * ray.camera_x;
	ray.dir_y = game->player->dir_y + game->player->plane_y * ray.camera_x;
	ray.map_x = (int)game->player->pos_x;
	ray.map_y = (int)game->player->pos_y;
	if (ray.dir_x == 0)
		ray.delta_dx = HUGE_VAL;
	else
		ray.delta_dx = fabs(1 / ray.dir_x);
	if (ray.dir_y == 0)
		ray.delta_dy = HUGE_VAL;
	else
		ray.delta_dy = fabs(1 / ray.dir_y);
	calc_step_dist(&ray, game->player);
	perform_dda(game, &ray);
	ray.hit_x = game->player->pos_x + ray.dir_x * ray.perp_wall_d;
	ray.hit_y = game->player->pos_y + ray.dir_y * ray.perp_wall_d;
	ray.hit_x_px = ray.hit_x * minimap->cell_size;
	ray.hit_y_px = ray.hit_y * minimap->cell_size;
	return(ray);
}

void	cast_all_rays_minimap(t_game *game, t_minimap *minimap)
{
	int	i;
	
	i = 0;
	while (i < minimap->ray_count)
	{
		minimap->rays[i] = cast_ray_minimap(game, minimap, i);
		i ++;
	}
}
