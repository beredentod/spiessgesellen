#include "graph.h"

void Graph::addEdge(int a, int b){
	gr[a].pb(b+n);
	gr[b+n].pb(a);
}

