
#include "../../inc/cub3d.h"

/*
** Aplica oscurecimiento a paredes horizontales
** Mejora la percepción de profundidad
*/
/*int	apply_shading(int color, int side)
{
	int	r;
	int	g;
	int	b;

	if (side == 1)
	{
		r = ((color >> 16) & 0xFF) * 3 / 4;
		g = ((color >> 8) & 0xFF) * 3 / 4;
		b = (color & 0xFF) * 3 / 4;
		return ((r << 16) | (g << 8) | b);
	}
	return (color);
}*/

/*
** Dibuja techo y suelo para una columna
*/
static void	draw_floor_ceiling(t_game *game, t_ray *ray, int x)
{
	int	y;

	y = 0;
	while (y < ray->draw_start)
	{
		//fast_pixel_put(game->screen, x, y, game->map->ceiling_color);
		mlx_put_pixel(game->img, x, y, game->map->ceiling_color);
		y++;
	}
	y = ray->draw_end + 1;
	while (y < WIN_HEIGHT)
	{
		//fast_pixel_put(game->screen, x, y, game->map->floor_color);
		mlx_put_pixel(game->img, x, y, game->map->floor_color);
		y++;
	}
}

/*
** Dibuja la pared con textura para una columna
*/

static void	draw_textured_wall(t_game *game, t_ray *ray, int x, mlx_image_t *tex, int tex_x)
{
	int		y;
	int		tex_y;
	int		color;
	double	step;
	double	tex_pos;


	step = (double)tex->height / (double)ray->line_height;
	tex_pos = (ray->draw_start - WIN_HEIGHT / 2.0 + ray->line_height / 2.0) * step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= (int)tex->height)
			tex_y = tex->height - 1;
		tex_pos += step;
		color = get_tex_color(tex, tex_x, tex_y);
		//color = apply_shading(color, ray->side);
		mlx_put_pixel(game->img, x, y, color);
		y++;
	}
}

/*
** Dibuja una columna completa (techo, pared, suelo)
** Esta función se llama para cada X de la pantalla
*/
void	draw_column(t_game *game, int col, t_ray *ray)
{
	int		tex_index;
	mlx_image_t	*tex;
	double	wall_x;
	int		tex_x;

	tex_index = get_texture_index(ray);
	tex = game->textures[tex_index];
	if (!tex)
	{
		draw_floor_ceiling(game, ray, col);
		return ;
	}
	calc_texture_x(ray, game->player, &wall_x, &tex_x, tex);
	draw_floor_ceiling(game, ray, col);
	draw_textured_wall(game, ray, col, tex, tex_x);
}

void	draw_3d_view(t_game *game)
{
	int	i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		draw_column(game, i, &game->rays[i]);
		i ++;
	}
}
