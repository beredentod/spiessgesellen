#include "solver.h"

//diese Methode liest Informationen aus der Datei ein 
void Solver::readFile(string path) {
	fstream file;
	file.open(path, ios::in);
	if (file.is_open()) {

		//Anzahl der Obstsorten
		file >> n;

		//Erstellung der Adjazenzmatrix
		for (int i = 0; i < n + 1; i++) {
			bitset<MAXN> b = bitset<MAXN>().set();
			matrix.pb(b);
		}

		//Erstellung der Liste der im Programm verwendeten internen Indizes
		used = vector<int>(n + n);

		string data;
		getline(file, data);
		getline(file, data);
		istringstream iss(data);

		//die gewünschten Obstsorten werden erstmal als Strings gespeichert
		vector<string> wishes_words{istream_iterator<string>{iss}, istream_iterator<string>{}};

		//die Menge der allen Obstsorten als Strings
		set<string> all_fruits(wishes_words.begin(), wishes_words.end());

		//Anzahl der Spießkombinationen
		file >> m;
		getline(file, data);

		//Spießkombinationen werden erstmal als ein Menge von Zahlen und 
		//	eine Menge von Strings gespeichert
		vector<pair<set<int>, vector<string>>> tempInfos;

		for (int i = 0; i < m; i++) {
			getline(file, data);
			istringstream isss(data);
			vector<string> words{istream_iterator<string>{isss}, istream_iterator<string>{}};

			//die Menge der Indizes der jeweiligen Spießkombination wird
			//	erstellt
			set<int> currNum;
			for (auto x: words) {
				currNum.insert(stoi(x));

				//der interne Index eines Index einer Obstsorte wird
				//	als verwendet merkiert
				used[stoi(x)+n-1] = true;
			}

			getline(file, data);
			istringstream issss(data);

			//die Menge der Obstsorten der jeweiligen Spießkombination wird
			//	erstmal als Strings erstellt
			vector<string> currFruits{istream_iterator<string>{issss}, istream_iterator<string>{}};

			//alle Obstsorten werden zu einer gemeinsamen Menge hizugefügt
			for (auto x: currFruits)
				all_fruits.insert(x);

			//falls die beiden Menge einer Spießkombination nicht gleichmächtig sind
			if (currNum.size() != currFruits.size()) {
				cerr << "Error: Es gibt Fehler in der Eingabedatei.\n";
				exit(0);
			}

			//eine Spießkombination als ein Menge von Zahlen und 
			//	eine Menge von Strings wird gespeichert
			tempInfos.pb({currNum, currFruits});
		}

		//Zuweisung der internen Indizes 0..(n-1) jeder Obstsorte
		int it = 0;
		for (auto x: all_fruits) {
			ID2Fruit[it] = x;
			fruit2ID[x] = it;

			//der interne Index einer Obstsorte wird als verwendet merkiert
			used[it] = true;
			it++;
		}

		file.close();

		//die gewünschten Obstsorten werden als Indizes gespeichert
		for (auto x: wishes_words)
			wishes.insert(fruit2ID.find(x)->second);

		//die Spießkombinationen als Mengen der Indizes der Obstsorten
		//	einer Spießkombination und Mengen der internen Indizes der Obstsorten
		for (auto x: tempInfos) {
			set<int> currFruits;
			for (auto y: x.second)
				currFruits.insert(fruit2ID.find(y)->second);
			infos.pb({x.first, currFruits});
		}
	} 
	//falls die Textdatei nicht geöffnet werden kann
	else {
		cerr << "Error: File could not be opened. Abort.\n";
		exit(0);
	}
}

//diese Methode bearbeitet die Informationen aus einer Spießkombination
void Solver::analyzeInfo(pair<set<int>, set<int>> info) {
	//fruits -- die Menge der Obstsorten der Spießkombination
	//nums -- die Menge der Indizes der Obstsorten der Spießkombination
	set<int> nums = info.first, fruits = info.second;

	bitset<MAXN> mask_nums = 0, mask_full = 0, mask_rev = 0;

	//eine Bitmaske mit 1-en auf allen n Stellen
	for (int i = 0; i < n; i++) mask_full ^= (1 << i);

	//die Bitmaske für die Obstsorten aus der Menge
	for (auto x: nums) mask_nums ^= (1 << x-1);

	//die Bitmaske für die Obstsorten außerhalb der Menge
	mask_rev = ~(mask_nums) & mask_full;
	
	//es wird über die Menge der Obstsorten der Spießkombination
	//	iteriert
	auto it = fruits.begin();

	//es wird über die Obstsorten in der Adjazenzmatrix iteriert
	for (int i = 0; i < n; i++) {
		//falls eine Obstsorte zur Menge der Obstsorten der 
		//	Spießkombination gehört
		if (*it == i && it != fruits.end()) {
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
	for (auto x: ID2Fruit) {
		for (int i = matrix[x.first]._Find_next(-1); i < MAXN; i = matrix[x.first]._Find_next(i))
			G.addEdge(x.first, i);
	}
}

//diese Methode prüft, ob die eingegeben Informationen über den Graphen
//	keinen Widerspruch ergeben
bool Solver::checkCoherence(){ 
	//die Anzahl der nicht verwendeten Indizes 
	//	der Obstsorten (notusedA) und der Indizes der Obstsorten (notusedB)
	int notusedA = 0, notusedB = 0;

	//es wird gezählt, wie viele von diesen im Programm nicht verwendet werden
	for (int i = 0; i < n; i++)
		if (!used[i]) notusedA++;
	for (int i = 0; i < n; i++)
		if (!used[i]) notusedB++;

	//falls die Anzahl der nicht verwendeten in den beiden Partitionen nicht übereinstimmt,
	//	ist die Eingabe falsch
	if (notusedA != notusedB)
		return false;

	//falls ein im Programm verwendeter Knoten den Grad 0 hat,
	//	ist die Eingabe falsch
	for (int i = 0; i < int(used.size()); i++)
		if (used[i] && G.deg(i) == 0)
			return false;

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
			vector<int> neigh = G.getNeighbors(x);

			//der einzige Nachabr wird in die Ergebnismenge hinzugefügt
			result.insert(neigh[0] - n + 1);
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
			for (auto x: setA) {
				//falls die Obstsorte x nicht gewünscht ist,
				//	gibt es keine eindeutige Lösung dür die Eingabe
				if (!todo[x]) {
					solv = false;
					prob = true;
				}
				//falls die Obstsorte x gewünscht ist,
				//	wird sie als geprüft markiert
				else
					ready[x] = 1;
			}

			//falls mind. ein Knoten zur Komponente gehört, aber nicht 
			//	gewünscht ist
			if (prob) {
				//die Menge der Obstsorten der Komponente enthält 
				//	mind. einen Knoten einer Obstsorte, die nicht gewünscht ist
				set<int> currSet;
				for (auto x: setA)
					currSet.insert(x);
				problems.pb(currSet);
			}
			//falls alle Knoten, die zur Komponente gehören, gewünscht sind
			else
				//alle Indizes der Obstsorte der Komponente werden 
				//	in die Menge der Indizes der gewünschten Obstsorten hinzugefügt
				for (auto x: setB)
					result.insert(x - n + 1);
		}
	}
	
	//falls eine Lösung für die Eingabe existiert
	if (solv) {
		return true;
	}
	//falls es keine eindeutige Lösung für die Eingabe gibt
	else {
		cout << "Für die folgenden Obstsorten konnte keine eideutige Zuweisung gefunden werden.\n";
		for (auto x: problems) {
			cout << "Komponente: ";
			//Aufzählen der Obstsorten, die zur Komponente gehören
			for (auto y: x)
				cout << ID2Fruit.find(y)->second << " ";
			cout << "\n\t--> Nicht auf der Wunschliste: ";
			//Aufzählen der Obstsorten, die zur Komponente gehören,
			//	 aber nicht gewünscht sind
			for (auto y: x) 
				if (!todo[y])
					cout << ID2Fruit.find(y)->second << " ";
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
		cout << ID2Fruit.find(x)->second << " ";
	cout << "\n";
}

//			=== DEBUG ===
//die folgenden Methoden werden als Debugging-Tools verwende

//diese Methode zeigt eine Spießkombination an
void Solver::printInfo(int num) {
	auto x = infos[num];
	for (auto y: x.first) cout << y << " ";
	cout << "\n";
	for (auto y: x.second) cout << ID2Fruit.find(y)->second << " ";
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
	for (auto x: fruit2ID)
		cout << "DEBUG: " << x.first << "   " << x.second << "\n";	
	cout << "\n";
}

//diese Methode zeigt die Adjazenzmatrix des Graphen an
void Solver::printMatrix() {
	cout << "DEBUG: Matrix\n";
	for (int i = 0; i < n; i++)
		cout << "DEBUG: " << matrix[i] << "\n";
	cout << "\n";
}

//diese Methode zeigt die möglichen Zuweisungen der Indizes
//	der jeweiligen Obstsorte an
void Solver::printPossibleAssignments() {
	cout << "DEBUG: Possible assignments\n";
	for (auto x: ID2Fruit) {
		cout << "DEBUG: " << x.second << "\t";
		for (int i = matrix[x.first]._Find_next(-1); i < MAXN; i = matrix[x.first]._Find_next(i))
			cout << i + 1 << " ";
		cout << "\n"; 
	}	
	cout << "\n";
}



 