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
	int	y;
	int	y;

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
		mlx_put_pixel(game->img, x, y, game->map->ceiling_color);
		y++;
	}
	y = ray->draw_end + 1;
	while (y < WIN_HEIGHT)
	{
		mlx_put_pixel(game->img, x, y, game->map->floor_color);
		y++;
	}
}

/*
** Dibuja la pared con textura para una columna
*/

static void	draw_textured_wall(t_draw_params *params)
{
	int		y;
	int		tex_y;
	int		color;
	double	step;
	double	tex_pos;

	step = (double)params->tex->height / (double)params->ray->line_height;
	tex_pos = (params->ray->draw_start - WIN_HEIGHT / 2.0
			+ params->ray->line_height / 2.0) * step;
	y = params->ray->draw_start;
	while (y <= params->ray->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= (int)params->tex->height)
			tex_y = params->tex->height - 1;
		tex_pos += step;
		color = get_tex_color(params->tex, params->tex_x, tex_y);
		mlx_put_pixel(params->game->img, params->x, y, color);
		y++;
	}
}

/*
** Dibuja una columna completa (techo, pared, suelo)
** Esta función se llama para cada X de la pantalla
*/
static void	init_tex_calc(t_tex_calc *calc, t_ray *ray, t_player *player,
		mlx_image_t *tex)
{
	calc->ray = ray;
	calc->player = player;
	calc->tex = tex;
	calc->wall_x = 0.0;
	calc->tex_x = 0;
	calc_texture_x(calc);
}

void	draw_column(t_game *game, int col, t_ray *ray)
{
	int				tex_index;
	mlx_image_t		*tex;
	t_tex_calc		calc;
	t_draw_params	params;

	tex_index = get_texture_index(ray);
	tex = game->textures[tex_index];
	if (!tex)
	{
		draw_floor_ceiling(game, ray, col);
		return ;
	}
	init_tex_calc(&calc, ray, game->player, tex);
	draw_floor_ceiling(game, ray, col);
	params.game = game;
	params.ray = ray;
	params.tex = tex;
	params.tex_x = calc.tex_x;
	params.x = col;
	draw_textured_wall(&params);
}

void	draw_3d_view(t_game *game)
{
	int	i;

	i = 0;
	while (i < WIN_WIDTH)
	{
		draw_column(game, i, &game->rays[i]);
		i++;
	}
}
