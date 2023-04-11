#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <errno.h>

#define L 0b0001
#define R 0b0010
#define T 0b0100
#define B 0b1000

#define PAUSE 100000

typedef struct s_vec2i
{
	int	x;
	int	y;
}	t_vec2i;

typedef struct s_stack
{
	t_vec2i	*arr;
	size_t	size;
}	t_stack;

typedef struct s_locations
{
	bool	term;
	int		*door;
	int32_t	num_doors;
	int		*entity;
	int32_t	num_entities;
	int		player;
	int		dir;
}	t_locations;

t_vec2i get_term_size( void ) {
  struct winsize ws;
  if ( ioctl( STDIN_FILENO , TIOCGWINSZ, &ws ) != 0 &&
       ioctl( STDOUT_FILENO, TIOCGWINSZ, &ws ) != 0 &&
       ioctl( STDERR_FILENO, TIOCGWINSZ, &ws ) != 0 ) {
    fprintf( stderr,
      "ioctl() failed (%d): %s\n", errno, strerror( errno )
    );
    return ((t_vec2i){0, 0});
  }
  return ((t_vec2i){ws.ws_row, ws.ws_col});
}

void	print(int **arr, t_vec2i size)
{
	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			printf("%c", arr[i][j]);
		}
		printf("\n");
	}
}

void	print_animation(int **arr, t_vec2i size)
{
	t_vec2i const	termsize = get_term_size();

	if (termsize.x == 0 && termsize.y == 0)
		return ;
	printf("\033[2J");
	for (int i = 0; i < size.x && i < termsize.x - 1; i++)
	{
		for (int j = 0; j < size.y && j < termsize.y - 1; j++)
		{
			if (arr[i][j] != '1')
				printf("\033[31m%c\033[0m", arr[i][j]);
			else
				printf("%c", arr[i][j]);
		}
		printf("\n");
	}
	usleep(PAUSE);
}

int isinrange(int num, int range)
{
	if (num < 0 || num >= range)
		return (false);
	else
		return (true);
}

int	get_neighbor(t_vec2i pos, t_vec2i size, int **arr)
{
	int	result = 0;

	if (isinrange(pos.x - 2, size.x) && arr[pos.x - 2][pos.y] == '1')
		result |= L;
	if (isinrange(pos.x + 2, size.x) && arr[pos.x + 2][pos.y] == '1')
		result |= R;
	if (isinrange(pos.y - 2, size.y) && arr[pos.x][pos.y - 2] == '1')
		result |= T;
	if (isinrange(pos.y + 2, size.y) && arr[pos.x][pos.y + 2] == '1')
		result |= B;
	return (result);
}

int num_neightbors(int result)
{
	int num;

	num = 0;
	if (result & R)
		num++;
	if (result & L)
		num++;
	if (result & B)
		num++;
	if (result & T)
		num++;
	return (num);
}

t_vec2i	get_next(int neighbors, int rand)
{
	int x;
	int	bit;

	x = 0;
	bit = 1;
	while (true)
	{
		if (neighbors & bit)
		{
			if (x == rand)
			{
				if (bit == L)
					return ((t_vec2i){.x = -1, .y = 0});
				if (bit == R)
					return ((t_vec2i){.x = 1, .y = 0});
				if (bit == T)
					return ((t_vec2i){.x = 0, .y = -1});
				if (bit == B)
					return ((t_vec2i){.x = 0, .y = 1});
			}
			x++;
		}
		bit <<= 1;
	}
}

t_stack	*push(t_stack *stack, t_vec2i pos)
{
	t_vec2i	*tmp;

	tmp = stack->arr;
	stack->arr = malloc(sizeof(*(stack->arr)) * (stack->size + 1));
	if (!stack->arr)
	{
		if (tmp)
			free(tmp);
		return (NULL);
	}
	if (tmp)
	{
		memcpy(stack->arr, tmp, sizeof(*(stack->arr)) * stack->size);
		free(tmp);
	}
	stack->arr[stack->size] = pos;
	stack->size++;
	return (stack);
}

t_vec2i	pop(t_stack *stack)
{
	stack->size--;
	return (stack->arr[stack->size]);
}

int replace_wall(t_locations *loc)
{
	static long	count = 0;

	count++;
	if (count == loc->player + 1)
		return (loc->dir);
	for (int i = 0; i < loc->num_doors; i++)
	{
		if (count == loc->door[i] + 1)
			return ('2');
	}
	for (int i = 0; i < loc->num_entities; i++)
	{
		if (count == loc->entity[i] + 1)
			return ('3');
	}
	return ('0');
}

t_vec2i	vec2i_add(t_vec2i v1, t_vec2i v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	return (v1);
}

t_vec2i	vec2i_sub(t_vec2i v1, t_vec2i v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	return (v1);
}

void	create_maze(t_vec2i pos, int **arr, t_vec2i size, t_locations *loc)
{
	t_stack	stack = {0};
	bool creating = false;

	arr[pos.x][pos.y] = '0';
	t_vec2i next = {0, 0};
	while (true)
	{
		int neighbors = get_neighbor(pos, size, arr);
		int num = num_neightbors(neighbors);

		if (num == 0)
		{
			if (stack.size == 0)
			{
				free(stack.arr);
				break ;
			}
			if (creating && (next.x || next.y) && rand() % 3 == 0)
			{
				pos = vec2i_add(pos, next);
				pos = vec2i_add(pos, next);
				if (isinrange(pos.x, size.x) && isinrange(pos.y, size.y))
				{
					if (loc->term)
						print_animation(arr, size);
					pos = vec2i_sub(pos, next);
					arr[pos.x][pos.y] = '0';
				}
			}
			creating = false;
			pos = pop(&stack);
			continue ;
		}
		creating = true;
		next = get_next(neighbors, rand() % num);
		push(&stack, pos);
		if (loc->term)
			print_animation(arr, size);
		pos = vec2i_add(pos, next);
		arr[pos.x][pos.y] = replace_wall(loc);
		if (loc->term)
			print_animation(arr, size);
		pos = vec2i_add(pos, next);
		arr[pos.x][pos.y] = replace_wall(loc);
	}
}

t_vec2i	check_size(t_vec2i size)
{
	if (size.x < 3)
		size.x = 3;
	if (size.y < 3)
		size.y = 3;
	if (size.x % 2 == 0)
		size.x += 1;
	if (size.y % 2 == 0)
		size.y += 1;
	return (size);
}

int	main(int argc, char **argv)
{
	t_locations	loc;

	if (argc != 3)
		return (1);
	srand(time(NULL));
	loc.term = isatty(1);
	t_vec2i	size = {.x = atoi(argv[1]), .y = atoi(argv[2])};
	
	size = check_size(size);
	int **arr = malloc(sizeof(*arr) * size.x);
	if (!arr)
	{
		fprintf(stderr, "error: ENOMEM\n");
		return (1);
	}
	for (int i = 0; i < size.x; i++)
	{
		arr[i] = malloc(sizeof(*(arr[i])) * size.y);
		if (!arr[i])
		{
			while (i--)
				free(arr[i]);
			free(arr);
			fprintf(stderr, "error: ENOMEM\n");
			return (1);
		}
		for (int j = 0; j < size.y; j++)
			arr[i][j] = '1';
	}
	t_vec2i	pos = {.x = 1, .y = 1};

	loc.num_doors = 10;
	loc.door = malloc(sizeof(*(loc.door)) * loc.num_doors);
	if (!loc.door)
	{
		fprintf(stderr, "error: ENOMEM\n");
		return (1);
	}
	loc.num_entities = 20;
	loc.entity = malloc(sizeof(*(loc.entity)) * loc.num_entities);
	if (!loc.entity)
	{
		fprintf(stderr, "error: ENOMEM\n");
		return (1);
	}
	size_t num_zeros = ((size.x - 1) * (size.y - 1)) / 2 - 1;
	for (int i = 0; i < loc.num_doors; i++)
		loc.door[i] = (rand() % (num_zeros / 2)) * 2;
	for (int i = 0; i < loc.num_entities; i++)
		loc.entity[i] = (rand() % (num_zeros / 2)) * 2 + 1;
	loc.player = rand() % num_zeros;
	loc.dir = 'N';
	create_maze(pos, arr, size, &loc);
	if (loc.term)
		print_animation(arr, size);
	else
		print(arr, size);
}
