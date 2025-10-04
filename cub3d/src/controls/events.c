
#include "../../inc/cub3d.h"

/*
** Función para rotar al jugador
** - Cambia la dirección en la que mira
** - También rota el plano de la cámara
** - direction: -1 = izquierda, 1 = derecha
*/
void	rotate_player(t_game *game, int direction)
{
	double	old_dir_x;
	double	old_plane_x;
	double	rot_speed;
	
	/* Determinar velocidad de rotación */
	rot_speed = game->player->rot_speed * direction;
	
	/* Rotar vector de dirección */
	old_dir_x = game->player->dir_x;
	game->player->dir_x = game->player->dir_x * cos(rot_speed) - 
						  game->player->dir_y * sin(rot_speed);
	game->player->dir_y = old_dir_x * sin(rot_speed) + 
						  game->player->dir_y * cos(rot_speed);
	
	/* Rotar plano de cámara */
	old_plane_x = game->player->plane_x;
	game->player->plane_x = game->player->plane_x * cos(rot_speed) - 
							game->player->plane_y * sin(rot_speed);
	game->player->plane_y = old_plane_x * sin(rot_speed) + 
							game->player->plane_y * cos(rot_speed);
}

/*
** Función para verificar si una posición es válida (no es una pared)
** - Convierte coordenadas del mundo a coordenadas del mapa
** - Verifica que la casilla sea '0' (espacio vacío)
** - Añade un margen de seguridad para evitar atravesar paredes en las esquinas
*/
static int	is_valid_position(t_game *game, double x, double y)
{
	int		map_x;
	int		map_y;
	double	margin;
	
	margin = 0.3; /* Margen de seguridad para el jugador */
	
	/* Verificar la posición central */
	map_x = (int)x;
	map_y = (int)y;
	
	/* Verificar límites del mapa */
	if (map_x < 0 || map_x >= game->map->width || 
		map_y < 0 || map_y >= game->map->height)
		return (0);
		
	/* Verificar que la casilla central no sea pared */
	if (game->map->grid[map_y][map_x] != '0' && 
		game->map->grid[map_y][map_x] != 'N' &&
		game->map->grid[map_y][map_x] != 'S' &&
		game->map->grid[map_y][map_x] != 'E' &&
		game->map->grid[map_y][map_x] != 'W')
		return (0);
	
	/* Verificar esquinas del jugador para evitar atravesar en diagonal */
	/* Esquina superior izquierda */
	if (game->map->grid[(int)(y - margin)][(int)(x - margin)] == '1')
		return (0);
	
	/* Esquina superior derecha */
	if (game->map->grid[(int)(y - margin)][(int)(x + margin)] == '1')
		return (0);
	
	/* Esquina inferior izquierda */
	if (game->map->grid[(int)(y + margin)][(int)(x - margin)] == '1')
		return (0);
	
	/* Esquina inferior derecha */
	if (game->map->grid[(int)(y + margin)][(int)(x + margin)] == '1')
		return (0);
	
	return (1);
}

/*
** Función para mover al jugador con detección de colisiones mejorada
** - direction: 1 = adelante, -1 = atrás, 2 = derecha, -2 = izquierda
** - Verifica colisiones antes de mover
** - Permite "deslizarse" por las paredes (sliding)
*/
void	move_player(t_game *game, int direction)
{
	double	new_x;
	double	new_y;
	double	move_speed;
	
	move_speed = game->player->move_speed;
	new_x = game->player->pos_x;
	new_y = game->player->pos_y;
	
	/* Calcular nueva posición según dirección */
	if (direction == 1) /* Adelante (W) */
	{
		new_x += game->player->dir_x * move_speed;
		new_y += game->player->dir_y * move_speed;
	}
	else if (direction == -1) /* Atrás (S) */
	{
		new_x -= game->player->dir_x * move_speed;
		new_y -= game->player->dir_y * move_speed;
	}
	else if (direction == 2) /* Derecha (D) */
	{
		new_x -= game->player->dir_y * move_speed;
		new_y += game->player->dir_x * move_speed;
	}
	else if (direction == -2) /* Izquierda (A) */
	{
		new_x += game->player->dir_y * move_speed;
		new_y -= game->player->dir_x * move_speed;
	}
	
	/* Verificar colisiones en X y Y por separado para permitir "sliding" */
	/* Intentar mover en X */
	if (is_valid_position(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	
	/* Intentar mover en Y */
	if (is_valid_position(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

/*
** Función para manejar teclas presionadas con renderizado directo
*/
int	handle_keypress(int keycode, t_game *game)
{
	static int movements = 0;
	
	if (keycode == KEY_ESC)
	{
		printf("Saliendo del juego...\n");
		cleanup_game(game);
		exit(0);
	}
	else if (keycode == KEY_W)
	{
		move_player(game, 1);
		movements++;
	}
	else if (keycode == KEY_S)
	{
		move_player(game, -1);
		movements++;
	}
	else if (keycode == KEY_A)
	{
		move_player(game, -2);
		movements++;
	}
	else if (keycode == KEY_D)
	{
		move_player(game, 2);
		movements++;
	}
	else if (keycode == KEY_LEFT)
	{
		rotate_player(game, -1);
		movements++;
	}
	else if (keycode == KEY_RIGHT)
	{
		rotate_player(game, 1);
		movements++;
	}
	
	/* Redibujar con renderizado directo */
	if (movements > 0 && movements < 50) /* Limitar redibujados para evitar lentitud */
	{
		printf("Redibujando escena (movimiento %d)...\n", movements);
		cast_rays(game);
		mlx_put_image_to_window(game->mlx, game->window, game->screen->img, 0, 0);
	}
	
	return (0);
}

/*
** Función para manejar el cierre de la ventana
** - Se llama cuando haces clic en la X de la ventana
** - Limpia memoria y sale del programa
*/
int	handle_close(t_game *game)
{
	printf("👋 Cerrando ventana...\n");
	cleanup_game(game);
	exit(0);
	return (0);
}

/*
** Raycasting directo para Mac (sin buffer de imagen)
*/
void	cast_rays_direct(t_game *game)
{
	int	x;
	static int debug_count = 0;
	
	debug_count++;
	if (debug_count == 1)
		printf("Ejecutando raycasting directo (compatible Mac)\n");
	
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		/* Calcular rayo */
		calculate_ray(game, x);
		calculate_step_and_side_dist(game);
		perform_dda(game);
		calculate_wall_distance(game);
		
		/* Dibujar directamente en ventana */
		draw_direct_line(game, x);
		
		x++;
	}
	
	if (debug_count == 1)
		printf("Raycasting completado - deberías ver el mundo 3D\n");
}

/*
** Función principal de renderizado adaptada para Mac
*/
int	render_frame(t_game *game)
{
	static int rendered = 0;
	
	/* Solo renderizar una vez inicialmente */
	if (rendered)
		return (0);
	
	printf("Renderizando mundo 3D (modo directo para Mac)...\n");
	
	/* Ejecutar raycasting directo */
	cast_rays(game);
	mlx_put_image_to_window(game->mlx, game->window, game->screen->img, 0, 0);
	
	rendered = 1;
	return (0);
}

/*
** Función para mostrar información de depuración
** - Muestra posición del jugador y FPS
** - Útil durante desarrollo
*/
void	print_debug_info(t_game *game)
{
	static int	frame_count = 0;
	
	frame_count++;
	if (frame_count % 60 == 0) /* Cada 60 frames */
	{
		printf("🎮 Jugador: (%.2f, %.2f) Dir: (%.2f, %.2f)\n",
			   game->player->pos_x, game->player->pos_y,
			   game->player->dir_x, game->player->dir_y);
	}
}
