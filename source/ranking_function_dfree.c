/*
	RANKING_FUNCTION_DFREE.C
	------------------------
*/
#ifdef _MSC_VER
	/*
		This is necessary for Microsoft C/C++ to include M_PI
	*/
	#define _USE_MATH_DEFINES
#endif
#include "maths.h"
#include "pragma.h"
#include "ranking_function_dfree.h"
#include "search_engine_btree_leaf.h"
#include "compress.h"
#include "search_engine_accumulator.h"

/*
	ANT_RANKING_FUNCTION_DFREE::RELEVANCE_RANK_TOP_K()
	--------------------------------------------------
*/
void ANT_ranking_function_DFRee::relevance_rank_top_k(ANT_search_engine_result *accumulator, ANT_search_engine_btree_leaf *term_details, ANT_compressable_integer *impact_ordering, long long trim_point, double prescalar, double postscalar)
{
long long docid;
double prior, posterior, InvPriorCollection, norm;
double tf, cf, score;
ANT_compressable_integer *current, *end;

current = impact_ordering;
end = impact_ordering + (term_details->local_document_frequency >= trim_point ? trim_point : term_details->local_document_frequency);
cf = (double)term_details->global_collection_frequency;
while (current < end)
	{
	end += 2;		// account for the impact_order and the terminator
	tf = *current++ * prescalar;
	docid = -1;
	while (*current != 0)
		{
		docid += *current++;

		prior = tf / document_lengths[(size_t)docid];
		posterior = (tf + 1.0) / (document_lengths[(size_t)docid] + 1);
		InvPriorCollection = collection_length_in_terms / tf;
		norm = tf * ANT_log2(posterior / prior);

		score = 1.0 * norm * (tf * (-ANT_log2(prior * InvPriorCollection)) + (tf + 1.0) *  (+ANT_log2(posterior * InvPriorCollection)) + 0.5 * ANT_log2(posterior / prior));

		accumulator->add_rsv(docid, postscalar * score);
		}
	current++;		// skip over the zero
	}
}

/*
	ANT_RANKING_FUNCTION_DFREE::RANK()
	----------------------------------
*/
double ANT_ranking_function_DFRee::rank(ANT_compressable_integer docid, ANT_compressable_integer length, unsigned char term_frequency, long long collection_frequency, long long document_frequency)
{
double tf = term_frequency;
double prior = tf / (double)length;
double posterior = (tf + 1.0) / ((double)length + 1);
double InvPriorCollection = collection_length_in_terms / tf;
double norm = tf * ANT_log2(posterior / prior);

double score = 1.0 * norm * (tf * (-ANT_log2(prior * InvPriorCollection)) + (tf + 1.0) *  (+ANT_log2(posterior * InvPriorCollection)) + 0.5 * ANT_log2(posterior / prior));

return score;
#pragma ANT_PRAGMA_UNUSED_PARAMETER
}