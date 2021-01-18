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

		//Anzahl der Informationen / TODO: change name <Informationen>
		file >> m;
		getline(file, data);

		vector<pair<set<int>, vector<string>>> tempInfos;

		for (int i = 0; i < m; i++) {
			getline(file, data);
			istringstream isss(data);
			vector<string> words{istream_iterator<string>{isss}, istream_iterator<string>{}};
			set<int> currNum;
			for (auto x: words) currNum.insert(stoi(x));

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
			it++;
		}

		file.close();

		for (auto x: wishes_words) wishes.insert(fruit2ID.find(x)->second);

		for (auto x: tempInfos) {
			set<int> currFruits;
			for (auto y: x.second) currFruits.insert(fruit2ID.find(y)->second);
			infos.pb({x.first, currFruits});
		}

		printFruits();
		printInfos();
		printWishes();
	} 
	else {
		cerr << "Error: File could not be opened. Abort.\n";
	}
}

void Solver::analyzeInfo(pair<set<int>, set<int>> info) {
	set<int> nums = info.first, fruits = info.second;

	bitset<MAXN> mask_nums = 0, mask_full = 0, mask_rev = 0;

	//eine Bitmaske mit 1-en auf n Stellen
	for (int i = 0; i < n; i++) mask_full ^= (1 << i);

	//die Bitmaske fuer die Obst aus der Menge
	for (auto x: nums) mask_nums ^= (1 << x-1);

	//die Bitmaske fuer die Obst nicht aus der Menge
	mask_rev = ~(mask_nums) & mask_full;

	auto it = fruits.begin();

	for (int i = 0; i < n; i++) {
		if (*it == i) {
			matrix[i] &= mask_nums;
			//cout << 1 << matrix[i] << "\n";
			it++;
		}
		else {
			matrix[i] &= mask_rev;
			//cout << 2 << matrix[i] << "\n";
		}
	}
	//cout << "\n";
}

void Solver::printFruits() {
	for (auto x: fruit2ID)
		cout << x.first << "   " << x.second << "\n";	
}

void Solver::printInfo(int num) {
	auto x = infos[num];
	for (auto y: x.first) cout << y << " ";
	cout << "\n";
	for (auto y: x.second) cout << ID2Fruit.find(y)->second << " ";
	cout << "\n";	
} 

void Solver::printWishes() {
	//for (auto x: wishes) cout << x << " ";
	cout << "Wishes: ";
	for (auto x: wishes) cout << ID2Fruit.find(x)->second << ", ";
	cout << "\n";
}

void Solver::printInfos() {
	for (int i = 0; i < int(infos.size()); i++) printInfo(i);
}

void Solver::analyzeAllInfos(){
	for (auto x: infos)
		analyzeInfo(x);
//		analyzeInfo(infos[1]);

	for (int i = 0; i < n; i++) {
		cout << matrix[i] << "\n";
	}

	for (auto x: ID2Fruit) {
		cout << matrix[x.first].count()/*-MAXN+n*/ << " "<< x.second << "\t";
		for (int i = 0; i < n; i++)
			if (matrix[x.first][i]) cout << i+1 << " ";
		cout << "\n"; 
	}

	/*for (int i = 0; i < n; i++) {
		cout << ID2Fruit.find(i)->second << "\t" << matrix[i].count()-MAXN+n << "\n";
	}*/
}

bool Solver::checkResult(){
	int count = 0;
	set<int> multip;
	for(auto x: wishes) {
		if (matrix[x].count() == 1) {
			for (int i = 0; i < n; i++)
				if (matrix[x].test(i))
					result.insert(i+1);
		} 
		else {
			count++;
			multip.insert(x);
		}
	} 

	if (count == 0)
		return true;

	cout << "Multiple possible: " << count << "\n";
	for (auto x: multip) cout << ID2Fruit.find(x)->second << " ";
	cout << "\n";

	set<int> ver;

	for (auto x: multip) {
		ver.insert(x);
		for (int i = matrix[x]._Find_next(-1); i < MAXN; i = matrix[x]._Find_next(i))
			ver.insert(count + i+1);
	}

	Graph G(2*n);

	for (auto x: ver)
		cout << x << " ";
	cout << "\n";

	return false;
	return true;
}


set<int> Solver::getResult(){
	return result; 
}