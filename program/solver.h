#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include<bits/stdc++.h>
#include "graph.h"
#include "hashing.h"

#define pb push_back

using namespace std;

class Solver : private Hashing {
	//die Adjazenzmatrix des Graphen
	vector< bitset<MAXN> > matrix;

	//der Graph, der zur einfacheren Darstellung des Problems 
	//	verwendet wird
	Graph G;
	
	//die Menge der Indizes der gewünschten Obstsorten
	set<int> result;

	//diese Methode bearbeitet die Informationen aus einer Spießkombination
	void analyzeInfo(pair<set<int>, set<int>> info);
	
public:
	Solver(string path) {
		readFile(path);

		//Erstellung der Adjazenzmatrix
		for (int i = 0; i < n + 1; i++) {
			bitset<MAXN> b = bitset<MAXN>().set();
			matrix.pb(b);
		}

		//ein bipartiter Graph mit jeweils n Knoten 
		//	pro Partition wird erstellt
		G = Graph(n, n);
	}
	~Solver(void){}

	//diese Methode bearbeitet alle Informationen aus allen Spießkombinationen
	void analyzeAllInfos();

	//diese Methode prüft, ob die eingegeben Informationen über den Graphen
	//	keinen Widerspruch ergeben
	bool checkCoherence();

	//diese Methode prüft, ob eine eindeutige Lösung exisitiert
	bool checkResult();

	//diese Methode gibt die Menge der Indizes der gewünschten Obstsorten zurück
	set<int> getResult();

	//diese Methode zeigt alle gewünschten Obstsorten an
	void printWishes();


//			=== DEBUG ===
//die folgenden Methoden werden als Debugging-Tools verwendet

	//diese Methode zeigt eine Spießkombination an
	void printInfo(int num);

	//diese Methode zeigt alle Spießkombinationen an
	void printInfos();

	//diese Methode zeigt die internen Indizes der Obstsorten an
	void printFruits();

	//diese Methode zeigt die Adjazenzmatrix des Graphen an
	void printMatrix();

	//diese Methode zeigt die möglichen Zuweisungen der Indizes
	//	der jeweiligen Obstsorte an
	void printPossibleAssignments();
};

#endif
