#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include<bits/stdc++.h>

#define pb push_back

using namespace std;

class Graph {
	int n1, n2;
	vector<vector<int>> gr;
	vector<int> vis, fin; //for cycle detection


public:	
	Graph(){};
	Graph(int num1, int num2) : n1(num1), n2(num2) {
		gr = vector<vector<int>> (n1 + n2, vector<int>());
		vis = vector<int> (n1 + n2);
		fin = vector<int> (n1 + n2);
	}
	~Graph(void){};

	set<int> comp; 

	void addEdge(int a, int b);
	void printNeighbors (int v);
	void printGraph();
	bool checkCycle(int v);
	bool checkComponent(int v);
	int deg(int v);
	vector<int> getNeighbors(int v);
	void DFS(int v);
	void clearVis() {
		for (auto &x: vis)
			x = 0;
	}
};

#endif