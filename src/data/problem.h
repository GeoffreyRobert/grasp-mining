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
    Fichier incluant la déclaration de la structure permettant de stocker les
    informations du problème
*/

class Problem {
  // *************************************************************************
  // JOB SHOP BASE	********************************************************
  // *************************************************************************
public:
  Problem(int nJob, int nMac, int lowerBound, std::vector<std::pair<int, int>> operationSpecs);

  string ToString() const;

  void Clear();

  int size; // taille du problème
  int nJob; // nombre de jobs
  int nMac; // nombre de machines
  int lowerBound; // borne inf du problème

  // caractéristiques des jobs
  int minTime;
  int maxTime;

  vector<vector<int>> operationNumber; // numéro du sommet traité (job, op)
  vector<int> prevOperation; // numéro de l'op. suivante dans le job
  vector<int> nextOperation; // numéro de l'op. préc. dans le job
  vector<int> machineNumber; // numéro de machine qui exec. l'op
  vector<int> timeOnMachine; // durée à passer sur la machine
  vector<int> opToJob; // job associé à un numéro d'opération
  vector<int> opToRank; // rang d'une opération dans un job
};

std::ostream& operator<<(std::ostream&, const Problem&);

// méthode pour extraire les données d'un fichier .txt
Problem LoadProblemFromPath(const string& file_path);
Problem LoadProblemFromStream(std::istream& file);

#endif
