#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include<bits/stdc++.h>
#include"graph.h"

#define pb push_back

using namespace std;

const int MAXN = 26;

class Solver {
	int n, m;
	vector< pair<set<int>, set<int>> > infos;
	vector< bitset<MAXN> > matrix;
	vector<int> used;
	set<int> wishes;
	
	map<string, int> fruit2ID;
	map<int, string> ID2Fruit;
	Graph G;
	
	set<int> result;

	void readFile(string path);
	void analyzeInfo(pair<set<int>, set<int>> info);
	
public:
	Solver(string path) {
		readFile(path);
		G = Graph(n, n);
	}
	~Solver(void){}

	void analyzeAllInfos();
	bool checkCoherence();
	bool checkResult();
	set<int> getResult();
	void printWishes();

//DEBUG
	void printInfo(int num);
	void printInfos();
	void printFruits();
	void printMatrix();
	void printPossibleAssignments();
};


#endif