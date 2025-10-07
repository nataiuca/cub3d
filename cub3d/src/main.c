
#include "../inc/cub3d.h"

/* ========================================================================== */
/* ⚠️  INICIO: CÓDIGO COMPATIBLE MAC/LINUX - OK PARA DESARROLLO              */
/* ========================================================================== */

/*
** Inicializa estructuras del juego
** Reserva memoria y pone valores por defecto
*/
static int	init_structs(t_game *game)
{
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
	return (1);
}

/*
** Inicializa el juego completo
** Pone todo a NULL por seguridad antes de reservar memoria
*/
static int	init_game(t_game *game)
{
	game->mlx = NULL;
	game->window = NULL;
	game->screen = NULL;
	game->textures[0] = NULL;
	game->textures[1] = NULL;
	game->textures[2] = NULL;
	game->textures[3] = NULL;
	if (!init_structs(game))
		return (0);
	init_map(game->map);
	return (1);
}

/*
** Valida argumentos de entrada
** Debe haber exactamente 2 argumentos y el archivo debe terminar en .cub
*/
static int	validate_args(int argc, char **argv)
{
	char	*extension;

	if (argc != 2)
	{
		write(2, "Error\n", 6);
		write(2, "Usage: ./cub3D <map.cub>\n", 25);
		return (0);
	}
	extension = ft_strrchr(argv[1], '.');
	if (!extension || ft_strcmp(extension, ".cub") != 0)
	{
		write(2, "Error\n", 6);
		write(2, "File must have .cub extension\n", 31);
		return (0);
	}
	return (1);
}

/* ========================================================================== */
/* ⚠️  SECCIÓN MAC: SOLO PARA DESARROLLO - ELIMINAR PARA ENTREGA            */
/* ========================================================================== */

#ifdef __APPLE__
/*
** MENSAJE DE DEBUG PARA MAC
** Esta función solo existe en la versión de desarrollo para Mac
** Te avisa que estás usando la versión de Mac
*/
static void	print_mac_warning(void)
{
	write(1, "\n", 1);
	write(1, "🍎 RUNNING ON MAC - DEVELOPMENT VERSION\n", 41);
	write(1, "⚠️  Remember to remove Mac code before submission!\n", 54);
	write(1, "\n", 1);
}

/*
** MAIN PARA MAC - VERSIÓN DE DESARROLLO
** Incluye mensaje de advertencia y configuración específica de Mac
*/
int	main(int argc, char **argv)
{
	t_game	game;

	print_mac_warning();
	if (!validate_args(argc, argv))
		return (1);
	if (!init_game(&game))
		error_exit("Failed to initialize game");
	if (!parse_file(argv[1], &game))
		error_cleanup_exit("Failed to parse file", &game);
	if (!init_graphics(&game))
		error_cleanup_exit("Failed to initialize graphics", &game);
	write(1, "✅ Game initialized successfully!\n", 37);
	write(1, "Controls: WASD=move, Arrows=rotate, ESC=exit\n", 46);
	render_game(&game);
	mlx_hook(game.window, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.window, 17, 0, handle_close, &game);
	mlx_loop(game.mlx);
	cleanup_game(&game);
	return (0);
}

#else

/* ========================================================================== */
/* ✅ SECCIÓN LINUX: VERSIÓN FINAL PARA ENTREGA                              */
/* ========================================================================== */

/*
** MAIN PARA LINUX - VERSIÓN FINAL
** Esta es la versión que debes entregar en el campus
** Sin mensajes específicos de Mac
*/
int	main(int argc, char **argv)
{
	t_game	game;

	if (!validate_args(argc, argv))
		return (1);
	if (!init_game(&game))
		error_exit("Failed to initialize game");
	if (!parse_file(argv[1], &game))
		error_cleanup_exit("Failed to parse file", &game);
	if (!init_graphics(&game))
		error_cleanup_exit("Failed to initialize graphics", &game);
	render_game(&game);
	mlx_hook(game.window, 2, 1L << 0, handle_keypress, &game);
	mlx_hook(game.window, 17, 0, handle_close, &game);
	mlx_loop(game.mlx);
	cleanup_game(&game);
	return (0);
}

#endif

/* ========================================================================== */
/* 📝 INSTRUCCIONES PARA ENTREGA:                                            */
/*                                                                            */
/* OPCIÓN 1 (Recomendada): ELIMINAR TODO DESDE #ifdef __APPLE__ hasta #else */
/* y eliminar el #endif final. Dejar solo la versión de Linux.               */
/*                                                                            */
/* OPCIÓN 2: Reemplazar TODO el archivo por la versión siguiente:            */
/* ========================================================================== */