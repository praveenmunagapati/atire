/*
	DIRECTORY_RECURSIVE_ITERATOR.H
	------------------------------
*/
#ifndef DIRECTORY_RECURSIVE_ITERATOR_H_
#define DIRECTORY_RECURSIVE_ITERATOR_H_

#include <stdlib.h>
#include "disk.h"
#include "directory_iterator.h"

#ifdef _MSC_VER
	#include <windows.h>
	#define PATH_MAX MAX_PATH
#else
	#include <limits.h>
#endif

class ANT_disk_directory;

/*
	class ANT_DIRECTORY_RECURSIVE_ITERATOR
	--------------------------------------
*/
class ANT_directory_recursive_iterator : public ANT_directory_iterator
{
private:
	ANT_disk_directory *handle_stack;
	ANT_disk_directory *file_list;
	char path_buffer[PATH_MAX];

private:
	long push_directory(void);
	long pop_directory(void);
	char *next_match_wildcard(void);
#ifdef _MSC_VER
	long ANT_directory_recursive_iterator::get_next_candidate(void);
#else
	char *first(char *root_directory, char *local_directory);
	static long ANT_directory_recursive_iterator::PathMatchSpec(const char *str, const char *pattern)
#endif

public:
	ANT_directory_recursive_iterator(char *the_wildcard);
	virtual ~ANT_directory_recursive_iterator();

	virtual ANT_directory_iterator_object *first(ANT_directory_iterator_object *object, long get_file = 0);
	virtual ANT_directory_iterator_object *next(ANT_directory_iterator_object *object, long get_file = 0);
} ;

#endif  /* DIRECTORY_RECURSIVE_ITERATOR_H_ */
