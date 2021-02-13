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

		//die gewuenschten Obstsorten werden erstmal als Strings gespeichert
		vector<string> wishes_words{istream_iterator<string>{iss}, 
			istream_iterator<string>{}};

		//die Menge der allen Obstsorten als Strings
		set<string> all_fruits(wishes_words.begin(), wishes_words.end());

		//Anzahl der Spiesskombinationen
		file >> m;
		getline(file, data);

		//Spiesskombinationen werden erstmal als ein Menge von Zahlen und 
		//	eine Menge von Strings gespeichert
		vector<pair<set<int>, vector<string>>> tempInfos;

		for (int i = 0; i < m; i++) {
			getline(file, data);
			istringstream isss(data);
			vector<string> words{istream_iterator<string>{isss}, istream_iterator<string>{}};

			//die Menge der Indizes der jeweiligen Spiesskombination wird
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

			//die Menge der Obstsorten der jeweiligen Spiesskombination wird
			//	erstmal als Strings erstellt
			vector<string> currFruits{istream_iterator<string>{issss},
				istream_iterator<string>{}};

			//alle Obstsorten werden zu einer gemeinsamen Menge hizugefuegt
			for (auto x: currFruits)
				all_fruits.insert(x);

			//falls die beiden Menge einer Spiesskombination nicht gleichmaechtig sind
			if (currNum.size() != currFruits.size()) {
				cerr << "Error: Es gibt Fehler in der Eingabedatei.\n";
				exit(0);
			}

			//eine Spiesskombination als ein Menge von Zahlen und 
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

		//die gewuenschten Obstsorten werden als Indizes gespeichert
		for (auto x: wishes_words)
			wishes.insert(fruit2ID.find(x)->second);

		//die Spiesskombinationen als Mengen der Indizes der Obstsorten
		//	einer Spiesskombination und Mengen der internen Indizes der Obstsorten
		for (auto x: tempInfos) {
			set<int> currFruits;
			for (auto y: x.second)
				currFruits.insert(fruit2ID.find(y)->second);
			infos.pb({x.first, currFruits});
		}
	} 
	//falls die Textdatei nicht geoeffnet werden kann
	else {
		cerr << "Error: File could not be opened. Abort.\n";
		exit(0);
	}
}

//diese Methode bearbeitet die Informationen aus einer Spiesskombination
void Solver::analyzeInfo(pair<set<int>, set<int>> info) {
	//fruits -- die Menge der Obstsorten der Spiesskombination
	//nums -- die Menge der Indizes der Obstsorten der Spiesskombination
	set<int> nums = info.first, fruits = info.second;

	bitset<MAXN> mask_nums = 0, mask_full = 0, mask_rev = 0;

	//eine Bitmaske mit 1-en auf allen n Stellen
	for (int i = 0; i < n; i++) mask_full ^= (1 << i);

	//die Bitmaske fuer die Obstsorten aus der Menge
	for (auto x: nums) mask_nums ^= (1 << (x-1));

	//die Bitmaske fuer die Obstsorten ausserhalb der Menge
	mask_rev = ~(mask_nums) & mask_full;
	
	//es wird ueber die Menge der Obstsorten der Spiesskombination
	//	iteriert
	auto it = fruits.begin();

	//es wird ueber die Obstsorten in der Adjazenzmatrix iteriert
	for (int i = 0; i < n; i++) {
		//falls eine Obstsorte zur Menge der Obstsorten der 
		//	Spiesskombination gehoert
		if (it != fruits.end() && *it == i) {
			matrix[i] &= mask_nums;
			it++;
		}
		//falls eine Obstsorte ausserhalb der Menge der Obstsorten
		//	der Spiesskombination liegt
		else
			matrix[i] &= mask_rev;
	}
}

//diese Methode bearbeitet alle Informationen aus allen Spiesskombinationen
void Solver::analyzeAllInfos(){
	//alle Spiesskombinationen werden bearbeitet
	for (auto x: infos)
		analyzeInfo(x);

	//alle noch uebrigen Kanten werden in den Graphen hinzugefuegt
	for (auto x: ID2Fruit) {
		for (int i = matrix[x.first]._Find_next(-1); i < MAXN;
		   i = matrix[x.first]._Find_next(i))
			G.addEdge(x.first, i);
	}
}

//diese Methode prueft, ob die eingegeben Informationen ueber den Graphen
//	keinen Widerspruch ergeben
bool Solver::checkCoherence(){ 
	//falls zwei im Programm verwendete Knoten auf einer Komponente
	//	unterschiedliche Kardinalitaeten haben, ist die Eingabe falsch
	for (int i = 0; i < int(used.size()); i++)
		if (used[i]){
			//falls ein Knoten die Kardinalitaet von 0 besitzt
			if (G.deg(i) == 0)
				return false;

			//wir nehmen den ersten Nachbarn von i 
			int neigh = G.getFirstNeighbor(i);

			//wir vergleichen die Kardinalitaeten
			if (G.deg(neigh) != G.deg(i))
				return false;
		}

	return true;
}

//diese Methode prueft, ob eine eindeutige Loesung exisitiert
bool Solver::checkResult(){
	//die Menge der Obstsorten (Knoten), die einen Grad von
	//	mehr als 1 haben
	set<int> multip;

	//die Liste der Knoten, deren Komponente geprueft werden muss
	vector<int> ready(n);

	//die Liste der Knoten der gewuenschten Obstsorten
	vector<int> todo(n);

	//es wird durch die Menge der Wuensche iteriert
	for (auto x: wishes) {
		//falls der Grad des Knotens der gewuenschten Obstsorte
		// gleich 1 ist
		if (G.deg(x) == 1) {
			int neigh = G.getFirstNeighbor(x);

			//der einzige Nachabr wird in die Ergebnismenge hinzugefuegt
			result.insert(neigh - n + 1);
		}
		//falls der Grad des Knotens der gewuenschten Obstsorte
		// mehr als 1 betraegt
		else {
			//der gewuenschte Knoten wird in die Menge 
			//	fuer die Untersuchung der Komponente hinzugefuegt
			multip.insert(x);

			//die Obstsorte wird in die To-do-Liste 
			// fuer die Untersuchung der Komponente hinzugefuegt
			todo[x] = 1;
		}
	} 

	//falls alle gewuenschten Obstsorten einen Grad von 1 haben
	if (multip.size() == 0)
		return true;

	//diese Variable zeigt an, ob eine Loesung fuer die Eingabe existiert
	bool solv = true;

	//eine Liste mit Mengen der Obstsorten der Komponenten,
	//	zu deren Knoten gehoeren, die nicht auf der Wunschliste stehen 
	vector<set<int>> problems;

	//es wird durch die Menge der Knoten der gewuenschten Obstsorten iteriert,
	// deren Grad groesser als 1 betraegt
	for (auto x: multip) {
		//diese Variable zeigt an, ob mind. ein Knoten zur Komponente
		// gehoert, aber nicht gewuenscht ist
		bool prob = false;

		//falls die Komponente eines Knotens noch nicht geprueft wurde
		if (!ready[x]) {
			//die Menge der Knoten der Indizes der Obstsorten der jeweiliger Komponente
			vector<int> setB = G.getNeighbors(x);
			//die Menge der Knoten der Obstsorten der jeweiliger Komponente
			vector<int> setA = G.getNeighbors(setB[0]);

			//es wir durch die Knoten der Obstsorten dieser Komponente
			//	iteriert
			for (auto y: setA) {
				//falls die Obstsorte x nicht gewuenscht ist,
				//	gibt es keine eindeutige Loesung duer die Eingabe
				if (!todo[y]) {
					solv = false;
					prob = true;
				}
				//falls die Obstsorte x gewuenscht ist,
				//	wird sie als geprueft markiert
				else
					ready[y] = 1;
			}

			//falls mind. ein Knoten zur Komponente gehoert, aber nicht 
			//	gewuenscht ist
			if (prob) {
				//die Menge der Obstsorten der Komponente enthaelt 
				//	mind. einen Knoten einer Obstsorte, die nicht gewuenscht ist
				set<int> currSet;
				for (auto y: setA)
					currSet.insert(y);
				problems.pb(currSet);
			}
			//falls alle Knoten, die zur Komponente gehoeren, gewuenscht sind
			else
				//alle Indizes der Obstsorte der Komponente werden 
				//	in die Menge der Indizes der gewuenschten Obstsorten hinzugefuegt
				for (auto y: setB)
					result.insert(y - n + 1);
		}
	}
	
	//falls eine Loesung fuer die Eingabe existiert
	if (solv) {
		return true;
	}
	//falls es keine eindeutige Loesung fuer die Eingabe gibt
	else {
		cout <<
   "Fuer die folgenden Obstsorten konnte keine eideutige Zuweisung gefunden werden.\n";
		for (auto x: problems) {
			cout << "Komponente: ";
			//Aufzaehlen der Obstsorten, die zur Komponente gehoeren
			for (auto y: x)
				cout << ID2Fruit.find(y)->second << " ";

			//Aufzaehlen der Obstsorten, die zur Komponente gehoeren,
			//	 aber nicht gewuenscht sind
			cout << "\n\t--> Nicht auf der Wunschliste: ";
			for (auto y: x) 
				if (!todo[y])
					cout << ID2Fruit.find(y)->second << " ";
			cout << "\n";
		}
		return false;
	}
}
