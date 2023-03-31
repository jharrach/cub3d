#include "../include/cub3d.h"

void	load_map(t_data *d, t_input *in)
{
	(void)d;
	while (*in->i != *in->input && get_input_type(*in->i) == NEWLINE)
		in->i++;
	printf("DEBUG: Found map in line %li\n", in->i - in->input + 1);
}
