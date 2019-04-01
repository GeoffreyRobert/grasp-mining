//#include <fstream>

#include "solver_factory.h"
#include "const-heuristic/binato_heuristic.h"
#include "const-heuristic/random_placement.h"
#include "local-search/empty_search.h"
#include "local-search/laarhoven_search.h"
#include "miner/empty_miner.h"
#include "miner/pattern_miner.h"

//SolverFactory::SolverFactory(const string& file_path) {
//	LoadParametersFromFile(file_path);
//}
//
//void SolverFactory::LoadParametersFromFile(const string& file_path) {
//	string param;
//	std::ifstream file(file_path, std::ios::in);
//	while (file >> param) {
//		UpdateParameterFromString(param);
//	}
//	file.close();
//}
//
//void SolverFactory::UpdateParameterFromString(const string& param) {
//	int pos = param.find('::');
//	string type = param.substr(0, pos);
//	string val = param.substr(0, pos + 2);
//	switch (type)
//	{
//	case "ConstHeuristicType":
//		switch (val)
//		{
//		case "Binato": const_heuristic_type = ConstHeuristicType::Random; break;
//		case "Random":
//		default: const_heuristic_type = ConstHeuristicType::Random;
//		}
//		break;
//	case "LocalSearchType":
//		switch (val)
//		{
//		case "Laarhoven": local_search_type = LocalSearchType::Laarhoven;
//		case "Empty":
//		default: local_search_type = LocalSearchType::Empty;
//		}
//		break;
//	case "DataMinerTyper":
//		switch (val)
//		{
//		case "PatternMiner": data_miner_type = DataMinerType::PatternMiner;
//		case "Empty":
//		default: data_miner_type = DataMinerType::Empty;
//		}
//		break;
//	}
//}

ConstHeuristic SolverFactory::BuildConstHeuristic() {
	switch (const_heuristic_type)
	{
	case ConstHeuristicType::Binato:
		return BinatoHeuristic();
	case ConstHeuristicType::Random:
	default:
		return RandomPlacement();
	}
}

LocalSearch SolverFactory::BuildLocalSearch() {
	switch (local_search_type) {
	case LocalSearchType::Laarhoven:
		return LaarhovenSearch();
	case LocalSearchType::Empty:
	default:
		return EmptySearch();
	}
}

DataMiner SolverFactory::BuildDataMiner() {
	switch (data_miner_type)
	{
	case DataMinerType::PatternMiner:
		return PatternMiner();
	case DataMinerType::Empty:
	default:
		return EmptyMiner();
	}
}