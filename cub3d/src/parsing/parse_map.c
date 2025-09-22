/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 23:33:55 by natferna          #+#    #+#             */
/*   Updated: 2025/09/22 23:34:00 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

/*
** Función para verificar si un carácter es válido en el mapa
** - 0: espacio vacío (puedes caminar)
** - 1: pared (no puedes caminar)
** - N/S/E/W: posición inicial del jugador y dirección
** - ' ' (espacio): válido como parte del mapa
*/
int	is_valid_char(char c)
{
	return (c == '0' || c == '1' || c == 'N' || c == 'S' || 
			c == 'E' || c == 'W' || c == ' ');
}

/*
** Función para contar las líneas del mapa en el archivo
** - Necesitamos saber cuántas líneas tiene para reservar memoria
*/
static int	count_map_lines(char *filename)
{
	int		fd;
	char	*line;
	int		count;
	int		map_started;
	
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
		
	count = 0;
	map_started = 0;
	
	while ((line = get_next_line(fd)) != NULL)
	{
		/* Saltar líneas de configuración hasta encontrar el mapa */
		if (!map_started)
		{
			if (line[0] == '1' || line[0] == ' ')
				map_started = 1;
			else
			{
				free(line);
				continue;
			}
		}
		
		/* Si ya empezó el mapa, contar líneas no vacías */
		if (map_started && line[0] != '\0' && line[0] != '\n')
			count++;
			
		free(line);
	}
	
	close(fd);
	printf("📊 Líneas de mapa contadas: %d\n", count);
	return (count);
}

/*
** Función para encontrar el ancho máximo del mapa
** - El mapa puede tener líneas de diferentes longitudes
** - Necesitamos el ancho máximo para hacer el grid rectangular
*/
static int	find_max_width(char **grid, int height)
{
	int	max_width;
	int	current_width;
	int	i;
	
	max_width = 0;
	i = 0;
	while (i < height)
	{
		current_width = ft_strlen(grid[i]);
		if (current_width > max_width)
			max_width = current_width;
		i++;
	}
	return (max_width);
}

/*
** Función para normalizar el mapa
** - Hace que todas las líneas tengan la misma longitud
** - Rellena con espacios las líneas más cortas
*/
static int	normalize_map(t_game *game)
{
	int		i;
	int		j;
	int		old_len;
	char	*new_line;
	
	i = 0;
	while (i < game->map->height)
	{
		old_len = ft_strlen(game->map->grid[i]);
		if (old_len < game->map->width)
		{
			/* Crear nueva línea con espacios adicionales */
			new_line = malloc(game->map->width + 1);
			if (!new_line)
				return (0);
				
			/* Copiar contenido original */
			ft_strcpy(new_line, game->map->grid[i]);
			
			/* Rellenar con espacios */
			j = old_len;
			while (j < game->map->width)
			{
				new_line[j] = ' ';
				j++;
			}
			new_line[game->map->width] = '\0';
			
			/* Reemplazar línea original */
			free(game->map->grid[i]);
			game->map->grid[i] = new_line;
		}
		i++;
	}
	return (1);
}

/*
** Función para leer el mapa desde el archivo
** - Lee todas las líneas del mapa
** - Elimina saltos de línea
** - Valida caracteres
*/
static int	read_map_data(char *filename, t_game *game)
{
	int		fd;
	char	*line;
	int		i;
	int		map_started;
	int		len;
	
	/* Contar líneas primero */
	game->map->height = count_map_lines(filename);
	if (game->map->height == 0)
	{
		printf("❌ No se encontró mapa válido\n");
		return (0);
	}
	
	/* Reservar memoria para el grid */
	game->map->grid = malloc(sizeof(char *) * (game->map->height + 1));
	if (!game->map->grid)
		return (0);
	
	/* Inicializar todas las posiciones a NULL */
	for (int j = 0; j <= game->map->height; j++)
		game->map->grid[j] = NULL;
	
	/* Abrir archivo nuevamente */
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (0);
		
	i = 0;
	map_started = 0;
	
	/* Leer líneas del mapa */
	while ((line = get_next_line(fd)) != NULL && i < game->map->height)
	{
		/* Saltar configuración hasta encontrar mapa */
		if (!map_started)
		{
			if (line[0] == '1' || line[0] == ' ')
				map_started = 1;
			else
			{
				free(line);
				continue;
			}
		}
		
		/* Saltar líneas vacías */
		if (line[0] == '\0' || line[0] == '\n')
		{
			free(line);
			continue;
		}
		
		/* Eliminar salto de línea */
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
			
		/* Validar caracteres de la línea */
		int j = 0;
		while (line[j])
		{
			if (!is_valid_char(line[j]))
			{
				printf("❌ Carácter inválido en mapa: '%c' en línea %d\n", line[j], i + 1);
				free(line);
				close(fd);
				return (0);
			}
			j++;
		}
		
		/* Guardar línea */
		game->map->grid[i] = line;
		printf("📝 Línea %d: %s\n", i, line);
		i++;
	}
	
	/* Cerrar archivo y limpiar líneas restantes */
	while ((line = get_next_line(fd)) != NULL)
		free(line);
	close(fd);
	
	/* Actualizar altura real */
	game->map->height = i;
	game->map->grid[i] = NULL;
	
	/* Calcular ancho máximo */
	game->map->width = find_max_width(game->map->grid, game->map->height);
	
	/* Normalizar mapa */
	if (!normalize_map(game))
		return (0);
		
	printf("🗺️  Mapa leído: %d x %d\n", game->map->width, game->map->height);
	return (1);
}

/*
** Función para encontrar al jugador en el mapa
** - Busca N, S, E, o W
** - Solo puede haber un jugador
** - Configura posición y dirección inicial
*/
int	find_player(t_game *game)
{
	int		i;
	int		j;
	int		player_count;
	char	player_char;
	
	player_count = 0;
	player_char = '\0';
	
	/* Buscar jugador en todo el mapa */
	i = 0;
	while (i < game->map->height)
	{
		j = 0;
		while (j < game->map->width)
		{
			if (game->map->grid[i][j] == 'N' || game->map->grid[i][j] == 'S' ||
				game->map->grid[i][j] == 'E' || game->map->grid[i][j] == 'W')
			{
				player_count++;
				player_char = game->map->grid[i][j];
				
				/* Guardar posición del jugador */
				game->player->pos_x = j + 0.5; /* Centro de la casilla */
				game->player->pos_y = i + 0.5;
				
				printf("👤 Jugador encontrado en (%d, %d) mirando %c\n", 
					   j, i, player_char);
			}
			j++;
		}
		i++;
	}
	
	/* Validar que hay exactamente un jugador */
	if (player_count == 0)
	{
		printf("❌ No se encontró jugador en el mapa\n");
		return (0);
	}
	if (player_count > 1)
	{
		printf("❌ Hay más de un jugador en el mapa (%d)\n", player_count);
		return (0);
	}
	
	/* Configurar dirección inicial según orientación */
	if (player_char == 'N')
	{
		game->player->dir_x = 0;
		game->player->dir_y = -1;  /* Norte es hacia arriba */
		game->player->plane_x = 0.66;  /* Plano de cámara perpendicular */
		game->player->plane_y = 0;
	}
	else if (player_char == 'S')
	{
		game->player->dir_x = 0;
		game->player->dir_y = 1;   /* Sur es hacia abajo */
		game->player->plane_x = -0.66;
		game->player->plane_y = 0;
	}
	else if (player_char == 'E')
	{
		game->player->dir_x = 1;   /* Este es hacia la derecha */
		game->player->dir_y = 0;
		game->player->plane_x = 0;
		game->player->plane_y = 0.66;
	}
	else if (player_char == 'W')
	{
		game->player->dir_x = -1;  /* Oeste es hacia la izquierda */
		game->player->dir_y = 0;
		game->player->plane_x = 0;
		game->player->plane_y = -0.66;
	}
	
	/* Configurar velocidades */
	game->player->move_speed = 0.05;
	game->player->rot_speed = 0.03;
	
	return (1);
}

/*
** Función para verificar si el mapa está cerrado
** - Las paredes exteriores deben ser todas '1' o ' '
** - No puede haber espacios '0' en los bordes
*/
static int	check_map_closed(t_game *game)
{
	int	i;
	int	j;
	
	/* Verificar primera y última fila */
	i = 0;
	while (i < game->map->height)
	{
		/* Primera y última columna */
		if ((game->map->grid[i][0] != '1' && game->map->grid[i][0] != ' ') ||
			(game->map->grid[i][game->map->width - 1] != '1' && 
			 game->map->grid[i][game->map->width - 1] != ' '))
		{
			printf("❌ Mapa no está cerrado (bordes verticales)\n");
			return (0);
		}
		i++;
	}
	
	/* Verificar primera y última fila */
	j = 0;
	while (j < game->map->width)
	{
		if ((game->map->grid[0][j] != '1' && game->map->grid[0][j] != ' ') ||
			(game->map->grid[game->map->height - 1][j] != '1' && 
			 game->map->grid[game->map->height - 1][j] != ' '))
		{
			printf("❌ Mapa no está cerrado (bordes horizontales)\n");
			return (0);
		}
		j++;
	}
	
	printf("✅ Mapa está correctamente cerrado\n");
	return (1);
}

/*
** Función principal para validar el mapa
** - Lee el mapa desde el archivo
** - Encuentra al jugador
** - Verifica que está cerrado
*/
int	validate_map_from_file(char *filename, t_game *game)
{
	printf("🔍 Validando mapa desde: %s\n", filename);
	
	/* Leer datos del mapa */
	if (!read_map_data(filename, game))
	{
		printf("❌ Error leyendo datos del mapa\n");
		return (0);
	}
		
	/* Encontrar jugador */
	if (!find_player(game))
	{
		printf("❌ Error encontrando jugador\n");
		return (0);
	}
		
	/* Verificar que está cerrado */
	if (!check_map_closed(game))
	{
		printf("❌ Error: mapa no está cerrado\n");
		return (0);
	}
		
	printf("✅ Mapa válido y listo para usar\n");
	return (1);
}
