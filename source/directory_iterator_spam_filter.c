/*
	DIRECTORY_ITERATOR_SPAM_FILTER.C
	--------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "disk.h"
#include "directory_iterator_spam_filter.h"

char **ANT_directory_iterator_spam_filter::docids = NULL;
long long ANT_directory_iterator_spam_filter::number_docs = 0;

/*
	ANT_DIRECTORY_ITERATOR_SPAM_FILTER::ANT_DIRECTORY_ITERATOR_SPAM_FILTER()
	------------------------------------------------------------------------
	Each line of the file containing spam scores should have the score followed by docid and be sorted by docid
*/
ANT_directory_iterator_spam_filter::ANT_directory_iterator_spam_filter(ANT_directory_iterator *source, char *filename, long long threshold, long method, long get_file) : ANT_directory_iterator("", get_file)
{
this->source = source;
this->method = method;

/*
	We only want the spam scores to be loaded once
*/
mutex.enter();
	if (docids == NULL)
		{
		char *spam_file = ANT_disk::read_entire_file(filename);
		char *ptr = spam_file, *nl;
		long score;
		long docids_recorded = 0;

		while (*ptr)
			{
			score = ANT_atol(ptr);
			if ((method == INCLUDE && score >= threshold) || (method == EXCLUDE && score < threshold))
				number_docs++;
			ptr = strchr(ptr, '\n') + 1; // skip to next score
			}

		docids = new char *[number_docs];
		ptr = spam_file;

		while (*ptr)
			{
			score = ANT_atol(ptr); // get score
			ptr = strchr(ptr, ' ') + 1; // skip to document
			nl = strchr(ptr, '\n'); // find end of docid
			if ((method == INCLUDE && score >= threshold) || (method == EXCLUDE && score < threshold))
				docids[docids_recorded++] = strnnew(ptr, nl - ptr);
			ptr = nl + 1; // skip to next line
			}
		
		/*
			We've finished with the spam rankings, so free up that memory
		*/
		delete [] spam_file;
		}
mutex.leave();
}

/*
	ANT_DIRECTORY_ITERATOR_SPAM_FILTER::~ANT_DIRECTORY_ITERATOR_SPAM_FILTER()
	-------------------------------------------------------------------------
*/
ANT_directory_iterator_spam_filter::~ANT_directory_iterator_spam_filter()
{
delete source;
delete [] docids;
}

/*
	ANT_DIRECTORY_ITERATOR_SPAM_FILTER::SHOULD_INDEX()
	--------------------------------------------------
*/
inline long ANT_directory_iterator_spam_filter::should_index(char *docid)
{
void *result = bsearch(strip_space_inplace(docid), docids, number_docs, sizeof(*docids), char_star_char_star_star_strcmp);

return (!result && method == EXCLUDE) || (!!result && method == INCLUDE);
}

/*
	ANT_DIRECTORY_ITERATOR_SPAM_FILTER::FIRST()
	-------------------------------------------
*/
ANT_directory_iterator_object *ANT_directory_iterator_spam_filter::first(ANT_directory_iterator_object *object)
{
ANT_directory_iterator_object *t = source->first(object);

if (should_index(t->filename))
	return t;
return next(object);
}

/*
	ANT_DIRECTORY_ITERATOR_SPAM_FILTER::NEXT()
	------------------------------------------
*/
ANT_directory_iterator_object *ANT_directory_iterator_spam_filter::next(ANT_directory_iterator_object *object)
{
ANT_directory_iterator_object *t = source->next(object);

while (t && !should_index(t->filename))
	t = source->next(object);

return t;
}