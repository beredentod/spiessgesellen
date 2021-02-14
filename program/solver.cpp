#include "solver.h"

//diese Methode bearbeitet die Informationen aus einer Spießkombination
void Solver::analyzeInfo(pair<set<int>, set<int>> info) {
	//fruits -- die Menge der Obstsorten der Spießkombination
	//nums -- die Menge der Indizes der Obstsorten der Spießkombination
	set<int> nums = info.first, fruits = info.second;

	bitset<MAXN> mask_nums = 0, mask_full = 0, mask_rev = 0;

	//eine Bitmaske mit 1-en auf allen n Stellen
	for (int i = 0; i < n; i++) mask_full ^= (1 << i);

	//die Bitmaske für die Obstsorten aus der Menge
	for (auto x: nums) mask_nums ^= (1 << (x-1));

	//die Bitmaske für die Obstsorten außerhalb der Menge
	mask_rev = ~(mask_nums) & mask_full;
	
	//es wird über die Menge der Obstsorten der Spießkombination
	//	iteriert
	auto it = fruits.begin();

	//es wird über die Obstsorten in der Adjazenzmatrix iteriert
	for (int i = 0; i < n; i++) {
		//falls eine Obstsorte zur Menge der Obstsorten der 
		//	Spießkombination gehört
		if (it != fruits.end() && *it == i) {
			matrix[i] &= mask_nums;
			it++;
		}
		//falls eine Obstsorte außerhalb der Menge der Obstsorten
		//	der Spießkombination liegt
		else
			matrix[i] &= mask_rev;
	}
}

//diese Methode bearbeitet alle Informationen aus allen Spießkombinationen
void Solver::analyzeAllInfos(){
	//alle Spießkombinationen werden bearbeitet
	for (auto x: infos)
		analyzeInfo(x);

	//alle noch übrigen Kanten werden in den Graphen hinzugefügt
	for (int i = 0; i < ID2Fruit.size(); i++) {
		for (int j = matrix[i]._Find_next(-1); j < MAXN; j = matrix[i]._Find_next(j))
			G.addEdge(i, j);
	}
}

//diese Methode prüft, ob die eingegeben Informationen über den Graphen
//	keinen Widerspruch ergeben
bool Solver::checkCoherence(){ 
	//falls zwei im Programm verwendete Knoten auf einer Komponente
	//	unterschiedliche Kardinalitäten haben, ist die Eingabe falsch
	for (int i = 0; i < int(used.size()); i++)
		if (used[i]){
			//falls ein Knoten die Kardinalität von 0 besitzt
			if (G.deg(i) == 0)
				return false;

			//wir nehmen den ersten Nachbarn von i 
			int neigh = G.getFirstNeighbor(i);

			//wir vergleichen die Kardinalitäten
			if (G.deg(neigh) != G.deg(i))
				return false;
		}

	return true;
}

//diese Methode prüft, ob eine eindeutige Lösung exisitiert
bool Solver::checkResult(){
	//die Menge der Obstsorten (Knoten), die einen Grad von
	//	mehr als 1 haben
	set<int> multip;

	//die Liste der Knoten, deren Komponente geprüft werden muss
	vector<int> ready(n);

	//die Liste der Knoten der gewünschten Obstsorten
	vector<int> todo(n);

	//es wird durch die Menge der Wünsche iteriert
	for (auto x: wishes) {
		//falls der Grad des Knotens der gewünschten Obstsorte
		// gleich 1 ist
		if (G.deg(x) == 1) {
			int neigh = G.getFirstNeighbor(x);

			//der einzige Nachabr wird in die Ergebnismenge hinzugefügt
			result.insert(neigh - n + 1);
		}
		//falls der Grad des Knotens der gewünschten Obstsorte
		// mehr als 1 beträgt
		else {
			//der gewünschte Knoten wird in die Menge 
			//	für die Untersuchung der Komponente hinzugefügt
			multip.insert(x);

			//die Obstsorte wird in die To-do-Liste 
			// für die Untersuchung der Komponente hinzugefügt
			todo[x] = 1;
		}
	} 

	//falls alle gewünschten Obstsorten einen Grad von 1 haben
	if (multip.size() == 0)
		return true;

	//diese Variable zeigt an, ob eine Lösung für die Eingabe existiert
	bool solv = true;

	//eine Liste mit Mengen der Obstsorten der Komponenten,
	//	zu deren Knoten gehören, die nicht auf der Wunschliste stehen 
	vector<set<int>> problems;

	//es wird durch die Menge der Knoten der gewünschten Obstsorten iteriert,
	// deren Grad größer als 1 beträgt
	for (auto x: multip) {
		//diese Variable zeigt an, ob mind. ein Knoten zur Komponente
		// gehört, aber nicht gewünscht ist
		bool prob = false;

		//falls die Komponente eines Knotens noch nicht geprüft wurde
		if (!ready[x]) {
			//die Menge der Knoten der Indizes der Obstsorten der jeweiliger Komponente
			vector<int> setB = G.getNeighbors(x);
			//die Menge der Knoten der Obstsorten der jeweiliger Komponente
			vector<int> setA = G.getNeighbors(setB[0]);

			//es wir durch die Knoten der Obstsorten dieser Komponente
			//	iteriert
			for (auto y: setA) {
				//falls die Obstsorte x nicht gewünscht ist,
				//	gibt es keine eindeutige Lösung dür die Eingabe
				if (!todo[y]) {
					solv = false;
					prob = true;
				}
				//falls die Obstsorte x gewünscht ist,
				//	wird sie als geprüft markiert
				else
					ready[y] = 1;
			}

			//falls mind. ein Knoten zur Komponente gehört, aber nicht 
			//	gewünscht ist
			if (prob) {
				//die Menge der Obstsorten der Komponente enthält 
				//	mind. einen Knoten einer Obstsorte, die nicht gewünscht ist
				set<int> currSet;
				for (auto y: setA)
					currSet.insert(y);
				problems.pb(currSet);
			}
			//falls alle Knoten, die zur Komponente gehören, gewünscht sind
			else
				//alle Indizes der Obstsorte der Komponente werden 
				//	in die Menge der Indizes der gewünschten Obstsorten hinzugefügt
				for (auto y: setB)
					result.insert(y - n + 1);
		}
	}
	
	//falls eine Lösung für die Eingabe existiert
	if (solv) {
		return true;
	}
	//falls es keine eindeutige Lösung für die Eingabe gibt
	else {
		cout << "Für die folgenden Obstsorten konnte keine eindeutige Zuweisung gefunden werden.\n";
		for (auto x: problems) {
			cout << "Komponente: ";
			//Aufzählen der Obstsorten, die zur Komponente gehören
			for (auto y: x)
				cout << ID2Fruit[y] << " ";

			//Aufzählen der Obstsorten, die zur Komponente gehören,
			//	 aber nicht gewünscht sind
			cout << "\n\t--> Nicht auf der Wunschliste: ";
			for (auto y: x) 
				if (!todo[y])
					cout << ID2Fruit[y] << " ";
			cout << "\n";
		}
		return false;
	}
}

//diese Methode gibt die Menge der Indizes der gewünschten Obstsorten zurück
set<int> Solver::getResult(){
	return result; 
}

//diese Methode zeigt alle gewünschten Obstsorten an
void Solver::printWishes() {
	cout << "Wünsche: ";
	for (auto x: wishes)
		cout << ID2Fruit[x] << " ";
	cout << "\n";
}


//			=== DEBUG ===
//die folgenden Methoden werden als Debugging-Tools verwende

//diese Methode zeigt eine Spießkombination an
void Solver::printInfo(int num) {
	auto x = infos[num];
	cout << "\tDEBUG: ";
	for (auto y: x.first) cout << y << " ";
	cout << "\n\tDEBUG:";
	for (auto y: x.second) cout << ID2Fruit[y] << " ";
	cout << "\n";	
}

//diese Methode zeigt alle Spießkombinationen an
void Solver::printInfos() {
	cout << "DEBUG: All infos about the sets\n";
	for (int i = 0; i < int(infos.size()); i++)
		printInfo(i);
	cout << "\n";
}

//diese Methode zeigt die internen Indizes der Obstsorten an
void Solver::printFruits() {
	cout << "DEBUG: IDs of Fruits\n";
	for (int i = 0; i < ID2Fruit.size(); i++)
		cout << "\tDEBUG: " << ID2Fruit[i] << "   " << i << "\n";
	cout << "\n";
}

//diese Methode zeigt die Adjazenzmatrix des Graphen an
void Solver::printMatrix() {
	cout << "DEBUG: Matrix\n";
	for (int i = 0; i < n; i++)
		cout << "\tDEBUG: " << matrix[i] << "\n";
	cout << "\n";
}

//diese Methode zeigt die möglichen Zuweisungen der Indizes
//	der jeweiligen Obstsorte an
void Solver::printPossibleAssignments() {
	cout << "DEBUG: Possible assignments\n";
	for (int i = 0; i < ID2Fruit.size(); i++) {
		cout << "\tDEBUG: " << ID2Fruit[i] << "\t";
		for (int j = matrix[i]._Find_next(-1); j < MAXN; j = matrix[i]._Find_next(j))
			cout << j + 1 << " ";
		cout << "\n"; 
	}
	cout << "\n";
}



 