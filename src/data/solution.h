#ifndef SOLUTION_
#define SOLUTION_

#include "Global.h"
#include "bierwirth_sequence.h"
#include "problem.h"

class Solution {
public:
	Solution(const Problem& problem);
	~Solution();

	// Evaluation d'une sequence
	void Evaluate(BierwirthSequence& sequence, int start);

private:
	const Problem& problem_;
	unsigned int makespan;

	vector<unsigned int> parent;				// pere de chaque opération
	vector<unsigned int> startDate;				// date de début de chaque opération
	vector<unsigned int> endDate;				// date de fin de chaque operation

	vector<unsigned int> opNumber;				// sauvegarde l'étape de production pour un sommet
	vector<unsigned int> parentList;			// parent de chaque opération
	vector<unsigned int> childList;				// successeur de chaque opération
};
#endif
