/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:02:07 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/14 16:19:08 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/time.h>
# include "libft.h"
# include <MLX42.h>

/* Constants */
/* Window/mlx values */
# define WIN_WIDTH 1024
# define WIN_HEIGHT 768

# define MINIMAP_SCALE 0.5 
# define MIN_CELL_SIZE 4
# define MAX_CELL_SIZE 32
# define MAX_MINIMAP_SIZE 300

# define TEXTURE_SIZE 64

//# define MINIMAP_SCALE 0.2

# define PI 3.14159265358979323846

# define RIGHT 1
# define LEFT -1

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


/* ========================================================================== */
# endif
/* ⬆️  ELIMINAR el #ifdef __APPLE__, #else y #endif                          */
/* ⬆️  DEJAR SOLO las defines de Linux (KEY_W 119, etc.)                     */
/* ========================================================================== */

/* Error messages */

//General error messages

# define ERR_USAGE ": Usage: ./cub3d <map.dup>"
# define ERR_FILE_EXT ": File must have .cub extension."
# define ERR_INIT ": Failed to initialize game."
# define ERR_LOAD_DATA ": Failed to load file data."
# define ERR_PARSE ": Failed to parse file. "
# define ERR_INIT_GRAPHS ": Failed to initialize graphics"

# define ERR_LOAD_TEX ": Failed to load texture :"
# define ERR_TEX_TO_IMG ": Failed to convert texture to image"
# define ERR_MLX_INIT ": mlx initialization failed"
# define ERR_MINIMAP_INIT ": Error in minimap initialization"
# define ERR_SPRITE_INIT ": Error in sprites initialization"
# define ERR_LOAD_WALL_TEX ": Error loading textures"
# define ERR_INIT_SPRITE ": Error in sprite initialization"
# define ERR_LOAD_SPRITE ": Error loading animated sprite"

//Parsing error messages

# define ERR_EMPTY_FILE ": Empty file"
# define ERR_KEY ": Unexpected configuration key in file: "
# define ERR_MISSING_CONF_KEYS ": Missing configuration keys (NO, SO, EA, WE, C, F)"

# define ERR_TEXTURE_MISSING ": Texture path is missing"
# define ERR_TEXTURE_FORMAT ": Invalid texture path format: "

# define ERR_RGB ": Invalid RGB: "
# define ERR_RGB_NUM "values must be numeric"
# define ERR_RGB_RANGE "invalid range [0, 255]"
# define ERR_RGB_COUNT "must have 3 values"

# define ERR_CHAR_MAP ": Invalid character in map: "
# define ERR_PLAYERS ": More than one player in the map"
# define ERR_NO_PLAYER ": Player not found"
# define ERR_OPEN_MAP ": Open map"
# define ERR_NO_MAP ": Map is missing"
# define ERR_AFTER_MAP ": Unexpected line after the map"

/* Colors for messages */
# define RED     "\033[0;31m"
# define GREEN   "\033[0;32m"
# define YELLOW  "\033[0;33m"
# define BLUE    "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN    "\033[0;36m"
# define RESET   "\033[0m"

/* Structures */

/**
 * @enum	t_tex_index
 * 			LOS ENUM SE SUELEN LLAMAR CON T AL PRINCIPIO??	
 */
typedef enum e_tex_index
{
	TEX_NO = 0,
	TEX_SO = 1,
	TEX_EA = 2,
	TEX_WE = 3
}	t_tex_index;

/**
 * @struct	t_player ⚠️ --> HACER DIBUJOS/ESQUEMAS PARA EXPLICAR BIEN CADA VARIABLE
 * 			- pos_x/y: coordenadas x,y del jugador, en el centro de la celda; 
 * 			corresponde al valor de línea y de posición de char dentro de
 * 			esa línea (en el grid del map) + 0.5 para ubicarlo en el centro.
 * 			Su valor se asigna en map_validator->proces_map_line() y se actualiza
 * 			con los movimientos del player
 * 			- px/y: coordenadas en píxeles para minimap. Su valor es pos_x/y
 * 			* minimap->cell_size y se asigna en draw_minimap->draw_player()
 * 			- dir: dirección en integer. Es el character que indica la posición
 * 			y orientación del player en el input ('N', 'S', 'E', 'W')
 * 			- dir_x/y: valores x,y del vector de dirección del player.
 * 			Su valor se asigna en init_player_orientation deacuerdo al valor de
 * 			dir y cambia se actualiza con los rotates del player.
 * 			- plane_x/y: plano de la cámara, define el campo de visión
 * 			del player y es perpendicular al vector de dirección. Se asigna
 * 			en init_player_orientation() y se actualiza en los rotates del player
 * 			- move_speed;
 * 			- rot_speed:
 * 			- flags para controlar si el player debe moverse o girar
 */

typedef struct s_player
{
	double	pos_x;
	double	pos_y;
	float	px;
	float	py;
	int		dir;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	move_speed;
	double	rot_speed;
	bool	move_forward;
	bool	move_backward;
	bool	move_right;
	bool	move_left;
	bool	turn_right;
	bool	turn_left;
}	t_player;

/**
 * @struct	t_ray ⚠️
 * 			- camer_x:
 * 			- dir_x:
 * 
 * 
 * 			- dist: distancia perpendicular desde el player/camera hasta la pared golpeada
 */

typedef struct s_ray
{
	double	camera_x;
	double	dir_x;
	double	dir_y;
	int		map_x;
	int		map_y;
	double	side_dx;
	double	side_dy;
	double	delta_dx;
	double	delta_dy;
	double	dist; //distancia perpendicular desde player hasta pared hiteada
	int		step_x;
	int		step_y;
	int		hit;
	double		hit_x;
	double		hit_y;
	double		hit_x_px;
	double		hit_y_px;
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
	char	*no_texture;
	char	*so_texture;
	char	*ea_texture;
	char	*we_texture;
	uint32_t	floor_color;
	uint32_t	ceiling_color;
}	t_map;

typedef struct s_minimap
{
	mlx_image_t	*img;
	int			width;
	int			height;
	int			cell_size; //igual sobra, con escala a 1 se ve bien en los ejemplos que he probado
	int			offset_x;
	int			offset_y; //pos in window
	int			ray_count;
	t_ray		*rays;
}	t_minimap;

/**
 * @struct	t_sprite
 * 			Estructura para sprites animados
 * 			- frames: array de imágenes (una por cada frame)
 * 			- frame_count: cantidad de frames totales
 * 			- curr_frame: index of the current texture frame
 * 			- last_update: tiempo del último cambio de frame
 * 			- frame_time: tiempo entre frames (en segundos)
 * 			- x/y: posición del frame en el mapa
 * 			- cam_x/y: sprite X and Y in camera space (relative horizontal
 * 			and depth (cam_y positive means front))
 * 			- screen_x: horizontal pixel coordinate on the screen
 * 
 * 			- draw_x/y: top left X,Y coordinates of the sprite on screen
 * 			- 
 * 			- width/height: sclaed sprite size 
 */

typedef struct s_sprite
{
	mlx_image_t	**frames;
	int			frame_count;
	int			curr_frame;
	double		last_update;
	double		frame_time;
	double		x;
	double		y;
	double		cam_x;
	double		cam_y;
	double		screen_x;
	int			draw_x;
	int			draw_y;
	int			width;
	int			height;
}	t_sprite;


typedef struct	s_info
{
	int		fd;
	char	**file_raw_data;
	char	**cursor;
	char	*f_color_raw;
	char	*c_color_raw;
	bool	f_color_set;
	bool	c_color_set;
	char	**map_raw;
}	t_info;

/* Estructura principal del juego */
typedef struct s_game
{
	void			*mlx;
	mlx_image_t		*img;
	mlx_image_t		*textures[4];
	t_map			*map;
	t_minimap		*minimap;
	t_player		*player;
	t_ray			*rays;
	t_info			*info;
	t_sprite		**sprite;
	int				sprite_count;
	bool			mouse_rotation_enabled;
}	t_game;

typedef struct s_draw_params
{
	t_game			*game;
	t_ray			*ray;
	int				x;
	mlx_image_t		*tex;
	int				tex_x;
}	t_draw_params;

typedef struct s_tex_calc
{
    t_ray       *ray;
    t_player    *player;
    mlx_image_t *tex;
    double      wall_x;
    int         tex_x;
}   t_tex_calc;


/* main.c */
int		main(int argc, char **argv);

/* init_structs.c */
int		init_game(t_game *game);

/* parsing/read_file.c */
int		load_info(char *file, t_info *info);

/* parsing/config_parser.c */
int		parse_config(t_game *game, t_info *info);

/* parsing/map_parser.c */
int	parse_map(t_info *info);

/* parsing/config_validator.c */
int	validate_config(t_game *game);

/* parsing/map_validator.c */
int	validate_map(t_game *game, t_info *info, t_map *map);

/* parsing/parse_utils.c */
int	skip_empty_lines(t_info *info);
int	count_rows(char **array);
int	is_map_start_line(const char *line);

/* engines/player */
void	init_player_orientation(t_player *player);
void	update_player(t_game *game, t_player *player);

/* engines/raycasting.c */
int	check_hit(t_ray *ray, t_map *map);
t_ray	init_ray(t_player *player, int col);
void	calc_step_dist(t_ray *ray, t_player *player);
void	perform_dda(t_game *game, t_ray *ray);
void	calc_wall_dist(t_ray *ray, t_player *player);
void	cast_all_rays(t_game *game);
double	calc_dist_side_0(t_ray *ray, t_player *player);
double	calc_dist_side_1(t_ray *ray, t_player *player);

/* engines/collision.c */
int		is_valid_pos(t_game *game, double x, double y);;

/* graphics/draw.c */
mlx_image_t	*load_texture(void *mlx, char *path);
int		load_textures(t_game *game);
int		init_graphics(t_game *game);
void	draw_3d_view(t_game *game);

/* graphics/textures */
int	get_tex_color(mlx_image_t *tex, int x, int y);
int	get_texture_index(t_ray *ray);
void	calc_texture_x(t_tex_calc *calc); 

/* controls/events.c - OPTIMIZADO */
void	handle_keypress(mlx_key_data_t data, void *param);
int		handle_close(t_game *game);

/* controls/moves.c - OPTIMIZADO */
void	move_forward(t_game *game, t_player *player);
void	move_backward(t_game *game, t_player *player);
void	move_left(t_game *game, t_player *player);
void	move_right(t_game *game, t_player *player);
void	rotate_player(t_player *player, int direction);
void	handle_mouse_click(mouse_key_t button, action_t action, modifier_key_t mods, void *param);
void	handle_mouse_movement(double xpos, double ypos, void *param);

/* minimap/minimap_draw.c */
int		init_minimap(t_game *game, t_minimap *minimap); 
void	draw_minimap(t_game *game);

/* minimap/minimap_cast.c */
int		get_minimap_cell_color(char cell);
void	cast_all_rays_minimap(t_game *game, t_minimap *minimap);

/* minimap/minimap_utils.c */
void	draw_square(t_minimap *minimap, int x, int y, int color);
void	clear_minimap(t_minimap *minimap);

/* sprite/init_sprite.c */
int	init_sprites(t_game *game);

/* sprite/sprite_load.c */
int	load_all_sprites(t_game *game);

/* sprite/sprite_draw.c*/
void	draw_all_sprites(t_game *game);

/* sprite/sprite_update.c */
void	update_sprite(t_sprite *sprite);
int	project_sprite(t_sprite *sprite, t_player *player);

/* utils/cleanup.c */
void	ft_free_str_array(char **array);
void	cleanup_game(t_game *game);
void	free_info(t_game *game);
void	free_map(t_game *game);
void	free_sprite(t_game *game);

/* utils/error.c */
void	error_exit(char *msg);
void	error_cleanup_exit(char *msg, t_game *game);
int		error_msg(char *msg, char *arg, int exit_code);

#endif
