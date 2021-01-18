#pragma once

#ifndef GRAPH_H
#define GRAPH_H

#include<bits/stdc++.h>

#define pb push_back

using namespace std;

class Graph {
	int n;
	vector<vector<int>> gr;

public:	
	Graph(int num) : n(num) {
		for (int i = 0; i < 2*n; i++) gr.pb(vector<int>());
	}
	~Graph(void){};

	void addEdge(int a, int b);
};

#endif