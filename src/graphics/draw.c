
#include "../../inc/cub3d.h"

/*
** Determina qué textura usar según la pared golpeada
** Norte=0, Sur=1, Este=2, Oeste=3
*/
int	get_texture_index(t_game *game)
{
	if (game->ray->side == 0)
	{
		if (game->ray->step_x > 0)
			return (2);
		else
			return (3);
	}
	else
	{
		if (game->ray->step_y > 0)
			return (1);
		else
			return (0);
	}
}

/*
** Calcula coordenada X de la textura
** wall_x: posición exacta donde el rayo golpeó la pared
*/
void	calc_texture_x(t_game *game, double *wall_x, int *tex_x, t_img *tex)
{
	if (game->ray->side == 0)
		*wall_x = game->player->pos_y + game->ray->perp_wall_dist
			* game->ray->ray_dir_y;
	else
		*wall_x = game->player->pos_x + game->ray->perp_wall_dist
			* game->ray->ray_dir_x;
	*wall_x -= floor(*wall_x);
	*tex_x = (int)(*wall_x * (double)tex->width);
	if ((game->ray->side == 0 && game->ray->ray_dir_x > 0)
		|| (game->ray->side == 1 && game->ray->ray_dir_y < 0))
		*tex_x = tex->width - *tex_x - 1;
	if (*tex_x < 0)
		*tex_x = 0;
	if (*tex_x >= tex->width)
		*tex_x = tex->width - 1;
}

/*
** Aplica oscurecimiento a paredes horizontales
** Mejora la percepción de profundidad
*/
static int	apply_shading(int color, int side)
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
}

/*
** Dibuja techo y suelo para una columna
*/
static void	draw_floor_ceiling(t_game *game, int x)
{
	int	y;

	y = 0;
	while (y < game->ray->draw_start)
	{
		fast_pixel_put(game->screen, x, y, game->map->ceiling_color);
		y++;
	}
	y = game->ray->draw_end + 1;
	while (y < WIN_HEIGHT)
	{
		fast_pixel_put(game->screen, x, y, game->map->floor_color);
		y++;
	}
}

/*
** Dibuja la pared con textura para una columna
*/
static void	draw_textured_wall(t_game *game, int x, t_img *tex, int tex_x)
{
	int		y;
	int		tex_y;
	int		color;
	double	step;
	double	tex_pos;

	step = (double)tex->height / (double)game->ray->line_height;
	tex_pos = (game->ray->draw_start - WIN_HEIGHT / 2.0
			+ game->ray->line_height / 2.0) * step;
	y = game->ray->draw_start;
	while (y <= game->ray->draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex->height)
			tex_y = tex->height - 1;
		tex_pos += step;
		color = get_tex_color(tex, tex_x, tex_y);
		color = apply_shading(color, game->ray->side);
		fast_pixel_put(game->screen, x, y, color);
		y++;
	}
}

/*
** Dibuja una columna completa (techo, pared, suelo)
** Esta función se llama para cada X de la pantalla
*/
void	draw_column(t_game *game, int x)
{
	int		tex_index;
	t_img	*tex;
	double	wall_x;
	int		tex_x;

	tex_index = get_texture_index(game);
	tex = game->textures[tex_index];
	if (!tex || !tex->addr)
	{
		draw_floor_ceiling(game, x);
		return ;
	}
	calc_texture_x(game, &wall_x, &tex_x, tex);
	draw_floor_ceiling(game, x);
	draw_textured_wall(game, x, tex, tex_x);
}
