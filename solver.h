#pragma once

#ifndef SOLVER_H
#define SOLVER_H

#include<bits/stdc++.h>

#define pb push_back

using namespace std;

class Solver {
	int n, m;
	vector< vector<int> > matrix;
	vector<string> wishes_words;
	vector<int> wishes;
	vector< pair< vector<int>, vector<string> > > infos;
	map<string, int> fruit2ID;
	map<int, string> ID2Fruit;

	void readFile(string path);
	vector<int> getResult();

public:
	Solver(int num) {
		readFile("./beispiele/spiesse" + to_string(num) + ".txt");
	}
	~Solver(void){}

	void printInfo(int num);
	void printInfos();
	void printFruits();
};


#endif