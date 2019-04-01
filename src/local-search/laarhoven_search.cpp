#include <utility>

#include "laarhoven_search.h"

Solution& LaarhovenSearch::operator()(const Problem& problem, Solution& solution) {
	int operation = solution.criticalOp;			// op. considérée pour relocation
	int parent;										// parent de l'operation
	if (solution.isCritMachine[operation]) {
		parent = solution.macParent[operation];
	}
	else {
		parent = problem.prevOperation[operation];
	}

	// remonter le chemin critique
	while (parent != -1) {
		// verification que les deux ops. ne sont pas dans le meme job
		if (problem.jobForOp[operation] != problem.jobForOp[parent]) {

			// on inverse deux ops. sur le chemin critique si réduction du makespan
			if (solution.CheckAndSwap(operation, parent)) {
				operation = solution.criticalOp;	// on repart de la fin
			}
			// on avance le long du chemin critique
			else {
				operation = parent;
			}
		}
		// sinon on progresse sur le chemin critique
		else {
			operation = parent;
		}
		if (solution.isCritMachine[operation]) {
			parent = solution.macParent[operation];
		}
		else {
			parent = problem.prevOperation[operation];
		}
	}
	return solution;

}