
#include "../../inc/cub3d.h"

/*
** Función para eliminar espacios al inicio y final de una cadena
** - Crea una nueva cadena sin espacios al principio y al final
** - Útil para limpiar las líneas leídas del archivo
*/
static char	*trim_spaces(char *str)
{
	char	*start;
	char	*end;
	char	*result;
	int		len;
	
	if (!str)
		return (NULL);
		
	/* Saltar espacios al inicio */
	start = str;
	while (*start == ' ' || *start == '\t')
		start++;
		
	/* Si solo hay espacios, devolver cadena vacía */
	if (*start == '\0')
		return (ft_strdup(""));
		
	/* Encontrar el final (sin espacios) */
	end = start + ft_strlen(start) - 1;
	while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;
		
	/* Crear nueva cadena */
	len = end - start + 1;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strncpy(result, start, len);
	result[len] = '\0';
	
	return (result);
}

/*
** Función para parsear un color en formato "R,G,B"
** - Convierte "220,100,0" a un valor entero de color
** - Valida que los valores estén en el rango [0,255]
*/
static int	parse_color(char *color_str)
{
	char	**rgb;
	int		r, g, b;
	int		color;
	
	if (!color_str)
		return (-1);
		
	/* Dividir por comas */
	rgb = ft_split(color_str, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
	{
		/* Error: debe haber exactamente 3 valores */
		if (rgb)
		{
			int i = 0;
			while (rgb[i])
				free(rgb[i++]);
			free(rgb);
		}
		return (-1);
	}
	
	/* Convertir a enteros y validar rango */
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	
	/* Liberar memoria */
	free(rgb[0]);
	free(rgb[1]);
	free(rgb[2]);
	free(rgb);
	
	/* Validar rango [0,255] */
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
		
	/* Combinar en un solo entero (formato RGB) */
	color = (r << 16) | (g << 8) | b;
	return (color);
}

/*
** Función para parsear una línea de configuración
** - Identifica el tipo de línea (NO, SO, EA, WE, F, C)
** - Extrae y guarda la información correspondiente
*/
static int	parse_config_line(char *line, t_game *game)
{
	char	*trimmed;
	char	**parts;
	int		success;
	
	success = 0;
	trimmed = trim_spaces(line);
	if (!trimmed || trimmed[0] == '\0' || trimmed[0] == '\n')
	{
		free(trimmed);
		return (1); /* Línea vacía, ignorar pero no es error */
	}
	
	/* Dividir la línea en partes */
	parts = ft_split(trimmed, ' ');
	if (!parts || !parts[0])
	{
		free(trimmed);
		return (0);
	}
	
	/* Parsear según el identificador */
	if (ft_strcmp(parts[0], "NO") == 0 && parts[1] && !parts[2])
	{
		game->map->north_texture = ft_strdup(parts[1]);
		success = (game->map->north_texture != NULL);
		printf("📍 Textura Norte: %s\n", parts[1]);
	}
	else if (ft_strcmp(parts[0], "SO") == 0 && parts[1] && !parts[2])
	{
		game->map->south_texture = ft_strdup(parts[1]);
		success = (game->map->south_texture != NULL);
		printf("📍 Textura Sur: %s\n", parts[1]);
	}
	else if (ft_strcmp(parts[0], "WE") == 0 && parts[1] && !parts[2])
	{
		game->map->west_texture = ft_strdup(parts[1]);
		success = (game->map->west_texture != NULL);
		printf("📍 Textura Oeste: %s\n", parts[1]);
	}
	else if (ft_strcmp(parts[0], "EA") == 0 && parts[1] && !parts[2])
	{
		game->map->east_texture = ft_strdup(parts[1]);
		success = (game->map->east_texture != NULL);
		printf("📍 Textura Este: %s\n", parts[1]);
	}
	else if (ft_strcmp(parts[0], "F") == 0 && parts[1] && !parts[2])
	{
		game->map->floor_color = parse_color(parts[1]);
		success = (game->map->floor_color != -1);
		printf("🟤 Color suelo: %s (0x%06X)\n", parts[1], game->map->floor_color);
	}
	else if (ft_strcmp(parts[0], "C") == 0 && parts[1] && !parts[2])
	{
		game->map->ceiling_color = parse_color(parts[1]);
		success = (game->map->ceiling_color != -1);
		printf("🔵 Color techo: %s (0x%06X)\n", parts[1], game->map->ceiling_color);
	}
	
	/* Liberar memoria */
	int i = 0;
	while (parts[i])
		free(parts[i++]);
	free(parts);
	free(trimmed);
	
	return (success);
}

/*
** Función para verificar que todas las configuraciones están completas
** - Todas las texturas deben estar definidas
** - Los colores deben ser válidos
*/
static int	validate_config(t_game *game)
{
	if (!game->map->north_texture)
	{
		printf("❌ Falta textura Norte (NO)\n");
		return (0);
	}
	if (!game->map->south_texture)
	{
		printf("❌ Falta textura Sur (SO)\n");
		return (0);
	}
	if (!game->map->east_texture)
	{
		printf("❌ Falta textura Este (EA)\n");
		return (0);
	}
	if (!game->map->west_texture)
	{
		printf("❌ Falta textura Oeste (WE)\n");
		return (0);
	}
	if (game->map->floor_color == -1)
	{
		printf("❌ Color de suelo inválido (F)\n");
		return (0);
	}
	if (game->map->ceiling_color == -1)
	{
		printf("❌ Color de techo inválido (C)\n");
		return (0);
	}
	
	printf("✅ Todas las configuraciones son válidas\n");
	return (1);
}

/*
** Función principal para parsear el archivo .cub
** - Abre el archivo
** - Lee línea por línea
** - Parsea configuraciones y mapa
** - Valida todo
*/
int	parse_file(char *filename, t_game *game)
{
	int		fd;
	char	*line;
	int		config_complete;
	
	printf("📖 Parseando archivo: %s\n", filename);
	
	/* Abrir archivo */
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("❌ No se puede abrir el archivo: %s\n", filename);
		return (0);
	}
	
	config_complete = 0;
	
	/* Leer línea por línea */
	while ((line = get_next_line(fd)) != NULL)
	{
		/* Si aún no hemos terminado la configuración */
		if (!config_complete)
		{
			/* Si la línea parece ser parte del mapa (empieza con 1 o espacio) */
			if (line[0] == '1' || line[0] == ' ')
			{
				config_complete = 1;
				printf("🗺️  Configuración terminada, empezando mapa...\n");
				/* Por ahora, guardar esta línea para el mapa */
				free(line);
				break;
			}
			else
			{
				/* Parsear línea de configuración */
				int parse_result = parse_config_line(line, game);
				if (parse_result == 0)
				{
					printf("❌ Error parseando línea: %s", line);
					free(line);
					close(fd);
					return (0);
				}
				/* parse_result == 1 significa éxito (línea válida o vacía) */
			}
		}
		free(line);
	}
	
	close(fd);
	
	/* Validar que toda la configuración esté completa */
	if (!validate_config(game))
		return (0);
		
	/* Parsear y validar el mapa */
	if (!validate_map_from_file(filename, game))
	{
		printf("❌ Error validando mapa\n");
		return (0);
	}
	
	printf("✅ Archivo parseado correctamente\n");
	return (1);
}
