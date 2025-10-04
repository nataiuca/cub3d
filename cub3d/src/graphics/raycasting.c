
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
	t_img	*texture;
	
	/* Determinar qué textura usar */
	tex_index = get_texture_index(game);
	texture = game->textures[tex_index];
	
	/* Verificar que la textura existe */
	if (!texture || !texture->addr)
	{
		printf("⚠️  Textura %d no disponible, usando colores sólidos\n", tex_index);
		/* Dibujar con colores sólidos como fallback */
		y = 0;
		while (y < WINDOW_HEIGHT)
		{
			if (y < game->ray->draw_start)
				my_mlx_pixel_put(game->screen, x, y, game->map->ceiling_color);
			else if (y <= game->ray->draw_end)
				my_mlx_pixel_put(game->screen, x, y, 0xFF0000); /* Rojo para debug */
			else
				my_mlx_pixel_put(game->screen, x, y, game->map->floor_color);
			y++;
		}
		return;
	}
	
	/* Calcular dónde exactamente golpeó la pared */
	if (game->ray->side == 0)
		wall_x = game->player->pos_y + game->ray->perp_wall_dist * game->ray->ray_dir_y;
	else
		wall_x = game->player->pos_x + game->ray->perp_wall_dist * game->ray->ray_dir_x;
	wall_x -= floor(wall_x); /* Obtener solo la parte decimal */
	
	/* Convertir a coordenada x de textura */
	tex_x = (int)(wall_x * (double)texture->width);
	
	/* Ajustar para orientación correcta */
	if ((game->ray->side == 0 && game->ray->ray_dir_x > 0) ||
		(game->ray->side == 1 && game->ray->ray_dir_y < 0))
		tex_x = texture->width - tex_x - 1;
	
	/* IMPORTANTE: Asegurar que tex_x está dentro de límites */
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= texture->width)
		tex_x = texture->width - 1;
	
	/* Calcular paso y posición inicial para la textura */
	step = (double)texture->height / (double)game->ray->line_height;
	tex_pos = (game->ray->draw_start - WINDOW_HEIGHT / 2.0 + 
	           game->ray->line_height / 2.0) * step;
	
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
			
			/* CORRECCIÓN PRINCIPAL: Usar (int) en lugar de bitwise AND */
			/* Esto funciona con texturas de cualquier tamaño, no solo potencias de 2 */
			tex_y = (int)tex_pos;
			
			/* Asegurar que tex_y está dentro de límites */
			if (tex_y < 0)
				tex_y = 0;
			if (tex_y >= texture->height)
				tex_y = texture->height - 1;
			
			tex_pos += step;
			
			/* Obtener color de la textura */
			color = get_texture_color(texture, tex_x, tex_y);
			
			/* Oscurecer paredes horizontales - MÉTODO MEJORADO */
			if (game->ray->side == 1)
			{
				/* Separar componentes RGB */
				int r = (color >> 16) & 0xFF;
				int g = (color >> 8) & 0xFF;
				int b = color & 0xFF;
				
				/* Reducir cada componente un 25% */
				r = (r * 3) / 4;
				g = (g * 3) / 4;
				b = (b * 3) / 4;
				
				/* Recomponer el color */
				color = (r << 16) | (g << 8) | b;
			}
			
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
	static int debug_count = 0;
	
	// DEBUG: Verificar texturas en el primer frame
	if (debug_count == 0)
	{
		printf("\n🔍 DEBUG TEXTURAS:\n");
		for (int i = 0; i < 4; i++)
		{
			printf("Textura %d: ", i);
			if (game->textures[i] == NULL)
				printf("❌ ES NULL\n");
			else if (game->textures[i]->addr == NULL)
				printf("⚠️  imagen existe pero addr es NULL\n");
			else
				printf("✅ OK (img=%p, addr=%p)\n", 
					game->textures[i]->img, 
					game->textures[i]->addr);
		}
		printf("\n");
		debug_count = 1;
	}
	
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		calculate_ray(game, x);
		calculate_step_and_side_dist(game);
		perform_dda(game);
		calculate_wall_distance(game);
		draw_textured_line(game, x);  // <-- aquí está fallando
		x++;
	}
}
