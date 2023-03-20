#include "../include/cub3d.h"

void ft_hook(void* param)
{
	mlx_t *const mlx = param;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
}

int	main(void)
{
	mlx_t *const mlx = mlx_init(500, 500, "cub3D", 0);
	ft_printf("libft\n");
	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
}
