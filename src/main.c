
#include "../inc/cub3d.h"

/* ========================================================================== */
/* ⚠️  INICIO: CÓDIGO COMPATIBLE MAC/LINUX - OK PARA DESARROLLO              */
/* ========================================================================== */

int	parse_validate_input(t_game *game)
{

	if (!parse_config(game->mapinfo))
		return (0);
	if(!validate_config(game->mapinfo))
		return (0);
	if (!skip_empty_lines(game->mapinfo))
		return (0);
	if (!parse_map(game->mapinfo))
		return (0);
	if (!validate_map(game, game->mapinfo, game->map))
		return (0);
	return (1);
}

/*
** Valida argumentos de entrada
** Debe haber exactamente 2 argumentos y el archivo debe terminar en .cub
**
** DUDA: + el archivo debe poder abrirse (o dejar esto en otro?)
*/

static int	validate_args(int argc, char **argv)
{
	char	*ext;

	if (argc != 2)
		return (error_msg(ERR_USAGE, NULL, 0));
	ext = ft_strrchr(argv[1], '.');
	if (!ext || ft_strcmp(ext, ".cub") != 0 || ext[4] != '\0')
		return (error_msg(ERR_FILE_EXT, NULL, 0));
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
		return (EXIT_FAILURE); //xk no es un error de malloc, open... es un error de uso
	if (!init_game(&game))
		error_exit(ERR_INIT);
	if (!load_mapinfo(argv[1], game.mapinfo))
		error_cleanup_exit(ERR_LOAD_DATA, &game);
	if (!parse_validate_input(&game))
		error_cleanup_exit(ERR_PARSE, &game);

	/*if (!parse_file(argv[1], &game))
		error_cleanup_exit("Failed to parse file", &game);*/
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