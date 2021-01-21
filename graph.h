#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include<bits/stdc++.h>

#define pb push_back

using namespace std;

class Graph {
	int n1, n2;
	vector<vector<int>> gr;
	vector<int> vis, viscyc, fin; //for cycle detection


public:	
	Graph(){};
	Graph(int num1, int num2) : n1(num1), n2(num2) {
		gr = vector<vector<int>> (n1 + n2, vector<int>());
		vis = vector<int> (n1 + n2);
		viscyc = vector<int> (n1 + n2);
		fin = vector<int> (n1 + n2);
	}
	~Graph(void){};

	set<int> comp, compA, compB; 

	void addEdge(int a, int b);
	void printNeighbors (int v);
	void printGraph();
	bool checkCycle(int v, int par = -1){
		//cout << "here " << v << " " << par << "\n";
		viscyc[v] = true;
		for (auto u: gr[v]){
			if (!viscyc[u]){
				//cout << u << " <- \n";
				if (checkCycle(u, v))
					return true; 
			}
			else if (u != par) {
				//cout << u << " " << par << " hhh\n";
				return true; 
			}
		} 
		return false; 
	}
	//bool checkCycle2(int v);
	int deg(int v);
	vector<int> getNeighbors(int v);
	void DFS(int v);
	void clearVis() {
		for (auto &x: vis)
			x = 0;
	}
};

#endif