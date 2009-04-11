/*
	MEMORY.H
	--------
*/
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdio.h>
#include <stdlib.h>
#include <new>

class ANT_memory
{
private:
	char *chunk, *at, *chunk_end;
	long long used;
	long long allocated;
	long long block_size;
	long has_large_pages;
	size_t short_page_size, large_page_size;

protected:
	void *alloc(long long *size);
	void *get_chained_block(long long bytes);
#ifdef _MSC_VER
	long set_privilege(char *priv_name, long enable);
#endif

public:
	ANT_memory(long long block_size_for_allocation = 1024 * 1024 * 1024);
	~ANT_memory();

	void *malloc(long long bytes);
	long long bytes_allocated(void) { return allocated; }
	long long bytes_used(void) { return used; }
	void realign(void) { allocated += (allocated % sizeof(long long) == 0) ? 0 : sizeof(long long) - allocated % sizeof(long long); }		//
} ;

/*
	ANT_MEMORY::MALLOC()
	--------------------
*/
inline void *ANT_memory::malloc(long long bytes)
{
void *ans;

if (chunk == NULL || at + bytes > chunk_end)
	if (get_chained_block(bytes) == NULL)
		exit(printf("ANT:Out of memory:%lld bytes requested %lld bytes used %lld bytes allocated\n", (long long)bytes, used, allocated));

ans = at;
at += bytes;
used += bytes;

return ans;
}

#endif __MEMORY_H__

