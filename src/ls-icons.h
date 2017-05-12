#ifndef LS_ICONS_H
# define LS_ICONS_H

# include <stdio.h>
# include <stdlib.h>

int print_icon(const char *filename, const size_t len, FILE *stream,
	       int is_directory, int is_link, int is_quoted,
	       int is_colored);
int print_arrow_right(FILE *stream);

#endif /* LS_ICONS_H */
