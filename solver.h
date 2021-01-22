#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include<bits/stdc++.h>
#include"graph.h"

#define pb push_back

using namespace std;

//die maximale Anzahl an Obstsorten
const int MAXN = 26;

class Solver {
	//n -- die Anzahl an Obstsorten
	//m -- die Anzahl der Spießkombinationen
	int n, m;

	//die Spießkombinationen als < die Menge der Obsorten > 
	//	und < die Menge der Indizes >
	vector< pair<set<int>, set<int>> > infos;

	//die Adjazenzmatrzi des Graphen
	vector< bitset<MAXN> > matrix;

	//die Liste der im Programm verwendeten Indizes
	//	zur Darstellung der Obstsorten und der Indizes der Obstsorten
	vector<int> used;

	//die Menge der gewünschten Obstsorten
	set<int> wishes;
	
	//eine Map, die jedem Obstnamen einen internen Index zuweist
	map<string, int> fruit2ID;
	//eine Map, die jedem internen Index einen Obstnamen zuweist
	map<int, string> ID2Fruit;

	//der Graph, der zur einfacheren Darstellung des Problems 
	//	verwendet wird
	Graph G;
	
	//die Menge der Indizes der gewünschten Obstsorten
	set<int> result;

	//diese Methode liest Informationen aus der Datei ein 
	void readFile(string path);

	//diese Methode bearbeitet die Informationen aus einer Spießkombination
	void analyzeInfo(pair<set<int>, set<int>> info);
	
public:
	Solver(string path) {
		readFile(path);
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