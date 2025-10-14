/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:45:08 by root              #+#    #+#             */
/*   Updated: 2025/10/13 13:20:40 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/* ========================================================================== */
/* ⚠️  INICIO: CÓDIGO COMPATIBLE MAC/LINUX - OK PARA DESARROLLO              */
/* ========================================================================== */

/**
 * @brief	Prints the parsed and validated input data.
 * 
 * @param game	Pointer to the main game structure.
 */

static void	print_parsed_input(t_game *game)
{
	int	i;

	printf("\n%s=== CUB3D GAME INFO ===%s\n", CYAN, RESET);
	printf("%s[Textures]%s\n", YELLOW, RESET);
	printf("NO texture: %s\n", game->map->no_texture);
	printf("SO texture: %s\n", game->map->so_texture);
	printf("EA texture: %s\n", game->map->ea_texture);
	printf("WE texture: %s\n", game->map->we_texture);
	printf("\n%s[Colors]%s\n", GREEN, RESET);
	printf("Floor color: %d\n", game->map->floor_color);
	printf("Ceiling color: %d\n", game->map->ceiling_color);
	printf("\n%s[Map]%s\n", MAGENTA, RESET);
	i = 0;
	while (game->map->grid[i])
	{
		printf("%s%s%s$\n", BLUE, game->map->grid[i], RESET);
		i ++;
	}
	printf("\n%s=== GAME STARTED ===%s\n\n", CYAN, RESET);
}

/**
 * 	@brief	Parses and validates the contents of the .cub map file.
 * 			Performs configuration parsing (texture paths, colors), skips
 * 			empty lines, and parses and validates the map grid.
 * 
 * @param game	Pointer to the main game structure
 * @return 1 if parsing and validation succeed, 0 otherwise
 */

static int	parse_validate_input(t_game *game)
{
	if (!parse_config(game, game->info))
		return (0);
	if (!validate_config(game))
		return (0);
	if (!skip_empty_lines(game->info))
		return (error_msg(ERR_NO_MAP, NULL, 0));
	if (!parse_map(game->info))
		return (0);
	if (!validate_map(game, game->info, game->map))
		return (0);
	print_parsed_input(game);
	return (1);
}

/**
 * @brief	Updates the game state and renders the next frame.
 * 			This function is registered as a loop hook and is called
 * 			automatically by MLX42 once per frame. It handles player movements,
 * 			raycasting, 3d rendering, and minimap drawing.
 * 
 * @param param	Pointer to the main game structure (passed as void*)
 */

void	update_game(void *param)
{
	t_game	*game;

	game = param;
	update_player(game, game->player);
	cast_all_rays(game);
	draw_3d_view(game);
	draw_minimap(game);
	draw_all_sprites(game);
}

/**
 * @brief	Validates program arguments and checks the map file extension.
 * 
 * @param argc	Number of command-line arguments
 * @param argv	Array of argument strings
 * @return	1 if arguments are valid, 0 otherwise
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

/**
 * @brief	Program entry point.
 * 			- Validates the command-line arguments
 * 			- Initializes the game structures
 * 			- Loads the .cub file
 * 			- Parses and validates map and configuration data
 * 			- Initializes grapgics and MLX42
 * 			- Registers input and loop hooks
 * 			- Starts the MLX42 main loop
 * 			- Cleanups for exiting
 * 			- On any error, the program prints an appropriate message and exits
 */

int	main(int argc, char **argv)
{
	t_game	game;

	if (!validate_args(argc, argv))
		return (EXIT_FAILURE);
	if (!init_game(&game))
		error_cleanup_exit(ERR_INIT, &game);
	if (!load_info(argv[1], game.info))
		error_cleanup_exit(ERR_LOAD_DATA, &game);
	if (!parse_validate_input(&game))
		error_cleanup_exit(ERR_PARSE, &game);
	if (!init_graphics(&game))
		error_cleanup_exit(ERR_INIT_GRAPHS, &game);
	mlx_key_hook(game.mlx, handle_keypress, &game);
	mlx_loop_hook(game.mlx, (void *)update_game, &game);
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