#include "cub3d.h"

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
