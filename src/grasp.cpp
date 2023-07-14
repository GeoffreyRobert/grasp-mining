#include <fstream>
#include <filesystem>
#include <iostream>
#include <limits>
#include <vector>

#include <boost/program_options.hpp>

#include "data/problem.h"
#include "solver/solver.h"
#include "const-heuristic/scheduled_generator.h"
#include "const-heuristic/restricted_selector.h"
#include "const-heuristic/const_heuristic.h"
#include "local-search/laarhoven_search.h"
#include "miner/empty_miner.h"

namespace fs = std::filesystem;

using std::string; using std::vector;

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
  const unsigned population_size = varmap["population"].as<unsigned>();
  const double alpha = varmap["alpha"].as<double>();
  const unsigned seed = varmap["seed"].as<unsigned>();

  if (population_size < 1)
    return 0;

  // extraction des données
  auto problem = LoadProblemFromPath(file_path);

  // build the solver NOT using mining
  ScheduledGenerator generator(problem);
  RestrictedSelector selector(problem, alpha, seed);
  ConstHeuristic init_heuristic(problem, generator, selector);
  LaarhovenSearch local_search(problem);
  EmptyMiner data_miner(problem);
  Solver solver(
      init_heuristic
    , local_search
    , data_miner
    , init_heuristic
    , population_size);

  Solution solution = solver.Solve(problem);

  std::string instance = file_path.filename().string();
  instance = instance.substr(0, instance.length() - 4);
  std::cout
    << "instance: " << instance << " -- "
    << "low bound: " << problem.lowerBound << " -- "
    << "solution: " << solution.Makespan() << " -- ";
  auto seconds = solver.runtime.count() / 1000;
  auto milliseconds = solver.runtime.count() % 1000;
  std::cout
    << "runtime: " << seconds << '.' << milliseconds << 's' << '\n';

  return 0;
}

boost::program_options::variables_map ParseArgs(int argc, char* argv[])
{
  po::options_description visible_opts("Allowed options");
  visible_opts.add_options()
    ("help", "produce help message")
    ("population,p", po::value<unsigned>()->default_value(1000), "number of population of the same solver run")
    ("alpha,a", po::value<double>()->default_value(0.5), "alpha parameter of the binato heuristic")
    ("seed,s", po::value<unsigned>()->default_value(0), "seed to be used by all random generators");

  po::options_description hidden_opts("Hidden options");
  hidden_opts.add_options()
    ("input-file,i", po::value<string>(), "file path of the problem definition");

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
