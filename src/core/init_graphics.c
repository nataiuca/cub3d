
#include "cub3d.h"

/*
** Función para cargar una textura desde un archivo
** - Carga una imagen .png usando mlx_load_png

*/
static mlx_image_t	*load_texture(void *mlx, char *path)
{
	//xpm_t	*tex;
	mlx_texture_t	*tex;
	mlx_image_t	*img;

	//tex = mlx_load_xpm42(path);
	tex = mlx_load_png(path);
	if (!tex)
	{
		error_msg(ERR_LOAD_TEX, path, 0); //MLX establece su propio errno? MIRAR ERROR_MLX
		return (NULL);
	}
	printf("Textura cargada correctamenteo: \n");
	printf("Dimensiones: %dx%d pixeles\n", tex->width, tex->height);
	img = mlx_texture_to_image(mlx, tex);
	if (!img)
	{
		error_msg(NULL, NULL, 0);
		mlx_delete_texture(tex);
	}
	mlx_delete_texture(tex);

	/* Verificar si las dimensiones son potencia de 2 */
	/*if ((img->width & (img->width - 1)) != 0)
		printf("   ℹ️  Ancho no es potencia de 2 (está OK con las correcciones)\n");
	if ((img->height & (img->height - 1)) != 0)
		printf("   ℹ️  Alto no es potencia de 2 (está OK con las correcciones)\n");
	*/ //CREO QUE ESTO NO ES NECESARIO, NO AL MENOS PARA MLX42, YA VEREMOS MÁS TARDE
	//SI ES NECESARIO PARA EL CÓDIGO
	return (img);
}

/*
** Función para cargar todas las texturas del mapa
** - Carga las 4 texturas de las pa redes (N, S, E, W)
** - Las guarda en el array de texturas del juego
*/
int	load_textures(t_game *game)
{
	game->textures[TEX_NO] = load_texture(game->mlx, game->map->no_texture);
	if (!game->textures[0])
		return (0);
	game->textures[TEX_SO] = load_texture(game->mlx, game->map->so_texture);
	if (!game->textures[1])
		return (0);
	game->textures[TEX_EA] = load_texture(game->mlx, game->map->ea_texture);
	if (!game->textures[2])
		return (0);
	game->textures[TEX_WE] = load_texture(game->mlx, game->map->we_texture);
	if (!game->textures[3])
		return (0);
	return (1);
}

/**
 * @brief	se encarga de la interacción con la libreria gráfica
 * 			mlx_init(), mlx_new_img, mlx_image_to_window
 * 
 * @param
 */

int	init_graphics(t_game *game)
{
	game->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return(error_msg(ERR_MLX_INIT, NULL, 0));
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
		return(error_msg(NULL, NULL, 0)); //creo que si falla mlx_new_img ya establece su propio error -> COMPROBAR
	if (!init_minimap(game, game->minimap))
		return (error_msg("Error in minimap initialization.", NULL, 0));
	if (!load_textures(game))
		return(error_msg("Error loading textures.", NULL, 0));
	if (mlx_image_to_window(game->mlx, game->img, 0, 0) < 0)
		return (error_msg(NULL, NULL, 0));
	if (mlx_image_to_window(game->mlx, game->minimap->img, game->minimap->offset_x, game->minimap->offset_y) < 0)
		return (error_msg(NULL, NULL, 0));
	return (1);
}
