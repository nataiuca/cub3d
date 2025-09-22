/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 00:03:21 by natferna          #+#    #+#             */
/*   Updated: 2025/09/23 00:03:23 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
								  &img->line_length, &img->endian);
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
		   width, height, img->bits_per_pixel, img->line_length, img->endian);
	
	return (img);
}

/*
** Función para cargar una textura desde un archivo
** - Carga una imagen .xpm usando MLX
** - Obtiene sus datos para poder leerla píxel por píxel
*/
static t_img	*load_texture(void *mlx, char *path)
{
	t_img	*texture;
	
	printf("🎨 Cargando textura: %s\n", path);
	
	texture = malloc(sizeof(t_img));
	if (!texture)
		return (NULL);
		
	/* Cargar imagen desde archivo */
	texture->img = mlx_xpm_file_to_image(mlx, path, &texture->width, &texture->height);
	if (!texture->img)
	{
		printf("❌ Error cargando textura: %s\n", path);
		free(texture);
		return (NULL);
	}
	
	/* Obtener datos de la imagen */
	texture->addr = mlx_get_data_addr(texture->img, &texture->bits_per_pixel,
									 &texture->line_length, &texture->endian);
	if (!texture->addr)
	{
		mlx_destroy_image(mlx, texture->img);
		free(texture);
		return (NULL);
	}
	
	printf("✅ Textura cargada: %dx%d píxeles\n", texture->width, texture->height);
	return (texture);
}

/*
** Función para cargar todas las texturas del mapa
** - Carga las 4 texturas de las paredes (N, S, E, W)
** - Las guarda en el array de texturas del juego
*/
int	load_textures(t_game *game)
{
	printf("🎨 Cargando texturas...\n");
	
	/* Cargar textura Norte (índice 0) */
	game->textures[0] = load_texture(game->mlx, game->map->north_texture);
	if (!game->textures[0])
		return (0);
		
	/* Cargar textura Sur (índice 1) */
	game->textures[1] = load_texture(game->mlx, game->map->south_texture);
	if (!game->textures[1])
		return (0);
		
	/* Cargar textura Este (índice 2) */
	game->textures[2] = load_texture(game->mlx, game->map->east_texture);
	if (!game->textures[2])
		return (0);
		
	/* Cargar textura Oeste (índice 3) */
	game->textures[3] = load_texture(game->mlx, game->map->west_texture);
	if (!game->textures[3])
		return (0);
	
	printf("✅ Todas las texturas cargadas correctamente\n");
	return (1);
}

/*
** Función para verificar que un archivo de textura existe
** - Intenta abrir el archivo
** - Útil para validar antes de cargar con MLX
*/
static int	check_texture_file(char *path)
{
	int	fd;
	
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		printf("❌ No se puede abrir archivo de textura: %s\n", path);
		return (0);
	}
	close(fd);
	return (1);
}

/*
** Función para validar todos los archivos de textura antes de cargarlos
*/
static int	validate_texture_files(t_game *game)
{
	printf("🔍 Verificando archivos de textura...\n");
	
	if (!check_texture_file(game->map->north_texture))
		return (0);
	if (!check_texture_file(game->map->south_texture))
		return (0);
	if (!check_texture_file(game->map->east_texture))
		return (0);
	if (!check_texture_file(game->map->west_texture))
		return (0);
		
	printf("✅ Todos los archivos de textura son accesibles\n");
	return (1);
}

/*
** Función principal para inicializar los gráficos
** - Inicializa MLX
** - Crea la ventana
** - Carga texturas
** - Crea imagen para dibujar
** - Configura eventos
*/
int	init_graphics(t_game *game)
{
	printf("🖥️  Inicializando sistema gráfico...\n");
	
	/* Validar archivos de textura primero */
	if (!validate_texture_files(game))
		return (0);
	
	/* Inicializar MLX */
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		printf("❌ Error inicializando MLX\n");
		return (0);
	}
	printf("✅ MLX inicializado\n");
	
	/* Crear ventana */
	game->window = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "cub3D");
	if (!game->window)
	{
		printf("❌ Error creando ventana\n");
		return (0);
	}
	printf("✅ Ventana creada: %dx%d píxeles\n", WINDOW_WIDTH, WINDOW_HEIGHT);
	
	/* Crear imagen para dibujar en pantalla */
	game->screen = create_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->screen)
	{
		printf("❌ Error creando imagen de pantalla\n");
		return (0);
	}
	printf("✅ Buffer de pantalla creado\n");
	
	/* Cargar todas las texturas */
	if (!load_textures(game))
	{
		printf("❌ Error cargando texturas\n");
		return (0);
	}
	
	/* Configurar eventos de teclado y ventana */
	mlx_hook(game->window, 2, 1L<<0, handle_keypress, game);    /* Tecla presionada */
	mlx_hook(game->window, 17, 1L<<17, handle_close, game);     /* Cerrar ventana */
	
	/* Configurar loop de render */
	mlx_loop_hook(game->mlx, render_frame, game);
	
	printf("✅ Sistema gráfico inicializado correctamente\n");
	return (1);
}

/*
** Función para obtener el color de un píxel de una textura
** - Convierte coordenadas de textura a color
** - Útil para el raycasting
*/
int	get_texture_color(t_img *texture, int x, int y)
{
	char	*dst;
	
	/* Verificar que la textura existe */
	if (!texture || !texture->addr)
		return (0xFF00FF); /* Rosa magenta para debug */
		
	/* Verificar límites */
	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (0);
		
	/* Calcular posición en memoria */
	dst = texture->addr + (y * texture->line_length + x * (texture->bits_per_pixel / 8));
	
	/* Devolver color como entero */
	return (*(unsigned int*)dst);
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
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	
	/* Escribir color según el formato de bits por píxel */
	if (img->bits_per_pixel == 32)
	{
		*(unsigned int*)dst = color;
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
