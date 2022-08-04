#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;

/*
    Fichier incluant la d�claration de la structure permettant de stocker les
    informations du probl�me
*/

class Problem {
  // *************************************************************************
  // JOB SHOP BASE	********************************************************
  // *************************************************************************
public:
  Problem(int nJob, int nMac, int lowerBound, std::vector<std::pair<int, int>> operationSpecs);

  string ToString() const;

  void Clear();

  int size; // taille du probl�me
  int nJob; // nombre de jobs
  int nMac; // nombre de machines
  int lowerBound; // borne inf du probl�me

  // caract�ristiques des jobs
  int minTime;
  int maxTime;

  vector<vector<int>> operationNumber; // num�ro du sommet trait� (job, op)
  vector<int> prevOperation; // num�ro de l'op. suivante dans le job
  vector<int> nextOperation; // num�ro de l'op. pr�c. dans le job
  vector<int> machineNumber; // num�ro de machine qui exec. l'op
  vector<int> timeOnMachine; // dur�e � passer sur la machine
  vector<int> opToJob; // job associ� � un num�ro d'op�ration
  vector<int> opToRank; // rang d'une op�ration dans un job
};

std::ostream& operator<<(std::ostream&, const Problem&);

// m�thode pour extraire les donn�es d'un fichier .txt
Problem LoadProblemFromPath(const string& file_path);
Problem LoadProblemFromStream(std::istream& file);

#endif
