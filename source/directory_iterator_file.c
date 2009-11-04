/*
	DIRECTORY_ITERATOR_FILE.C
	-------------------------
*/
#include <new>
#include <string.h>
#include "pragma.h"
#include "directory_iterator_file.h"

/*
	ANT_DIRECTORY_ITERATOR_FILE::FIRST()
	------------------------------------
*/
ANT_directory_iterator_object *ANT_directory_iterator_file::first(ANT_directory_iterator_object *object, char *wildcard, long get_file)
{
document_end = document_start = file;

return next(object, get_file);
#pragma ANT_PRAGMA_UNUSED_PARAMETER
}

/*
	ANT_DIRECTORY_ITERATOR_FILE::NEXT()
	-----------------------------------
*/
ANT_directory_iterator_object *ANT_directory_iterator_file::next(ANT_directory_iterator_object *object, long get_file)
{
char *document_id_start, *document_id_end;

if ((document_start = strstr(document_end, "<DOC")) != NULL)
	if ((document_id_start = strstr(document_start, "<DOCNO>")) != NULL)
		if ((document_id_end = strstr(document_id_start += 7, "</DOCNO>")) != NULL)
			if ((document_end = strstr(document_id_end, "</DOC>")) != NULL)
				{
				strncpy(object->filename, document_id_start, document_id_end - document_id_start);
				object->filename[document_id_end - document_id_start] = '\0';
				if (get_file)
					read_entire_file(object);
				return object;
				}
return NULL;
}

/*
	ANT_DIRECTORY_ITERATOR_FILE::READ_ENTIRE_FILE()
	-----------------------------------------------
*/
ANT_directory_iterator_object *ANT_directory_iterator_file::read_entire_file(ANT_directory_iterator_object *object)
{
object->length = document_end - document_start;
object->file = new (std::nothrow) char [(size_t)(object->length + 1)];
strncpy(object->file, document_start, (size_t)object->length);
object->file[(size_t)object->length] = '\0';

return object;
}
