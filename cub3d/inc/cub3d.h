
#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <math.h>
# include "../libft/libft.h"
# include "../minilibx/mlx.h"

/* Constantes */
# define WINDOW_WIDTH 1024
# define WINDOW_HEIGHT 768
# define TEXTURE_SIZE 64

/* ========================================================================== */
/* ⚠️  ELIMINAR PARA ENTREGA: Códigos de teclas para macOS                   */
/* ========================================================================== */
# ifdef __APPLE__
/* macOS keycodes */
#  define KEY_W 13
#  define KEY_A 0
#  define KEY_S 1
#  define KEY_D 2
#  define KEY_LEFT 123
#  define KEY_RIGHT 124
#  define KEY_ESC 53
# else
/* ========================================================================== */

/* Linux keycodes - VERSIÓN FINAL PARA ENTREGA */
#  define KEY_W 119
#  define KEY_A 97
#  define KEY_S 115
#  define KEY_D 100
#  define KEY_LEFT 65361
#  define KEY_RIGHT 65363
#  define KEY_ESC 65307

/* ========================================================================== */
# endif
/* ⬆️  ELIMINAR el #ifdef __APPLE__, #else y #endif                          */
/* ⬆️  DEJAR SOLO las defines de Linux (KEY_W 119, etc.)                     */
/* ========================================================================== */

/* Colores */
# define BLACK 0x000000
# define WHITE 0xFFFFFF

/* Estructura para imágenes/texturas */
typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;

/* Estructura del jugador */
typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	move_speed;
	double	rot_speed;
}	t_player;

/* Estructura para raycasting */
typedef struct s_ray
{
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
}	t_ray;

/* Estructura del mapa */
typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
	char	*north_texture;
	char	*south_texture;
	char	*east_texture;
	char	*west_texture;
	int		floor_color;
	int		ceiling_color;
}	t_map;

/* Estructura principal del juego */
typedef struct s_game
{
	void		*mlx;
	void		*window;
	t_img		*screen;
	t_img		*textures[4];
	t_map		*map;
	t_player	*player;
	t_ray		*ray;
}	t_game;

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

/* graphics/render.c - OPTIMIZADO */
void	render_game(t_game *game);
void	fast_pixel_put(t_img *img, int x, int y, int color);
int		get_tex_color(t_img *tex, int x, int y);

/* graphics/raycasting.c - OPTIMIZADO */
void	init_ray(t_game *game, int x);
void	calc_step_dist(t_game *game);
void	perform_dda(t_game *game);
void	calc_wall_dist(t_game *game);

/* graphics/draw.c - OPTIMIZADO */
void	draw_column(t_game *game, int x);
int		get_texture_index(t_game *game);
void	calc_texture_x(t_game *game, double *wall_x, int *tex_x, t_img *tex);

/* controls/events.c - OPTIMIZADO */
int		handle_keypress(int keycode, t_game *game);
int		handle_close(t_game *game);
void	move_player(t_game *game, int direction);
void	rotate_player(t_game *game, int direction);

/* controls/movement.c - OPTIMIZADO */
int		is_valid_pos(t_game *game, double x, double y);
void	move_forward(t_game *game);
void	move_backward(t_game *game);
void	move_left(t_game *game);
void	move_right(t_game *game);

/* utils/cleanup.c */
void	cleanup_game(t_game *game);
void	free_map(t_map *map);

/* utils/error.c */
void	error_exit(char *message);
void	error_cleanup_exit(char *message, t_game *game);

#endif
