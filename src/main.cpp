#include <fstream>
#include <filesystem>
#include <iostream>
#include <limits>
#include <vector>

#include <boost/program_options.hpp>

#include "data/problem.h"
//#include "solver/solver_factory.h"
#include "solver/solver.h"
//#include "util/out_js.h"
#include "const-heuristic/binato_heuristic.h"
#include "local-search/empty_search.h"
#include "local-search/laarhoven_search.h"
#include "miner/empty_miner.h"

namespace fs = std::filesystem;
using std::string;
using std::vector;

/*
        Programme principal permettant l'appel des différentes méthodes
*/

namespace po = boost::program_options;

po::variables_map ParseArgs(int argc, char* argv[]);

int main(int argc, char** argv)
{
  auto varmap = ParseArgs(argc, argv);

  if (varmap.count("help"))
    return 0;

  // Récupération des instances à traiter
  const fs::path file_path = fs::absolute(varmap["input-file"].as<string>());
  const int max_replications = varmap["replications"].as<int>();

  if (max_replications < 1)
    return 0;

  // extraction des données
  auto problem = LoadProblemFromPath(file_path);
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

  std::cout << "problem: " << problem.lowerBound << " -- solution: "
        << best_sol.makespan << std::endl;

  return 0;
}

boost::program_options::variables_map ParseArgs(int argc, char* argv[])
{
  po::options_description visible_opts("Allowed options");
  visible_opts.add_options()
    ("help", "produce help message")
    ("replications,r", po::value<int>()->default_value(1), "number of replications of the same solver run");

  po::options_description hidden_opts("Hidden options");
  hidden_opts.add_options()
    ("input-file,i", po::value<string>(), "number of replications of the same solver run");

  po::options_description opts;
  opts.add(visible_opts).add(hidden_opts);

  po::positional_options_description pos;
  pos.add("input-file", 1);

  po::variables_map varmap;
  po::store(
      po::command_line_parser(argc, argv)
        .options(opts).positional(pos).run()
    , varmap);
  po::notify(varmap);

  if (varmap.count("help"))
  {
    std::cout
      << "Usage: grasp-mining [options] <input-file>" << '\n'
      << visible_opts << '\n';
  }

  return varmap;
}
