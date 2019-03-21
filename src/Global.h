#ifndef __GLOBAL__
#define __GLOBAL__

// ****************************************************************************************
//		Fichier contenant les différents include et variables Globales pour les tableaux statiques
// ****************************************************************************************

#include <string>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include "util/MT.h"
using namespace std;


#define MAX_JOBS  50							// nombre max de jobs que l'on peut traiter
#define MAX_MACHINES  20						// nombre max de machines
#define PROBLEM_SIZE 400	// taille maximale du problème
#define HASH_SIZE 999999						// taille de la table de Hachage
#define INFINITE_C 999999							// valeur simulant l'infini
#define ITER_MAX 1000								// nombre max d'iterations
#define MAX_INSTANCES 100						// nombre max d'instances
#define MAX_REPLICATION 10						// nombre de replication pour chaque instance


#endif