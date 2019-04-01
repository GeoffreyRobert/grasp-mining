#ifndef BIERWIRTH_SEQUENCE_
#define BIERWIRTH_SEQUENCE_


/*
	Fichier incluant diff�rentes d�claration des m�thodes de construction d'un vecteur de Bierwirth et son �valuation
*/

#include <vector>
#include <chrono>

#include "data/problem.h"
#include "data/solution.h"

class BierwirthSequence {
public:
	BierwirthSequence();

	~BierwirthSequence();

	// g�n�ration d'une s�quence al�atoire
	void GenerateRandomSequence(const Problem& problem);

	// m�thodes pour construction par rapport au makespan (heuristique Binato et Al.)
	void GenerateBinato(const Problem& problem, double alpha);

	// echange de deux valeurs dans le vecteur de bierwirth
	void SwapOperation(int sommet1, int sommet2);

	vector<unsigned> sequenceVec;		// vecteur contenant la s�quence de Bierwirth

private:
	clock_t init;							// stockage des temps d'ex�cution
	clock_t timeToBest;
	clock_t totalTime;

	unsigned int makespan;					// makespan de la s�quence
	unsigned int hash;						// valeur de hachage d�finitive

	//unsigned int hash_L;
	//unsigned int hashValue[PROBLEM_SIZE];	// sauvegarde des valeurs de hachage parcourues
};

struct Tab_Seq {
	int sizeTab;
	int maxSize;
	BierwirthSequence TabBier[100+1]; // � modifier si besoin
};

#endif