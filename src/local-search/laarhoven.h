#ifndef LAARHOVEN_
#define LAARHOVEN_

#include "data/bierwirth_sequence.h"
#include "data/problem.h"

class LocalSearchLaarhoven
{
public:
	LocalSearchLaarhoven();
	~LocalSearchLaarhoven();

	// Recherche locale le long du chemin critique
	void operator() (Problem& problem, BierwirthSequence& seq, BierwirthSequence& loc_seq);

private:

};

#endif // LAARHOVEN_