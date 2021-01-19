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
	vector< bitset<MAXN> > matrix;
	set<int> wishes;
	set<int> result;
	vector< pair< set<int>, set<int> > > infos;
	map<string, int> fruit2ID;
	map<int, string> ID2Fruit;
	Graph G;

	void readFile(string path);
	void analyzeInfo(pair<set<int>, set<int>> info);
	
public:
	Solver(int num) {
		readFile("../beispiele/spiesse" + to_string(num) + ".txt");
		G = Graph(n, n);
	}
	~Solver(void){}

	void analyzeAllInfos();
	void printInfo(int num);
	void printInfos();
	void printFruits();
	void printWishes();
	set<int> getResult();
	bool checkResult();
};


#endif