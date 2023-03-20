/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 16:45:38 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 13:50:58 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <limits.h>
# include <stdint.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 42
# endif

# define F_MINUS	0b00000001
# define F_ZERO		0b00000010
# define F_HASH		0b00000100
# define F_SPACE	0b00001000
# define F_PLUS		0b00010000
# define F_DOT		0b00100000

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_placeholder
{
	int	type;
	int	width;
	int	precision;
	int	flags;
	int	len;
}	t_placeholder;

int			ft_atoi(const char *str);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t count, size_t size);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isascii(int c);
int			ft_isdigit(int c);
int			ft_isprint(int c);
void		*ft_memchr(const void *s, int c, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dst, const void *src, size_t len);
void		*ft_memset(void *s, int c, size_t n);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s1);
size_t		ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_strrchr(const char *s, int c);
int			ft_tolower(int c);
int			ft_toupper(int c);

char		*ft_itoa(int n);
void		ft_putchar_fd(char c, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_putstr_fd(char *s, int fd);
char		**ft_split(char const *s, char c);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strmapi(const char *s, char (*f)(unsigned int, char));
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_substr(char const *s, unsigned int start, size_t len);

void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstclear(t_list **lst, void (*del)(void *));
void		ft_lstdelone(t_list *lst, void (*del)(void *));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list		*ft_lstnew(void *content);
int			ft_lstsize(t_list *lst);

int			ft_printf(const char *format, ...);
int			ft_printf_c(char c, t_placeholder ph);
int			ft_printf_s(const char *s1, t_placeholder ph);
int			ft_printf_p(uintptr_t p, t_placeholder ph);
int			ft_printf_x(unsigned int num, t_placeholder ph);
int			ft_printf_d(long num, t_placeholder ph);
int			ft_printf_u(unsigned int num, t_placeholder ph);
const char	*ft_getph(t_placeholder *ph, const char *format);
int			ft_dprintf(int fd, const char *format, ...);
int			ft_dprintf_c(int fd, char c, t_placeholder ph);
int			ft_dprintf_s(int fd, const char *s1, t_placeholder ph);
int			ft_dprintf_p(int fd, uintptr_t p, t_placeholder ph);
int			ft_dprintf_x(int fd, unsigned int num, t_placeholder ph);
int			ft_dprintf_d(int fd, long num, t_placeholder ph);
int			ft_dprintf_u(int fd, unsigned int num, t_placeholder ph);

char		*get_next_line(int fd);

int			ft_strcmp(const char *s1, const char *s2);
int			ft_isspace(int c);
int			ft_islong(const char *str);
int			ft_containsdigit(const char *str);
long		ft_atol(const char *str);

#endif
