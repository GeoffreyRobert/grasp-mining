#ifndef _PROBLEM_H_
#define _PROBLEM_H_

#include <vector>
#include <string>

using std::vector; using std::string;

/*
	Fichier incluant la d�claration de la structure permettant de stocker les informations du probl�me
*/


class Problem {
	// *********************************************************************************
	// JOB SHOP BASE	****************************************************************
	// *********************************************************************************
public:
	void LoadProblemFromFile(const string& file_path, const string& file_name);	// m�thode pour extraire les donn�es d'un fichier .txt

	void Clear();

	void Init();

	int size;								// taille du probl�me
	int nJob;								// nombre de jobs
	int nMac;								// nombre de machines
	int lowerBound;							// borne inf du probl�me

    vector<vector<int>> operationNumber;	// num�ro du sommet trait� (job, op)
	vector<int> prevOperation;				// num�ro de l'op. suivante dans le job
	vector<int> nextOperation;				// num�ro de l'op. pr�c�dente dans le job
	vector<int> machineNumber;				// num�ro de la machine devant traiter la pi�ce
	vector<int> timeOnMachine;				// dur�e � passer sur la machine
	vector<int> opToJob;					// job associ� � un num�ro d'op�ration
	vector<int> opToRank;					// rang d'une op�ration dans un job

};


#endif