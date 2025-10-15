/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natferna <natferna@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:45:08 by root              #+#    #+#             */
/*   Updated: 2025/10/15 13:24:25 by natferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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
 * 
 * @param argc	
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
