
#include "../inc/cub3d.h"

/*
** Función para inicializar todas las estructuras del juego
** - Reserva memoria para cada estructura
** - Pone valores por defecto
*/
static int	init_game(t_game *game)
{
	/* Inicializar todo a NULL por seguridad */
	game->mlx = NULL;
	game->window = NULL;
	game->screen = NULL;
	
	/* Reservar memoria para cada estructura */
	game->map = malloc(sizeof(t_map));
	if (!game->map)
		return (0);
		
	game->player = malloc(sizeof(t_player));
	if (!game->player)
	{
		free(game->map);
		return (0);
	}
	
	game->ray = malloc(sizeof(t_ray));
	if (!game->ray)
	{
		free(game->map);
		free(game->player);
		return (0);
	}
	
	/* Inicializar texturas a NULL */
	game->textures[0] = NULL; /* Norte */
	game->textures[1] = NULL; /* Sur */
	game->textures[2] = NULL; /* Este */
	game->textures[3] = NULL; /* Oeste */
	
	/* Inicializar valores por defecto del mapa */
	init_map(game->map);
	
	printf("✅ Estructuras del juego inicializadas\n");
	return (1);
}

/*
** Función para validar los argumentos de entrada
** - Debe haber exactamente 2 argumentos (programa + archivo.cub)
** - El archivo debe terminar en .cub
*/
static int	validate_args(int argc, char **argv)
{
	char	*extension;
	
	if (argc != 2)
	{
		printf("❌ Error: Uso correcto: ./cub3d <mapa.cub>\n");
		return (0);
	}
	
	/* Buscar la extensión del archivo */
	extension = ft_strrchr(argv[1], '.');
	if (!extension || ft_strcmp(extension, ".cub") != 0)
	{
		printf("❌ Error: El archivo debe tener extensión .cub\n");
		return (0);
	}
	
	printf("✅ Argumentos válidos: %s\n", argv[1]);
	return (1);
}

/*
** Función principal del programa
** 1. Valida argumentos
** 2. Inicializa estructuras
** 3. Parsea el archivo .cub
** 4. Inicializa gráficos
** 5. Inicia el juego
*/
int	main(int argc, char **argv)
{
	t_game	game;
	
	printf("🎮 Iniciando cub3D...\n");
	
	/* Paso 1: Validar argumentos */
	if (!validate_args(argc, argv))
		return (1);
	
	/* Paso 2: Inicializar estructuras del juego */
	if (!init_game(&game))
	{
		printf("❌ Error: No se pudo inicializar el juego\n");
		return (1);
	}
	
	/* Paso 3: Parsear el archivo .cub */
	if (!parse_file(argv[1], &game))
	{
		printf("❌ Error: No se pudo parsear el archivo\n");
		cleanup_game(&game);
		return (1);
	}
	
	/* Paso 4: Inicializar gráficos (MLX) */
	if (!init_graphics(&game))
	{
		printf("❌ Error: No se pudo inicializar MLX\n");
		cleanup_game(&game);
		return (1);
	}
	
	printf("🚀 ¡Juego iniciado correctamente!\n");
	printf("Controles:\n");
	printf("  WASD - Moverse\n");
	printf("  ←→ - Girar cámara\n");
	printf("  ESC - Salir\n");
	
	/* Renderizar el primer frame */
	render_frame(&game);
	
	/* Configurar el loop_hook para que mlx_loop no crashee */
	mlx_loop_hook(game.mlx, render_frame, &game);
	
	/* Paso 5: Iniciar el loop principal del juego */
	mlx_loop(game.mlx);
	
	/* Limpiar memoria al salir */
	cleanup_game(&game);
	return (0);
}
