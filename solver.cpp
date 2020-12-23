#include "solver.h"

void Solver::readFile(string path) {
	fstream file;
	file.open(path, ios::in);
	if (file.is_open()) {
		file >> n;
		for (int i = 0; i < n; i++) {
			vector<int> v(n, 0);
			matrix.pb(v);
		}

		string data;
		getline(file, data);
		getline(file, data);
		istringstream iss(data);
		copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(wishes_words));
		set<string> all_fruits(wishes_words.begin(), wishes_words.end());

		file >> m;
		getline(file, data);


		for (int i = 0; i < m; i++) {
			getline(file, data);
			istringstream isss(data);
			vector<string> words{istream_iterator<string>{isss}, istream_iterator<string>{}};
			vector<int> currNum;
			for (auto x: words) currNum.pb(stoi(x));

			getline(file, data);
			istringstream issss(data);
			vector<string> currFruits{istream_iterator<string>{issss}, istream_iterator<string>{}};

			for (auto x: currFruits) all_fruits.insert(x);
			infos.pb({currNum, currFruits});
		}

		int it = 0;
		for (auto x: all_fruits) {
			ID2Fruit[it] = x;
			fruit2ID[x] = it;
			it++;
		}

		file.close();

		printFruits();
		printInfos();
	} 
	else {
		cerr << "Error: File could not be opened. Abort.\n";
	}
}

void Solver::printFruits() {
	for (auto x: fruit2ID)
		cout << x.first << "   " << x.second << "\n";	
}

void Solver::printInfo(int num) {
	auto x = infos[num];
	for (auto y: x.first) cout << y << " ";
	cout << "\n";
	for (auto y: x.second) cout << y << " ";
	cout << "\n";	
} 

void Solver::printInfos() {
	for (int i = 0; i < int(infos.size()); i++) printInfo(i);
}