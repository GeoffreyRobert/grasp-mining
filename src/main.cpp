#include <vector>
#include <fstream>
#include <iostream>
#include <limits>

#include "data/problem.h"
//#include "solver/solver_factory.h"
#include "solver/solver.h"
//#include "util/out_js.h"
#include "const-heuristic/binato_heuristic.h"
#include "local-search/laarhoven_search.h"
#include "local-search/empty_search.h"
#include "miner/empty_miner.h"

using std::vector; using std::string; using std::cout; 

/*
	Programme principal permettant l'appel des différentes méthodes
*/

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

	// Récupération des instances à traiter
	const string file_path = "C:\\Users\\geoff\\EMSE\\Projets Ecole\\"
							 "Projet Recherche\\implementation\\"
							 "grasp-mining\\instances\\";
	vector<string> instances;
	{
		std::ifstream file(file_path + "instances_name.txt", std::ios::in);
		string problem_name;
		while (file >> problem_name) instances.push_back(problem_name);
		file.close();
	}

	//OutJS res;			// Structure de stockage des résultats
	std::ofstream file(file_path + "_results.txt", std::ios::out);
	int max_replications = 1; // TEMPORAIRE

	for (const string& problem_name: instances) {
		file << "problem name : " << problem_name << '\n';
		// extraction des données
		auto problem = LoadProblemFromPath(file_path + problem_name);
		Solution best_sol(problem);
		best_sol.makespan = std::numeric_limits<int>::max();
		Solution sol(problem);

        // Construction du solver
        BinatoHeuristic init_heuristic(problem, 0.5);
        BinatoHeuristic const_heuristic(problem);
        LaarhovenSearch local_search(problem);
        EmptyMiner data_miner(problem);
        Solver solver(init_heuristic, const_heuristic, local_search, data_miner, 20);

        for (int i = 0; i < max_replications; ++i) {
			sol = solver.Solve(problem);
			if (sol.makespan < best_sol.makespan) {
				best_sol = sol;
			}
		}
		file << "problem: " << problem.lowerBound << " -- solution: "
			<< best_sol.makespan << std::endl;
		//writeGANTT_SVG(problem, b, problem_name);
	}

	return 0;
} 
