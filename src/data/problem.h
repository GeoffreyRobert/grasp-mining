#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <istream>
#include <ostream>
#include <vector>
#include <string>

using std::vector; using std::string;

/*
	Fichier incluant la déclaration de la structure permettant de stocker les
    informations du problème
*/


class Problem {
	// *************************************************************************
	// JOB SHOP BASE	********************************************************
	// *************************************************************************
public:
    Problem();
    Problem(const string& file_path);

    // méthode pour extraire les données d'un fichier .txt
    void LoadProblemFromPath(const string& file_path);
	void LoadProblemFromStream(std::istream& file);
    string ToString() const;

	void Clear();

	void Init();

	int size;								// taille du problème
	int nJob;								// nombre de jobs
	int nMac;								// nombre de machines
	int lowerBound;							// borne inf du problème

    // caractéristiques des jobs
    int minTime;
    int maxTime;

    vector<vector<int>> operationNumber;// numéro du sommet traité (job, op)
	vector<int> prevOperation;			// numéro de l'op. suivante dans le job
	vector<int> nextOperation;			// numéro de l'op. préc. dans le job
	vector<int> machineNumber;			// numéro de machine qui exec. l'op
	vector<int> timeOnMachine;			// durée à passer sur la machine
	vector<int> opToJob;				// job associé à un numéro d'opération
	vector<int> opToRank;				// rang d'une opération dans un job

};

std::ostream& operator<<(std::ostream&, const Problem&);


#endif