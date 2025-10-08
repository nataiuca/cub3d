
#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <stdbool.h>
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

/* Error messages */

# define ERR_USAGE ": Usage: ./cub3d <map.dup>"
# define ERR_FILE_EXT ": File must have .cub extension."

# define ERR_INIT ": Faied to initialize game."
# define ERR_LOAD_DATA ": Failed loading file data."
# define ERR_PARSE ": Failed to parse file. "

# define ERR_MALLOC ": Malloc failed"

# define ERR_MLX_INIT ": mlx initialization failed"

//Parsing error

# define ERR_EMPTY_FILE ": Empty file"
# define ERR_KEY ": Unexpected configuration key in file: "
# define ERR_MISSING_CONF_KEYS ": Missing configuration keys (NO, SO, EA, WE, C, F)"
# define ERR_TEXTURE_MISSING ": Texture path is missing"
# define ERR_TEXTURE_FORMAT ": Invalid texture path format: "

# define ERR_RGB ": Invalid RGB: "
# define ERR_RGB_NUM "values must be numeric"
# define ERR_RGB_RANGE "invalid range [0, 255]"
# define ERR_RGB_COUNT "must have 3 values"

# define ERR_CHAR_MAP ": Invalid character in map"
# define ERR_EMPTY_LINE ": Empty line in map"
# define ERR_MAP_SHAPE ": The map is not rectangular"
# define ERR_NO_PLAYER ": Player not found"
# define ERR_PLAYERS ": More than one player in the map"
# define ERR_OPEN_MAP ": Open map"
# define ERR_NO_MAP ": Map is missing"
# define ERR_AFTER_MAP ": Unexpected line after the map"

# define ERR_IMG_MINIMAP ": The minimap image could not be created"

//Error codes
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1 //error generico: malloc, open...
# define EXIT_SYNTAX 2 //errores de sintaxis, formato, parsing... NECESITO OTRO NOMBRE PARA ESTO

/* Colors */
# define BLACK 0x000000
# define WHITE 0xFFFFFF

/* Structures */

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
	int		dir;
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

typedef struct	s_mapinfo
{
	int		fd;
	char	**file_raw_data;
	char	**cursor;

	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;

	char	*f_color_raw;
	char	*c_color_raw;
	bool	f_color_set;
	bool	c_color_set;

	int		f_color[3]; //NO SE SI ES NECESARIO TENER LOS RGB EN INT PORQUE IGUAL HAY QUE PASAR A OTRO FORMATO
	int		c_color[3];

	char	**map_raw;

}	t_mapinfo;

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
	t_mapinfo	*mapinfo;
}	t_game;

/* main.c */
int		main(int argc, char **argv);

/* init_structs.c */
int	init_game(t_game *game);

/* parsing/read_file.c */
int		load_mapinfo(char *file, t_mapinfo *mapinfo);

/* parsing/config_parser.c */
int	parse_config(t_mapinfo *mapinfo);

/* parsing/map_parser.c */
int	parse_map(t_mapinfo *mapinfo);

/* parsing/config_validator.c */
int	validate_config(t_mapinfo *mapinfo);

/* parsing/map_validator.c */
int	validate_map(t_game *game, t_mapinfo *mapinfo, t_map *map);

/* parsing/parse_file.c */
//int		parse_file(char *filename, t_game *game);
//void	init_map(t_map *map);

/* parsing/parse_map.c */
//int		validate_map_from_file(char *filename, t_game *game);
//int		is_valid_char(char c);
//int		find_player(t_game *game);

/* parsing/parse_utils.c */
int	skip_empty_lines(t_mapinfo *mapinfo);
int	is_map_start_line(const char *line);

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
void	ft_free_str_array(char **array);
void	cleanup_game(t_game *game);
void	free_map(t_map *map);

/* utils/error.c */
void	error_exit(char *msg);
void	error_cleanup_exit(char *msg, t_game *game);
int	error_msg(char *msg, char *arg, int exit_code);

#endif
