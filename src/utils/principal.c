#include "cub3d.h"

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

int	parse_validate_input(t_game *game)
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

int	validate_args(int argc, char **argv)
{
	char	*ext;

	if (argc != 2)
		return (error_msg(ERR_USAGE, NULL, 0));
	ext = ft_strrchr(argv[1], '.');
	if (!ext || ft_strcmp(ext, ".cub") != 0 || ext[4] != '\0')
		return (error_msg(ERR_FILE_EXT, NULL, 0));
	return (1);
}
