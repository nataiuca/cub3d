/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 23:52:48 by natferna          #+#    #+#             */
/*   Updated: 2025/09/23 00:10:28 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include "libft.h"
# include <mlx.h>

/* Constantes para la ventana y el juego */
# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 768
# define TEXTURE_SIZE 64

/* Códigos de teclas (pueden variar según el sistema) */
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define KEY_ESC 53

/* Colores por defecto */
# define BLACK 0x000000
# define WHITE 0xFFFFFF

/* Estructura para manejar imágenes/texturas */
typedef struct s_img
{
	void	*img;           /* Puntero a la imagen en MLX */
	char	*addr;          /* Dirección de memoria de la imagen */
	int		bits_per_pixel; /* Bits por píxel */
	int		line_length;    /* Longitud de línea */
	int		endian;         /* Orden de bytes */
	int		width;          /* Ancho de la imagen */
	int		height;         /* Alto de la imagen */
}	t_img;

/* Estructura para la posición del jugador */
typedef struct s_player
{
	double	pos_x;          /* Posición X del jugador */
	double	pos_y;          /* Posición Y del jugador */
	double	dir_x;          /* Dirección X (vector) */
	double	dir_y;          /* Dirección Y (vector) */
	double	plane_x;        /* Plano de cámara X */
	double	plane_y;        /* Plano de cámara Y */
	double	move_speed;     /* Velocidad de movimiento */
	double	rot_speed;      /* Velocidad de rotación */
}	t_player;

/* Estructura para los datos del raycasting */
typedef struct s_ray
{
	double	camera_x;       /* Posición X en el plano de cámara */
	double	ray_dir_x;      /* Dirección X del rayo */
	double	ray_dir_y;      /* Dirección Y del rayo */
	int		map_x;          /* Posición X en el mapa */
	int		map_y;          /* Posición Y en el mapa */
	double	side_dist_x;    /* Distancia al próximo lado X */
	double	side_dist_y;    /* Distancia al próximo lado Y */
	double	delta_dist_x;   /* Distancia entre líneas X */
	double	delta_dist_y;   /* Distancia entre líneas Y */
	double	perp_wall_dist; /* Distancia perpendicular a la pared */
	int		step_x;         /* Dirección de paso X (-1 o 1) */
	int		step_y;         /* Dirección de paso Y (-1 o 1) */
	int		hit;            /* ¿Se golpeó una pared? */
	int		side;           /* ¿Qué lado de la pared? (0=NS, 1=EW) */
	int		line_height;    /* Altura de línea a dibujar */
	int		draw_start;     /* Píxel inicial para dibujar */
	int		draw_end;       /* Píxel final para dibujar */
}	t_ray;

/* Estructura para el mapa */
typedef struct s_map
{
	char	**grid;         /* Grid del mapa (array 2D) */
	int		width;          /* Ancho del mapa */
	int		height;         /* Alto del mapa */
	char	*north_texture; /* Ruta texture norte */
	char	*south_texture; /* Ruta texture sur */
	char	*east_texture;  /* Ruta texture este */
	char	*west_texture;  /* Ruta texture oeste */
	int		floor_color;    /* Color del suelo */
	int		ceiling_color;  /* Color del techo */
}	t_map;

/* Estructura principal del juego */
typedef struct s_game
{
	void		*mlx;           /* Puntero a MLX */
	void		*window;        /* Puntero a la ventana */
	t_img		*screen;        /* Imagen de la pantalla */
	t_img		*textures[4];   /* Array de texturas [N,S,E,W] */
	t_map		*map;           /* Datos del mapa */
	t_player	*player;        /* Datos del jugador */
	t_ray		*ray;           /* Datos del raycasting */
}	t_game;

/* Funciones principales */
/* main.c */
int		main(int argc, char **argv);

/* parsing/parse_file.c */
int		parse_file(char *filename, t_game *game);
void	init_map(t_map *map);

/* parsing/parse_map.c */
int		validate_map_from_file(char *filename, t_game *game);
int		is_valid_char(char c);
int		find_player(t_game *game);

/* graphics/init_mlx.c */
int		init_graphics(t_game *game);
int		load_textures(t_game *game);
int		get_texture_color(t_img *texture, int x, int y);
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);

/* graphics/raycasting.c */
void	cast_rays(t_game *game);
void	cast_rays_simple(t_game *game);
void	calculate_ray(t_game *game, int x);
void	perform_dda(t_game *game);
void	calculate_wall_distance(t_game *game);
void	calculate_step_and_side_dist(t_game *game);

/* graphics/draw.c */
void	draw_frame(t_game *game);
void	draw_vertical_line(t_game *game, int x);

/* controls/events.c */
int		handle_keypress(int keycode, t_game *game);
int		handle_close(t_game *game);
int		render_frame(t_game *game);
void	move_player(t_game *game, int direction);
void	rotate_player(t_game *game, int direction);
void	print_debug_info(t_game *game);

/* utils/cleanup.c */
void	cleanup_game(t_game *game);
void	free_map(t_map *map);

/* utils/error.c */
void	error_exit(char *message);
void	error_cleanup_exit(char *message, t_game *game);

#endif