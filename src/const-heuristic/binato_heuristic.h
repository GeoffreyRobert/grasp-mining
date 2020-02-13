#ifndef BINATO_HEURISTIC_H_
#define BINATO_HEURISTIC_H_

#include <random>
#include <vector>

#include "const_heuristic.h"

class BinatoHeuristic : public ConstHeuristic {
public:
	BinatoHeuristic(const Problem&, double alpha = 0.5);

	// construction greedy randomisée
	Solution& operator()(Solution&);

private:
	double alpha = 0.5;
	std::mt19937 generator;

    // Gestion des contraintes de dépendance
    vector<int> last_op_on_mac;	// dernière op. traitée par mach.
    vector<int> num_ops_of_job;	// nombre d'op. traitées par job

    // Gestion de la Restricted Candidate List
    vector<int> rc_list;				// Restricted Candidate List

    // Gestion des candidats à la RCL, de leur parent et du makespan
    vector<int> candidate_jobs;
    vector<int> tmp_parent_list;
    vector<int> tmp_mkspan_list;
    vector<bool> tmp_is_on_mac;
};
#endif // !BINATO_HEURISTIC_H_
