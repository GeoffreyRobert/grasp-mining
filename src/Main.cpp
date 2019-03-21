#include <vector>

#include "Methods.h"
#include "data/bierwirth_sequence.h"
#include "data/problem.h"
#include "util/out_js.h"

/*
	Programme principal permettant l'appel des différentes méthodes
*/

int main (int argc, char** argv) {
	// Récupération des instances à traiter
	vector<const string> instances;
	{
	ifstream fichier("..\\instances\\instances_name.txt", ios::in);
	string problem_name;
	while (fichier >> problem_name) instances.push_back(problem_name);
	fichier.close();
	}

	// Déclaration d'un problème et d'un vecteur
	Problem problem;
	BierwirthSequence b, a;

	// Structure de stockage des résultats
	OutJS res;
	for (const string& problem_name: instances) {
		// extraction des données
		problem.LoadProblemFromFile(problem_name);

		for (int i = 0; i < MAX_REPLICATION; ++i) {
			init_genrand64(i);

			b = GRASP(data, 1000, 2); // random
		}
		writeGANTT_SVG(problem, b, problem_name);
		
		problem.clear();
	}

	return 0;
} 