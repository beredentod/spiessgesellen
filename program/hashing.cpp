#include "hashing.h"

//diese Methode ist eine Hashfunktion, die einem String
//	eine Stelle in der hashtable zuordnet
int Hashing::hash(string s) {
	//die Stelle in der hashtable wird anhand des
	//	ersten Buchstabne ermittelt
	char letter = s[0];
	letter -= 65;

	//falls der erste Buchstabe klein ist
	if (letter > 25)
		letter -= 32;

	//die Stelle wird ermittelt
	return letter % hashtable.size();
}

//diese Methode speichert einen String an der
//	durch die Hashfunktion ermittelte Stelle
void Hashing::saveHash(string s) {
	int pos = hash(s);

	//falls die zugeordnete Stelle nicht besetzt ist
	if (hashtable[pos].first == "") {
		hashtable[pos].first = s;
		return;
	}

	//falls die zugeordnete Stelle bereits besetzt ist,
	//	wird nach der ersten freien Stellen gesucht
	int it = (pos + 1) % hashtable.size();
	while (hashtable[it].first != "") {
		it++;
		if (it == pos)
			return;
	}

	//der String wird an der gefundenen Stelle gespeichert
	hashtable[it].first = s;
}

//diese Methode gibt die Stelle in der hashtable zurück,
//	falls ein String sich in der hashtable bereits befindet.
//	Sonst wird -1 ausgegeben
int Hashing::getHash(string s) {
	int pos = hash(s);

	//falls der gesuchte String sich an der zugeordneten
	//	Stelle befindet
	if (hashtable[pos].first == s)
		return pos;

	//es wird weiteriteriert, um die Stelle mit dem gegebenen String zu finden
	int i = (pos + 1) % hashtable.size();
	for (; i != pos; i = (i + 1) % hashtable.size()) {
		if (hashtable[i].first == "")
			return -1;
		if (hashtable[i].first == s)
			return i;
	}

	return -1;
}

//diese Methode gibt den internen Index einer Obstsorte zurück
int Hashing::getIndex(string s) {
	int pos = getHash(s);
	return hashtable[pos].second;
}

//diese Methode liest Informationen aus der Datei ein 
void Hashing::readFile(string path) {
	fstream file;
	file.open(path, ios::in);
	if (file.is_open()) {

		//Anzahl der Obstsorten
		file >> n;

		//Erstellung der Liste der im Programm verwendeten internen Indizes
		used = vector<int>(n + n);

		//Erstellung der hashtable
		hashtable = vector<pair<string, int>> (MAXN, {"", -1});

		string data;
		getline(file, data);
		getline(file, data);
		istringstream iss(data);

		//die gewünschten Obstsorten werden erstmal als Strings gespeichert
		vector<string> wishes_words{istream_iterator<string>{iss}, istream_iterator<string>{}};

		//in dieser Liste werden alle Obstnamen gespeichert, die in der
		//	Datei auftreten
		vector<string> all_fruits(wishes_words.begin(), wishes_words.end());

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

				//ein Index (aus der Aufgabenstellung) wird
				//	als verwendet merkiert
				used[stoi(x)+n-1] = true;
			}

			getline(file, data);
			istringstream issss(data);

			//die Menge der Obstsorten der jeweiligen Spießkombination wird
			//	erstmal als Strings erstellt
			vector<string> currFruits{istream_iterator<string>{issss}, istream_iterator<string>{}};

			//alle Obstsorten werden zu einer gemeinsamen Liste hizugefügt
			all_fruits.insert(all_fruits.end(), currFruits.begin(), currFruits.end());

			//falls die beiden Menge einer Spießkombination nicht gleichmächtig sind
			if (currNum.size() != currFruits.size()) {
				cerr << "Error: Es gibt Fehler in der Eingabedatei.\n";
				exit(0);
			}

			//eine Spießkombination als ein Menge von Zahlen und 
			//	eine Menge von Strings wird gespeichert
			tempInfos.pb({currNum, currFruits});
		}

		file.close();

		//alle Obstsorten werden durch die Hashfunktion verarbeitet
		for (auto x: all_fruits) {
			int id = getHash(x);
			//falls der Obstname x noch nicht aufgetreten ist
			if (id == -1)
				saveHash(x);
		}

		//Zuweisung der internen Indizes 0..(n-1) jeder Obstsorte
		//	mithilfe einer Hasfunktion
		int it = 0;
		for (int i = 0; i < hashtable.size(); i++) {
			if (hashtable[i].first != "") {
				//ein interner Index wird einer Obstsorte zugeordnet
				hashtable[i].second = it;

				//der Obstname wird in die Liste ID2Fruit hinzugefügt,
				//	dessen Stelle in der Liste dem internen Index
				//	der Obstsorte entspricht
				ID2Fruit.pb(hashtable[i].first);

				//der interne Index einer Obstsorte wird als verwendet merkiert
				used[it++];
			}
		}

		//die gewünschten Obstsorten werden als Indizes gespeichert
		for (auto x: wishes_words)
			wishes.insert(getIndex(x));

		//die Spießkombinationen als Mengen der Indizes der Obstsorten
		//	einer Spießkombination und Mengen der internen Indizes der Obstsorten
		for (auto x: tempInfos) {
			set<int> currFruits;
			for (auto y: x.second)
				currFruits.insert(getIndex(y));
			infos.pb({x.first, currFruits});
		}
	} 
	//falls die Textdatei nicht geöffnet werden kann
	else {
		cerr << "Error: File could not be opened. Abort.\n";
		exit(0);
	}
}