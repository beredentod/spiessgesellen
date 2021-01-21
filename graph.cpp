#include "graph.h"

void Graph::addEdge(int a, int b){
	gr[a].pb(b+n1);
	gr[b+n1].pb(a);
}

void Graph::printNeighbors(int v){
	cout << v << ": ";
	for (auto x: gr[v])
		cout << x << " ";
	cout << "\n";
}

void Graph::printGraph(){
	for (int i = 0; i < n1 + n2; i++) 
		printNeighbors(i);
}



/*bool Graph::checkCycle2(int v){
	if (vis[v])
		return false;
	if (fin[v])
		return true;

	fin[v] = true;
	for (auto u: gr[v])
		if (checkCycle2(u))
			return true;

	vis[v] = true;
	return false;
}*/

void Graph::DFS(int v) {
	comp.insert(v);

	if (v < n1)
		compA.insert(v);
	else
		compB.insert(v);

	vis[v] = true;
	for (auto u: gr[v])
		if (!vis[u])
			DFS(u);
}

int Graph::deg(int v) {
	return gr[v].size();
}

vector<int> Graph::getNeighbors(int v) {
	return gr[v];
}
