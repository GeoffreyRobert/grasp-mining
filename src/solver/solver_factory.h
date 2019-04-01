#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include <string>

#include "solver/solver.h"
#include "const-heuristic/const_heuristic.h"
#include "local-search/local_search.h"
#include "miner/data_miner.h"

using std::string;

enum class ConstHeuristicType {
	Random,
	Binato
};
enum class LocalSearchType {
	Empty,
	Laarhoven
};
enum class DataMinerType {
	Empty,
	PatternMiner
};

class SolverFactory {
public:
	//SolverFactory(const string& file_path);

	//void LoadParametersFromFile(const string& file_path);
	//void UpdateParameterFromString(const string& param);

	LocalSearch BuildLocalSearch();
	ConstHeuristic BuildConstHeuristic();
	DataMiner BuildDataMiner();

	LocalSearchType local_search_type = LocalSearchType::Laarhoven;
	ConstHeuristicType const_heuristic_type = ConstHeuristicType::Binato;
	DataMinerType data_miner_type = DataMinerType::Empty;
};

#endif // PARAMETERS_H_

