#include "graph.h"

//diese Methode fügt eine Kante zwischen den Knoten a und b hinzu
void Graph::addEdge(int a, int b){
	gr[a].pb(b+n1);
	gr[b+n1].pb(a);
}

//diese Methode gibt den Grad des Knotens zurück
int Graph::deg(int v) {
	return gr[v].size();
}

//diese Methode gibt die Nachbarliste eines Knotens zurück
vector<int> Graph::getNeighbors(int v) {
	return gr[v];
}

//			=== DEBUG ===
//die folgenden Methoden werden als Debugging-Tools verwendet

//diese Methode zeigt alle Nachbarknoten des Knotens v an
void Graph::printNeighbors(int v){
	cout << "DEBUG: Neighbors of " << v << ": ";
	for (auto x: gr[v])
		cout << x << " ";
	cout << "\n";
}

//diese Methode zeigt alle Nachbarknoten der allen Knoten im Graphen an
void Graph::printGraph(){
	cout << "DEBUG: Whole graph\n";
	for (int i = 0; i < n1 + n2; i++) 
		printNeighbors(i);
	cout << "\n";
}