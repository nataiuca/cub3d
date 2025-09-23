
#include "../../inc/cub3d.h"

/*
** Función para inicializar un rayo para una columna de pantalla
** - x: columna de pantalla (0 a WINDOW_WIDTH-1)
** - Calcula la dirección del rayo basado en la posición de la columna
*/
void	calculate_ray(t_game *game, int x)
{
	/* camera_x va de -1 a +1, donde 0 es el centro de la pantalla */
	game->ray->camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	
	/* Calcular dirección del rayo */
	game->ray->ray_dir_x = game->player->dir_x + game->player->plane_x * game->ray->camera_x;
	game->ray->ray_dir_y = game->player->dir_y + game->player->plane_y * game->ray->camera_x;
	
	/* Posición actual en el mapa */
	game->ray->map_x = (int)game->player->pos_x;
	game->ray->map_y = (int)game->player->pos_y;
	
	/* Calcular distancia que el rayo tiene que viajar para ir de un lado x o y al siguiente */
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
** Función para calcular el paso y la distancia inicial al lado
** - Determina en qué dirección avanzar (step_x, step_y)
** - Calcula la distancia inicial al próximo lado del mapa
*/
void	calculate_step_and_side_dist(t_game *game)
{
	/* Calcular step y side_dist inicial */
	if (game->ray->ray_dir_x < 0)
	{
		game->ray->step_x = -1;
		game->ray->side_dist_x = (game->player->pos_x - game->ray->map_x) * game->ray->delta_dist_x;
	}
	else
	{
		game->ray->step_x = 1;
		game->ray->side_dist_x = (game->ray->map_x + 1.0 - game->player->pos_x) * game->ray->delta_dist_x;
	}
	
	if (game->ray->ray_dir_y < 0)
	{
		game->ray->step_y = -1;
		game->ray->side_dist_y = (game->player->pos_y - game->ray->map_y) * game->ray->delta_dist_y;
	}
	else
	{
		game->ray->step_y = 1;
		game->ray->side_dist_y = (game->ray->map_y + 1.0 - game->player->pos_y) * game->ray->delta_dist_y;
	}
}

/*
** Algoritmo DDA (Digital Differential Analyzer)
** - Avanza el rayo paso a paso hasta que golpea una pared
** - Es como "caminar" por la grilla del mapa siguiendo el rayo
*/
void	perform_dda(t_game *game)
{
	game->ray->hit = 0;
	
	while (game->ray->hit == 0)
	{
		/* Saltar al próximo lado del mapa, ya sea en dirección x o y */
		if (game->ray->side_dist_x < game->ray->side_dist_y)
		{
			/* Próximo lado está en dirección x */
			game->ray->side_dist_x += game->ray->delta_dist_x;
			game->ray->map_x += game->ray->step_x;
			game->ray->side = 0; /* Golpeamos un lado vertical (Norte/Sur) */
		}
		else
		{
			/* Próximo lado está en dirección y */
			game->ray->side_dist_y += game->ray->delta_dist_y;
			game->ray->map_y += game->ray->step_y;
			game->ray->side = 1; /* Golpeamos un lado horizontal (Este/Oeste) */
		}
		
		/* Verificar si hemos golpeado una pared */
		if (game->ray->map_x >= 0 && game->ray->map_x < game->map->width &&
			game->ray->map_y >= 0 && game->ray->map_y < game->map->height &&
			game->map->grid[game->ray->map_y][game->ray->map_x] == '1')
		{
			game->ray->hit = 1;
		}
		
		/* Evitar loops infinitos si salimos del mapa */
		if (game->ray->map_x < 0 || game->ray->map_x >= game->map->width ||
			game->ray->map_y < 0 || game->ray->map_y >= game->map->height)
		{
			game->ray->hit = 1;
		}
	}
}

/*
** Función para calcular la distancia a la pared y la altura de línea
** - Calcula qué tan lejos está la pared
** - Determina qué tan alta debe ser la línea dibujada en pantalla
*/
void	calculate_wall_distance(t_game *game)
{
	/* Calcular distancia del rayo perpendicular a la pared */
	if (game->ray->side == 0)
	{
		/* Lado vertical (Norte/Sur) */
		game->ray->perp_wall_dist = (game->ray->map_x - game->player->pos_x + 
									 (1 - game->ray->step_x) / 2) / game->ray->ray_dir_x;
	}
	else
	{
		/* Lado horizontal (Este/Oeste) */
		game->ray->perp_wall_dist = (game->ray->map_y - game->player->pos_y + 
									 (1 - game->ray->step_y) / 2) / game->ray->ray_dir_y;
	}
	
	/* Calcular altura de la línea a dibujar en pantalla */
	if (game->ray->perp_wall_dist == 0)
		game->ray->line_height = WINDOW_HEIGHT;
	else
		game->ray->line_height = (int)(WINDOW_HEIGHT / game->ray->perp_wall_dist);
	
	/* Calcular píxeles más bajo y más alto para dibujar en esta columna */
	game->ray->draw_start = -game->ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (game->ray->draw_start < 0)
		game->ray->draw_start = 0;
		
	game->ray->draw_end = game->ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (game->ray->draw_end >= WINDOW_HEIGHT)
		game->ray->draw_end = WINDOW_HEIGHT - 1;
}

/*
** Función para determinar qué textura usar según qué pared golpeamos
** - Norte = 0, Sur = 1, Este = 2, Oeste = 3
** - Basado en la dirección del rayo y qué lado golpeó
*/
static int	get_texture_index(t_game *game)
{
	if (game->ray->side == 0) /* Lado vertical (Norte/Sur) */
	{
		if (game->ray->step_x > 0)
			return (2); /* Este */
		else
			return (3); /* Oeste */
	}
	else /* Lado horizontal (Este/Oeste) */
	{
		if (game->ray->step_y > 0)
			return (1); /* Sur */
		else
			return (0); /* Norte */
	}
}

/*
** Función para dibujar una columna vertical con texturas
** - x: columna de pantalla a dibujar
** - Dibuja suelo, pared con textura, y techo
*/
static void	draw_textured_line(t_game *game, int x)
{
	int		y;
	int		tex_index;
	double	wall_x;
	int		tex_x;
	int		tex_y;
	double	step;
	double	tex_pos;
	int		color;
	
	/* Determinar qué textura usar */
	tex_index = get_texture_index(game);
	
	/* Calcular dónde exactamente golpeó la pared */
	if (game->ray->side == 0)
		wall_x = game->player->pos_y + game->ray->perp_wall_dist * game->ray->ray_dir_y;
	else
		wall_x = game->player->pos_x + game->ray->perp_wall_dist * game->ray->ray_dir_x;
	wall_x -= floor(wall_x); /* Obtener solo la parte decimal */
	
	/* Convertir a coordenada x de textura */
	tex_x = (int)(wall_x * game->textures[tex_index]->width);
	if ((game->ray->side == 0 && game->ray->ray_dir_x > 0) ||
		(game->ray->side == 1 && game->ray->ray_dir_y < 0))
		tex_x = game->textures[tex_index]->width - tex_x - 1;
	
	/* Calcular paso y posición inicial para la textura */
	step = 1.0 * game->textures[tex_index]->height / game->ray->line_height;
	tex_pos = (game->ray->draw_start - WINDOW_HEIGHT / 2 + game->ray->line_height / 2) * step;
	
	/* Dibujar la columna píxel por píxel */
	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		if (y < game->ray->draw_start)
		{
			/* Dibujar techo */
			my_mlx_pixel_put(game->screen, x, y, game->map->ceiling_color);
		}
		else if (y >= game->ray->draw_start && y <= game->ray->draw_end)
		{
			/* Dibujar pared con textura */
			tex_y = (int)tex_pos & (game->textures[tex_index]->height - 1);
			tex_pos += step;
			
			/* Obtener color de la textura */
			color = get_texture_color(game->textures[tex_index], tex_x, tex_y);
			
			/* Oscurecer paredes horizontales para dar efecto de profundidad */
			if (game->ray->side == 1)
				color = (color >> 1) & 8355711; /* Dividir RGB por 2 */
			
			my_mlx_pixel_put(game->screen, x, y, color);
		}
		else
		{
			/* Dibujar suelo */
			my_mlx_pixel_put(game->screen, x, y, game->map->floor_color);
		}
		y++;
	}
}

/*
** Función principal de raycasting
** - Para cada columna de pantalla, lanza un rayo y dibuja el resultado
** - Esta es la función que se llama cada frame para renderizar la escena
*/
void	cast_rays(t_game *game)
{
	int	x;
	static int first_time = 1;
	
	if (first_time)
	{
		printf("🎯 Iniciando raycasting...\n");
		first_time = 0;
	}
	
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		/* Paso 1: Calcular dirección del rayo */
		calculate_ray(game, x);
		
		/* Paso 2: Calcular paso y distancia inicial */
		calculate_step_and_side_dist(game);
		
		/* Paso 3: Ejecutar DDA */
		perform_dda(game);
		
		/* Paso 4: Calcular distancia a la pared */
		calculate_wall_distance(game);
		
		/* Paso 5: Dibujar la columna */
		draw_textured_line(game, x);
		
		x++;
	}
}
