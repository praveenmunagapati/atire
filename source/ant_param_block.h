/*
	ANT_PARAM_BLOCK.H
	-----------------
*/
#ifndef __ANT_PARAM_BLOCK_H__
#define __ANT_PARAM_BLOCK_H__
/*
	class ANT_ANT_PARAM_BLOCK
	-------------------------
*/
class ANT_ANT_param_block
{
public:
	enum { MAP, MAgP, RANKEFF, P_AT_N } ;									// metrics
	enum { /* NONE = 0, */ INEX = 1, TREC = 2, INEX_EFFICIENCY = 4 } ;		// evaluation forum
	enum { NONE = 0, QUERY = 1, SUM = 2, SHORT = 4 };						// statistics to print (bitstring)
	enum { BM25, IMPACT, READABLE, LMD, LMJM, BOSE_EINSTEIN, DIVERGENCE};	// ranking function
	enum { /* NONE = 0, */ THRESHOLD = 1, WEIGHTED = 2 }; 					// stemming stuff
	enum { INDEX_IN_FILE = 0, INDEX_IN_MEMORY = 1};							// read the index from disk or load at startup

private:
	int argc;
	char **argv;

public:
	long logo;							// display the ANT banner logo or not
	long stemmer;						// which stemmer to use (or 0 for don't stem)
	long stemmer_similarity;			// 'correct' stemming with the thesaurus?
	double stemmer_similarity_threshold;	// threshold for rejecting stems with the thesaurus (default = 0.0)

	long long sort_top_k;				// accurate rank point in the accumulator sort (in the call to sort_results_list())
	long long trim_postings_k;			// trim the postigs lists at no fewer than k
	long metric;						// which metric to use (MAP, MAgP, etc)
	long metric_n;						// so we can so P@10, p@30 and so on using one metric
	char *assessments_filename;			// name of the file containing the assessments for the given queries
	char *queries_filename;				// name of a file containing one query per line (first token of each line is the query ID)
	long output_forum;					// export the list of results in INEX or TREC format
	char *output_filename;				// name of the run file
	char *participant_id;				// participant id (used in the export file)
	char *run_name;						// name of the run (used in the export file)
	long results_list_length;			// length of the results list as exported
	long stats;							// which stats to display
	long ranking_function;				// what ranking function should we use?

	double lmd_u;						// the u value for Language Modelling with Dirichlet smoothing
	double lmjm_l;						// the l (lamda) value for Language Modelling with Jelinek-Mercer smoothing
	double bm25_k1, bm25_b;				// the k1 and b value for BM25

	long segmentation;					// query segmentation need or not for east-asian languages, e.g. Chinese
	long file_or_memory;				// the index be loaded into memory at startup (TRUE / FALSE)

private:
	void export_format(char *forum);
	void term_expansion(char *which);
	void set_metric(char *which);
	void set_stats(char *which);
	void set_ranker(char *which);
	void get_two_parameters(char *from, double *first, double *second);
	void get_one_parameter(char *from, double *into);

public:
	ANT_ANT_param_block(int argc, char *argv[]);
	~ANT_ANT_param_block();

	void usage(void);
	void help(void);
	long parse(void);
} ;

#endif __ANT_PARAM_BLOCK_H__

