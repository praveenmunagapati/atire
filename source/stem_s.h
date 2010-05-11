/*
	STEM_S.H
	--------
*/
#ifndef STEM_S_H_
#define STEM_S_H_

#include "stem.h"

/*
	class ANT_STEM_S
	----------------
*/
class ANT_stem_s : public ANT_stem
{
public:
	ANT_stem_s() {}
	virtual ~ANT_stem_s() {}
	virtual size_t stem(char *term, char *destination);
} ;

#endif /* STEM_S_H_ */