/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:03:59 by natferna          #+#    #+#             */
/*   Updated: 2025/09/23 00:04:01 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
*/
static int	is_valid_position(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;
	
	map_x = (int)x;
	map_y = (int)y;
	
	/* Verificar límites del mapa */
	if (map_x < 0 || map_x >= game->map->width || 
		map_y < 0 || map_y >= game->map->height)
		return (0);
		
	/* Verificar que no sea una pared */
	return (game->map->grid[map_y][map_x] == '0');
}

/*
** Función para mover al jugador
** - direction: 1 = adelante, -1 = atrás, 2 = derecha, -2 = izquierda
** - Verifica colisiones antes de mover
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
	else if (direction == 2) /* Derecha (D) - perpendicular a la dirección */
	{
		new_x += game->player->dir_y * move_speed;  /* Perpendicular */
		new_y -= game->player->dir_x * move_speed;
	}
	else if (direction == -2) /* Izquierda (A) */
	{
		new_x -= game->player->dir_y * move_speed;
		new_y += game->player->dir_x * move_speed;
	}
	
	/* Verificar colisiones y mover */
	if (is_valid_position(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (is_valid_position(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

/*
** Función para manejar teclas presionadas
** - Mapea cada tecla a una acción específica
** - Devuelve 0 para continuar el programa
*/
int	handle_keypress(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
	{
		printf("👋 Saliendo del juego...\n");
		cleanup_game(game);
		exit(0);
	}
	else if (keycode == KEY_W)
	{
		move_player(game, 1);  /* Adelante */
	}
	else if (keycode == KEY_S)
	{
		move_player(game, -1); /* Atrás */
	}
	else if (keycode == KEY_A)
	{
		move_player(game, -2); /* Izquierda */
	}
	else if (keycode == KEY_D)
	{
		move_player(game, 2);  /* Derecha */
	}
	else if (keycode == KEY_LEFT)
	{
		rotate_player(game, -1); /* Girar izquierda */
	}
	else if (keycode == KEY_RIGHT)
	{
		rotate_player(game, 1);  /* Girar derecha */
	}
	
	/* Redibujar después de cualquier movimiento */
	render_frame(game);
	
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
** Función principal de renderizado
** - Se llama constantemente para mantener la pantalla actualizada
** - Ejecuta el raycasting y dibuja el frame
*/
int	render_frame(t_game *game)
{
	static int test_done = 0;
	
	/* TEST: Usar MLX directo para verificar si el problema es nuestra función */
	if (!test_done)
	{
		printf("🧪 Probando dibujo directo con MLX...\n");
		
		/* Probar dibujar píxeles directamente en la ventana */
		for (int i = 0; i < 200; i++)
		{
			for (int j = 0; j < 200; j++)
			{
				mlx_pixel_put(game->mlx, game->window, i, j, 0xFF0000); /* Rojo */
			}
		}
		
		for (int i = 200; i < 400; i++)
		{
			for (int j = 200; j < 400; j++)
			{
				mlx_pixel_put(game->mlx, game->window, i, j, 0x00FF00); /* Verde */
			}
		}
		
		printf("✅ Dibujado directo completado\n");
		test_done = 1;
		return (0);
	}
	
	/* Limpiar pantalla con color negro */
	int	x = 0;
	int	y;
	
	while (x < WINDOW_WIDTH)
	{
		y = 0;
		while (y < WINDOW_HEIGHT)
		{
			my_mlx_pixel_put(game->screen, x, y, BLACK);
			y++;
		}
		x++;
	}
	
	/* Ejecutar raycasting para dibujar el mundo 3D */
	cast_rays_simple(game);
	
	/* Mostrar imagen en pantalla */
	mlx_put_image_to_window(game->mlx, game->window, game->screen->img, 0, 0);
	
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
