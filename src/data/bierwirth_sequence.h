#ifndef BIERWIRTH_SEQUENCE_
#define BIERWIRTH_SEQUENCE_


/*
	Fichier incluant différentes déclaration des méthodes de construction d'un vecteur de Bierwirth et son évaluation
*/

#include <vector>
#include <chrono>

#include "data/problem.h"
#include "data/solution.h"

class BierwirthSequence {
public:
	BierwirthSequence();

	~BierwirthSequence();

	// génération d'une séquence aléatoire
	void GenerateRandomSequence(const Problem& problem);

	// méthodes pour construction par rapport au makespan (heuristique Binato et Al.)
	void GenerateBinato(const Problem& problem, double alpha);

	// echange de deux valeurs dans le vecteur de bierwirth
	void SwapOperation(int sommet1, int sommet2);

	vector<unsigned> sequenceVec;		// vecteur contenant la séquence de Bierwirth

private:
	clock_t init;							// stockage des temps d'exécution
	clock_t timeToBest;
	clock_t totalTime;

	unsigned int makespan;					// makespan de la séquence
	unsigned int hash;						// valeur de hachage définitive

	//unsigned int hash_L;
	//unsigned int hashValue[PROBLEM_SIZE];	// sauvegarde des valeurs de hachage parcourues
};

struct Tab_Seq {
	int sizeTab;
	int maxSize;
	BierwirthSequence TabBier[100+1]; // à modifier si besoin
};

#endif