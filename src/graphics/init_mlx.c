
#include "../../inc/cub3d.h"

/*
** Función para crear una nueva imagen
** - Crea una imagen en MLX con el tamaño especificado
** - Obtiene la dirección de memoria para dibujar píxeles
*/
static t_img	*create_image(void *mlx, int width, int height)
{
	t_img	*img;
	
	img = malloc(sizeof(t_img));
	if (!img)
		return (NULL);
		
	/* Crear imagen en MLX */
	img->img = mlx_new_image(mlx, width, height);
	if (!img->img)
	{
		free(img);
		return (NULL);
	}
	
	/* Obtener información de la imagen */
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
								  &img->line_len, &img->endian);
	if (!img->addr)
	{
		mlx_destroy_image(mlx, img->img);
		free(img);
		return (NULL);
	}
	
	img->width = width;
	img->height = height;
	
	/* Debug: mostrar formato de la imagen */
	printf("🖼️  Imagen creada: %dx%d, %d bits/pixel, line_length=%d, endian=%d\n", 
		   width, height, img->bits_per_pixel, img->line_len, img->endian);
	
	return (img);
}

/*
** Función para cargar una textura desde un archivo
** - Carga una imagen .xpm usando MLX
** - Obtiene sus datos para poder leerla píxel por píxel
*/
static mlx_image_t	*load_texture(void *mlx, char *path)
{
	mlx_image_t	*texture;
	int			fd;

	/* Verificación del archivo */
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		error_msg(NULL, NULL, 0);
		printf("❌ Error: No se puede abrir el archivo '%s'\n", path);
		printf("   errno: %d (%s)\n", errno, strerror(errno));
		return (NULL);
	}
	close(fd);
	
	texture = malloc(sizeof(t_img));
	if (!texture)
	{
		printf("❌ Error: malloc falló\n");
		return (NULL);
	}
		
	/* Cargar imagen desde archivo */
	texture->img = mlx_xpm_file_to_image(mlx, path, &texture->width, &texture->height);
	if (!texture->img)
	{
		printf("❌ Error: mlx_xpm_file_to_image falló para '%s'\n", path);
		free(texture);
		return (NULL);
	}
	
	/* Obtener datos de la imagen */
	texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
									 &texture->line_len, &texture->endian);
	if (!texture->addr)
	{
		printf("❌ Error: mlx_get_data_addr falló\n");
		mlx_destroy_image(mlx, texture->img);
		free(texture);
		return (NULL);
	}
	
	/* AÑADIDO: Información detallada de la textura */
	printf("✅ Textura cargada correctamente:\n");
	printf("   Dimensiones: %dx%d píxeles\n", texture->width, texture->height);
	printf("   Bits por píxel: %d\n", texture->bits_per_pixel);
	printf("   Line length: %d bytes\n", texture->line_len);
	printf("   Endian: %d\n", texture->endian);
	
	/* Verificar si las dimensiones son potencia de 2 */
	if ((texture->width & (texture->width - 1)) != 0)
		printf("   ℹ️  Ancho no es potencia de 2 (está OK con las correcciones)\n");
	if ((texture->height & (texture->height - 1)) != 0)
		printf("   ℹ️  Alto no es potencia de 2 (está OK con las correcciones)\n");
	
	return (texture);
}

/*
** Función para cargar todas las texturas del mapa
** - Carga las 4 texturas de las paredes (N, S, E, W)
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
** Función para verificar que un archivo de textura existe
** - Intenta abrir el archivo
** - Útil para validar antes de cargar con MLX

NO SE ESTÁ USANDO
*/
/*static int	check_texture_file(char *path)
{
	int	fd;
	
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return(error_msg(NULL, path, 0)); //errno ya sabe el mensaje
	close(fd);
	return (1);
}*/

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
		return(error_msg(ERR_LOAD_TEX, NULL, 0));
	mlx_key_hook(game->mlx, handle_keypress, game); //NO SE SI ES MEJOR QUE ESTAS FUNCIONES VAYAN AQUÍ O EN EL MAIN
	mlx_hook(game->mlx, handle_close, game); 
	
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
int	get_texture_color(mlx_image_t *texture, int x, int y)
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
		return (0xFF00FF); /* Rosa magenta para debug */
	}
		
	/* Verificar límites - MEJORADO */
	if (x < 0 || x >= (int)texture->width || y < 0 || y >= (int)texture->height)
	{
		if (debug_count++ < 5)
			printf("⚠️  Coordenadas fuera de límites: (%d,%d) max:(%d,%d)\n",
			       x, y, texture->width, texture->height);
		/* Hacer clamp en lugar de devolver 0 */
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
}

/*
** Función para dibujar un píxel en la imagen de pantalla
** - Coloca un color en una posición específica
** - Se usa durante el raycasting
*/
void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;
	
	/* Verificar límites */
	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return;
	
	/* Calcular posición en memoria */
	dst = img->addr + (y * img->line_len + x * (img->bits_per_pixel / 8));
	
	/* Escribir color según el formato de bits por píxel */
	if (img->bits_per_pixel == 32)
	{
		/* En Mac, a veces necesita formato BGRA */
		if (img->endian == 0)  /* Little endian */
		{
			dst[0] = (color) & 0xFF;        /* Blue */
			dst[1] = (color >> 8) & 0xFF;   /* Green */
			dst[2] = (color >> 16) & 0xFF;  /* Red */
			dst[3] = 0;                     /* Alpha */
		}
		else  /* Big endian */
		{
			*(unsigned int*)dst = color;
		}
	}
	else if (img->bits_per_pixel == 24)
	{
		dst[0] = (color) & 0xFF;        /* Blue */
		dst[1] = (color >> 8) & 0xFF;   /* Green */
		dst[2] = (color >> 16) & 0xFF;  /* Red */
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
