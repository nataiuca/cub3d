/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amacarul <amacarul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 10:02:07 by amacarul          #+#    #+#             */
/*   Updated: 2025/10/09 17:43:40 by amacarul         ###   ########.fr       */
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
# include "libft.h"
# include <MLX42.h>

/* Constants */
/* Window/mlx values */
# define WIN_WIDTH 1024
# define WIN_HEIGHT 768

# define TEXTURE_SIZE 64

//# define PI 3.14159265358979323846


# define RIGHT 1
# define LEFT -1

# define MOVE_SPEED 0.1
# define ROT_SPEED 0.08

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

# define ERR_USAGE ": Usage: ./cub3d <map.dup>"
# define ERR_FILE_EXT ": File must have .cub extension."

# define ERR_INIT ": Failed to initialize game."
# define ERR_LOAD_DATA ": Failed to load file data."

# define ERR_LOAD_TEX ": Failed to load texture :"
# define ERR_TEX_TO_IMG ": Failed to convert texture to image"
# define ERR_MLX_INIT ": mlx initialization failed"
# define ERR_IMG_MINIMAP ": The minimap image could not be created"
# define ERR_INIT_GRAPHS ": Failed to initialize graphics"


# define ERR_MALLOC ": Malloc failed"

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

# define ERR_PARSE ": Failed to parse file. "


//Parsing error


/* Colors for msgs */
# define RED "\033[0;31m"
# define RESET "\033[0m"

/* Colors */
# define BLACK 0x000000
# define WHITE 0xFFFFFF



/* Structures */

typedef enum e_tex_index
{
	TEX_NO = 0,
	TEX_SO = 1,
	TEX_EA = 2,
	TEX_WE = 3
}	t_tex_index;

/* Estructura para imágenes/texturas */
/*typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;*/

/* Estructura del jugador */
typedef struct s_player
{
	double	pos_x; //coordenadas en celdas ?
	double	pos_y;
	float	px; //coordeandas en pixeles para minimap
	float	py; //coordenadas en píxeles para minimap

	int		dir; //dirección en int
	double	dir_x; //dirección en x
	double	dir_y; // dirección en y
	double	plane_x;
	double	plane_y;

	float	angle; //ángulo de visión/orientación en radianes
	
	//double	move_speed;
	//double	rot_speed;

	bool	move_forward;
	bool	move_backward;
	bool	move_right;
	bool	move_left;
	bool	turn_right;
	bool	turn_left;
}	t_player;

/* Estructura para raycasting */
typedef struct s_ray
{
	double	camera_x;
	double	dir_x;
	double	dir_y;
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
	char	*no_texture;
	char	*so_texture;
	char	*ea_texture;
	char	*we_texture;
	int		floor_color;
	int		ceiling_color;
}	t_map;

typedef struct s_minimap
{
	mlx_image_t	*img;
	int			cell_size; //igual sobra, con escala a 1 se ve bien en los ejemplos que he probado
	int			offset_x;
	int			offset_y; //pos in window
	
}	t_minimap;

typedef struct	s_mapinfo
{
	int		fd;
	char	**file_raw_data;
	char	**cursor;

	/*char	*no_path;
	char	*so_path;
	char	*ea_path;
	char	*we_path;*/

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
	void			*mlx;
	mlx_image_t		*img;
	mlx_image_t		*textures[4];
	t_map			*map;
	t_player		*player;
	t_ray			*rays;
	t_mapinfo		*mapinfo;
}	t_game;

/* main.c */
int		main(int argc, char **argv);

/* init_structs.c */
int	init_game(t_game *game);

/* parsing/read_file.c */
int		load_mapinfo(char *file, t_mapinfo *mapinfo);

/* parsing/config_parser.c */
int	parse_config(t_game *game, t_mapinfo *mapinfo);

/* parsing/map_parser.c */
int	parse_map(t_mapinfo *mapinfo);

/* parsing/config_validator.c */
int	validate_config(t_game *game);

/* parsing/map_validator.c */
int	validate_map(t_game *game, t_mapinfo *mapinfo, t_map *map);

/* parsing/parse_utils.c */
int	skip_empty_lines(t_mapinfo *mapinfo);
int	is_map_start_line(const char *line);

/* engine/player */
void	init_player_orientation(t_player *player);
void	update_player(t_game *game, t_player *player);

/* engine/raycasting.c */
void	cast_all_rays(t_game *game);

/* graphics/init_mlx.c */
int		init_graphics(t_game *game);
int		load_textures(t_game *game);

/* graphics/render.c - OPTIMIZADO */
//void	render_game(t_game *game);
void	fast_pixel_put(mlx_image_t *img, int x, int y, int color);
int		get_tex_color(mlx_image_t *tex, int x, int y);

/* graphics/draw_3d.c - OPTIMIZADO */
void	draw_3d_view(t_game *game);

/* graphics/draw_minimap.c */


/* controls/events.c - OPTIMIZADO */
void	handle_keypress(mlx_key_data_t data, void *param);
int		handle_close(t_game *game);
//void	move_player(t_game *game, int direction);


/* controls/movement.c - OPTIMIZADO */
int		is_valid_pos(t_game *game, double x, double y);
void	move_forward(t_game *game);
void	move_backward(t_game *game);
void	move_left(t_game *game);
void	move_right(t_game *game);
void	rotate_player(t_game *game, int direction);

/* utils/cleanup.c */
void	ft_free_str_array(char **array);
void	cleanup_game(t_game *game);
void	free_map(t_map *map);

/* utils/error.c */
void	error_exit(char *msg);
void	error_cleanup_exit(char *msg, t_game *game);
int	error_msg(char *msg, char *arg, int exit_code);

#endif
