
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
	game->textures[0] = load_texture(game->mlx, game->map->no_texture);
	if (!game->textures[0])
		return (0);
	game->textures[1] = load_texture(game->mlx, game->map->so_texture);
	if (!game->textures[1])
		return (0);
	game->textures[2] = load_texture(game->mlx, game->map->ea_texture);
	if (!game->textures[2])
		return (0);
	game->textures[3] = load_texture(game->mlx, game->map->we_texture);
	if (!game->textures[3])
		return (0);
	return (1);
}

/*
** Función principal para inicializar los gráficos
** - Inicializa MLX (con la mlx42 ya se crea la ventana)
** - Carga texturas
** - Crea imagen para dibujar
** - Configura eventos
*/
int	init_graphics(t_game *game)
{
	game->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3D", false);
	if (!game->mlx)
		return(error_msg(ERR_MLX_INIT, NULL, 0));
	game->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!game->img)
		return(error_msg(NULL, NULL, 0)); //creo que si falla mlx_new_img ya establece su propio error -> COMPROBAR
	if (!load_textures(game))
		return(0);
	mlx_key_hook(game->mlx, handle_keypress, game); //NO SE SI ES MEJOR QUE ESTAS FUNCIONES VAYAN AQUÍ O EN EL MAIN
	//mlx_hook(game->mlx, handle_close, game);
	
	/* NO configurar loop de render automático por ahora */
	/* mlx_loop_hook(game->mlx, render_frame, game); */

	return (1);
}

/*
** Función para obtener el color de un píxel de una textura
** - Convierte coordenadas de textura a color
** - Útil para el raycasting

mlx_image_t->pixels: buffer de la imagen (cada pixel 4 bytes de rgba)
el buffer pixel tiene todos los píxeles en formato rgba
cada pixel son 4 uint8_t: r, g, b, a
el pixel (x,y) está en la posición index = (y * width + x) * 4

*/
/*int	get_texture_color(mlx_image_t *texture, int x, int y)
{
	uint8_t		*pixels;
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint8_t		a;
	uint32_t	index;
	int			debug_count;

	debug_count = 0;
	
	if (!texture || !texture->pixels)
	{
		if (debug_count++ < 5)
			printf("❌ Textura nula en get_texture_color\n");
		return (0xFF00FF); 
	}

	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
	{
		if (debug_count++ < 5)
			printf("⚠️  Coordenadas fuera de límites: (%d,%d) max:(%d,%d)\n",
			       x, y, texture->width, texture->height);

		if (x < 0) x = 0;
		if (x >= (int)texture->width) x = texture->width - 1;
		if (y < 0) y = 0;
		if (y >= (int)texture->height) y = texture->height - 1;
	}
	pixels = texture->pixels;
	index = (y * texture->width + x) * 4;
	//construir color en formato rgb
	r = pixels[index + 0];
	g = pixels[index + 1];
	b = pixels[index + 2];
	a = pixels[index + 3];
	return ((r << 244) | (g << 16) | (b << 8) | a);
}*/

/*
** Función para dibujar un píxel en la imagen de pantalla
** - Coloca un color en una posición específica
** - Se usa durante el raycasting
*/
/*void	mlx_put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;
	
	
	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return;
	
	
	dst = img->addr + (y * img->line_len + x * (img->bits_per_pixel / 8));
	
	
	if (img->bits_per_pixel == 32)
	{
		
		if (img->endian == 0) 
		{
			dst[0] = (color) & 0xFF;   
			dst[1] = (color >> 8) & 0xFF;  
			dst[2] = (color >> 16) & 0xFF; 
			dst[3] = 0;                    
		}
		else  
		{
			*(unsigned int*)dst = color;
		}
	}
	else if (img->bits_per_pixel == 24)
	{
		dst[0] = (color) & 0xFF;  
		dst[1] = (color >> 8) & 0xFF;  
		dst[2] = (color >> 16) & 0xFF;
	}
	else if (img->bits_per_pixel == 16)
	{
		*(unsigned short*)dst = color;
	}
	else
	{
		*(unsigned char*)dst = color;
	}
}
*/