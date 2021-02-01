#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include<bits/stdc++.h>

#define pb push_back

using namespace std;

class Graph {
	//die Anzahl der Knoten in der jeweiligen Partition
	int n1, n2;
	
	//der Graph als Adjazenliste
	vector<vector<int>> gr;
public:	
	Graph(){};
	Graph(int num1, int num2) : n1(num1), n2(num2) {
		//die Erstellung der Adjazenzliste
		gr = vector<vector<int>> (n1 + n2, vector<int>());
	}
	~Graph(void){};
	
	//diese Methode gibt den ersten Nachbarn eines Knotens zurück
	int getFirstNeighbor(int v);

	//diese Methode fügt eine Kante zwischen den Knoten a und b hinzu
	void addEdge(int a, int b);

	//diese Methode gibt den Grad des Knotens zurück
	int deg(int v);

	//diese Methode gibt die Nachbarliste eines Knotens zurück
	vector<int> getNeighbors(int v);

//			=== DEBUG ===
//die folgenden Methoden werden als Debugging-Tools verwendet

	//diese Methode zeigt alle Nachbarknoten des Knotens v an
	void printNeighbors (int v);

	//diese Methode zeigt alle Nachbarknoten der allen Knoten im Graphen an
	void printGraph();
};

#endif
