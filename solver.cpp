#include "solver.h"

//Einlesen der Daten aus der Textdatei
void Solver::readFile(string path) {
	fstream file;
	file.open(path, ios::in);
	if (file.is_open()) {

		//Anzahl der Obststuecke
		file >> n;

		//Generierung der Matrix
		for (int i = 0; i < n + 1; i++) {
			bitset<MAXN> b = bitset<MAXN>().set();
			matrix.pb(b);
		}

		string data;
		getline(file, data);
		getline(file, data);
		istringstream iss(data);
		vector<string> wishes_words{istream_iterator<string>{iss}, istream_iterator<string>{}};
		set<string> all_fruits(wishes_words.begin(), wishes_words.end());

		used = vector<int>(n + n);

		//Anzahl der Informationen / TODO: change name <Informationen>
		file >> m;
		getline(file, data);

		vector<pair<set<int>, vector<string>>> tempInfos;

		for (int i = 0; i < m; i++) {
			getline(file, data);
			istringstream isss(data);
			vector<string> words{istream_iterator<string>{isss}, istream_iterator<string>{}};
			set<int> currNum;
			for (auto x: words) {
				currNum.insert(stoi(x));
				used[stoi(x)+n-1] = true;
			}

			getline(file, data);
			istringstream issss(data);
			vector<string> currFruits{istream_iterator<string>{issss}, istream_iterator<string>{}};

			for (auto x: currFruits) all_fruits.insert(x);
			tempInfos.pb({currNum, currFruits});
		}

		//Zuweisung der Indizes fuer jedes Obst
		int it = 0;
		for (auto x: all_fruits) {
			ID2Fruit[it] = x;
			fruit2ID[x] = it;
			used[it] = true;
			it++;
		}

		file.close();

		for (auto x: wishes_words) wishes.insert(fruit2ID.find(x)->second);

		for (auto x: tempInfos) {
			set<int> currFruits;
			for (auto y: x.second) currFruits.insert(fruit2ID.find(y)->second);
			infos.pb({x.first, currFruits});
		}

		//printFruits();
		//printInfos();
	} 
	else {
		cerr << "Error: File could not be opened. Abort.\n";
	}
}

void Solver::analyzeInfo(pair<set<int>, set<int>> info) {
	set<int> nums = info.first, fruits = info.second;

	bitset<MAXN> mask_nums = 0, mask_full = 0, mask_rev = 0;

	//eine Bitmaske mit 1-en auf allen n Stellen
	for (int i = 0; i < n; i++) mask_full ^= (1 << i);

	//die Bitmaske fuer die Obst aus der Menge
	for (auto x: nums) mask_nums ^= (1 << x-1);

	//die Bitmaske fuer die Obst nicht aus der Menge
	mask_rev = ~(mask_nums) & mask_full;

	auto it = fruits.begin();

	//cout << mask_nums << " \n" << mask_rev << " \n\n";

	for (int i = 0; i < n; i++) {
		if (*it == i && it != fruits.end()) {
			matrix[i] &= mask_nums;
			//cout << 1 << " " << ID2Fruit.find(i)->second << " " << matrix[i] << "\n";
			it++;
		}
		else {
			matrix[i] &= mask_rev;
			//cout << 2 << " " << ID2Fruit.find(i)->second << " " << matrix[i] << "\n";
		}
	}
	//cout << "\n";
}

void Solver::analyzeAllInfos(){
	for (auto x: infos)
		analyzeInfo(x);

	for (auto x: ID2Fruit) {
		for (int i = matrix[x.first]._Find_next(-1); i < MAXN; i = matrix[x.first]._Find_next(i))
			G.addEdge(x.first, i);
	}

	//printMatrix();
	//printPossibleAssignments();
	//G.printGraph();
}

bool Solver::checkCoherence(){ 
	int notusedA = 0, notusedB = 0;
	for (int i = 0; i < n; i++)
		if (!used[i]) notusedA++;
	for (int i = 0; i < n; i++)
		if (!used[i]) notusedB++;

	if (notusedA != notusedB)
		return false;

	for (int i = 0; i < int(used.size()); i++)
		if (used[i] && G.deg(i) == 0)
			return false;

	return true;
}

bool Solver::checkResult(){
	int count = 0;
	set<int> multip;
	vector<int> ready(n);
	vector<int> todo(n);
	set<int> temp;

	for (auto x: wishes) {
		if (G.deg(x) == 0) {
			cout << "Der Obstsorte \"" << ID2Fruit.find(x)->second
				<< "\" kann kein Index zugewiesen werden.\n";
			return false;
		}
		else if (G.deg(x) == 1) {
			vector<int> neigh = G.getNeighbors(x);
			result.insert(neigh[0] - n + 1);
		}
		else {
			count++;
			multip.insert(x);
			todo[x] = 1;
		}
	} 

	if (count == 0)
		return true;

	/*cout << "DEBUG: Multiple possible assignments: " << count << "\n";
	for (auto x: multip)
		cout << "\t--> " << ID2Fruit.find(x)->second << "\n";
	cout << "\n";*/

	bool solv = true;
	vector<set<int>> problems;

	for (auto x: multip) {
		bool prob = false;
		if (!ready[x]) {
			vector<int> setB = G.getNeighbors(x);
			vector<int> setA = G.getNeighbors(setB[0]);

			for (auto x: setA) {
				if (!todo[x]) {
					solv = false;
					prob = true;
				}
				else
					ready[x] = 1;
			}

			if (prob) {
				set<int> curr_prob;
				for (auto x: setA)
						curr_prob.insert(x);
				problems.pb(curr_prob);
			}
			else
				for (auto x: setB)
					temp.insert(x - n + 1);
		}
	}
	
	if (solv) {
		for (auto x: temp) 
			result.insert(x);
		return true;
	}
	else {
		cout << "Für die folgenden Obstsorten konnte keine eideutige Zuweisung gefunden werden.\n";
		for (auto x: problems) {
			cout << "Komponente: ";
			for (auto y: x)
				cout << ID2Fruit.find(y)->second << " ";
			cout << "\n\t--> Nicht auf der Wunschliste: ";
			for (auto y: x) 
				if (!todo[y])
					cout << ID2Fruit.find(y)->second << " ";
			cout << "\n";
		}
		return false;
	}
}

set<int> Solver::getResult(){
	return result; 
}

void Solver::printWishes() {
	cout << "Wünsche: ";
	for (auto x: wishes)
		cout << ID2Fruit.find(x)->second << " ";
	cout << "\n";
}

void Solver::printInfos() {
	cout << "DEBUG: All infos about the sets\n";
	for (int i = 0; i < int(infos.size()); i++)
		printInfo(i);
	cout << "\n";
}

void Solver::printMatrix() {
	cout << "DEBUG: Matrix\n";
	for (int i = 0; i < n; i++)
		cout << "DEBUG: " << matrix[i] << "\n";
	cout << "\n";
}

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

void Solver::printFruits() {
	cout << "DEBUG: IDs of Fruits\n";
	for (auto x: fruit2ID)
		cout << "DEBUG: " << x.first << "   " << x.second << "\n";	
	cout << "\n";
}

void Solver::printInfo(int num) {
	auto x = infos[num];
	for (auto y: x.first) cout << y << " ";
	cout << "\n";
	for (auto y: x.second) cout << ID2Fruit.find(y)->second << " ";
	cout << "\n";	
} 