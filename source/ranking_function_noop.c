/*
	RANKING_FUNCTION_NOOP.C
	-----------------------------
*/
#include <math.h>
#include "pragma.h"
#include "ranking_function_noop.h"
#include "search_engine_btree_leaf.h"
#include "compress.h"
#include "search_engine_accumulator.h"

/*
	ANT_RANKING_FUNCTION_NOOP::RELEVANCE_RANK_TOP_K()
	-------------------------------------------------------
*/
void ANT_ranking_function_noop::relevance_rank_top_k(ANT_search_engine_result *accumulator, ANT_search_engine_btree_leaf *term_details, ANT_compressable_integer *impact_ordering, long long trim_point, double prescalar, double postscalar)
{
(void) accumulator;
(void) term_details;
(void) impact_ordering;
(void) trim_point;
(void) prescalar;
(void) postscalar;

long docid;
ANT_compressable_integer *current, *end;

current = impact_ordering;
end = impact_ordering + (term_details->document_frequency >= trim_point ? trim_point : term_details->document_frequency);
while (current < end)
	{
	end += 2;		// account for the impact_order and the terminator
	current++;		// do the conversion from integer to float only once.
	docid = -1;
	while (*current != 0)
		{
		docid += *current++;
		accumulator->add_document(docid);
		}
	current++;
	}
}

/*
	ANT_RANKING_FUNCTION_NOOP::RANK()
	---------------------------------------
*/
double ANT_ranking_function_noop::rank(ANT_compressable_integer docid, ANT_compressable_integer length, unsigned char term_frequency, long long collection_frequency, long long document_frequency)
{
return (double)1;
#pragma ANT_PRAGMA_UNUSED_PARAMETER
}
