#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <istream>
#include <ostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;

/*
    Fichier incluant la d�claration de la structure permettant de stocker les
    informations du probl�me
*/

using OperationId = unsigned;
using JobId = unsigned;
using MachineId = unsigned;
using OperationRank = unsigned;

class Problem {
  // *************************************************************************
  // JOB SHOP BASE	********************************************************
  // *************************************************************************
public:
  Problem(JobId nJob, MachineId nMac, int lowerBound, vector<std::pair<unsigned, int>> operationSpecs);

  string ToString() const;

  void Clear();

  static constexpr OperationId InvalidOp = std::numeric_limits<OperationId>::max();
  JobId nJob; // nombre de jobs
  MachineId nMac; // nombre de machines
  OperationId size; // taille du probl�me
  int lowerBound; // borne inf du probl�me

  // caract�ristiques des jobs
  int minTime;
  int maxTime;

  vector<vector<OperationId>> operationNumber; // num�ro du sommet trait� (job, op)

  vector<OperationId> prevOperation; // num�ro de l'op. suivante dans le job
  vector<OperationId> nextOperation; // num�ro de l'op. pr�c. dans le job
  vector<MachineId> machineNumber; // num�ro de machine qui exec. l'op
  vector<int> timeOnMachine; // dur�e � passer sur la machine
};

std::ostream& operator<<(std::ostream&, const Problem&);

// m�thode pour extraire les donn�es d'un fichier .txt
Problem LoadProblemFromPath(const string& file_path);
Problem LoadProblemFromStream(std::istream& file);

#endif
