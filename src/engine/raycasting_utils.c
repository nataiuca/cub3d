#include "cub3d.h"

void	calc_step_dist(t_ray *ray, t_player *player)
{
	if (ray->dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dx = (player->pos_x - ray->map_x) * ray->delta_dx;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dx = (ray->map_x + 1.0 - player->pos_x) * ray->delta_dx;
	}
	if (ray->dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dy = (player->pos_y - ray->map_y) * ray->delta_dy;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dy = (ray->map_y + 1.0 - player->pos_y) * ray->delta_dy;
	}
}

void	perform_dda(t_game *game, t_ray *ray)
{
	ray->hit = 0;
	while (!ray->hit)
	{
		if (ray->side_dx < ray->side_dy)
		{
			ray->side_dx += ray->delta_dx;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dy += ray->delta_dy;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (check_hit(ray, game->map))
			ray->hit = 1;
	}
}

double	calc_dist_side_0(t_ray *ray, t_player *player)
{
	double	eps;

	eps = 1e-9;
	if (fabs(ray->dir_x) < eps)
		return (HUGE_VAL);
	return ((ray->map_x - player->pos_x + (1 - ray->step_x) / 2.0)
		/ ray->dir_x);
}

double	calc_dist_side_1(t_ray *ray, t_player *player)
{
	double	eps;

	eps = 1e-9;
	if (fabs(ray->dir_y) < eps)
		return (HUGE_VAL);
	return ((ray->map_y - player->pos_y + (1 - ray->step_y) / 2.0)
		/ ray->dir_y);
}

void	calc_wall_dist(t_ray *ray, t_player *player)
{
	if (ray->side == 0)
		ray->dist = calc_dist_side_0(ray, player);
	else
		ray->dist = calc_dist_side_1(ray, player);
	if (!isfinite(ray->dist) || ray->dist <= 0.0)
		ray->line_height = WIN_HEIGHT;
	else
		ray->line_height = (int)(WIN_HEIGHT / ray->dist);
	ray->draw_start = -ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WIN_HEIGHT / 2;
	if (ray->draw_end >= WIN_HEIGHT)
		ray->draw_end = WIN_HEIGHT - 1;
}
