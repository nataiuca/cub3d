
#include "../../inc/cub3d.h"

/*
** Función para mostrar error y salir del programa
** - Muestra "Error\n" seguido del mensaje personalizado
** - Sale del programa con código de error 1
*/
void	error_exit(char *message)
{
	printf("Error\n");
	if (message)
		printf("%s\n", message);
	exit(1);
}

/*
** Función para mostrar error, limpiar memoria y salir
** - Igual que error_exit pero limpia la memoria antes de salir
** - Útil cuando ya hemos reservado memoria
*/
void	error_cleanup_exit(char *message, t_game *game)
{
	printf("Error\n");
	if (message)
		printf("%s\n", message);
	cleanup_game(game);
	exit(1);
}
