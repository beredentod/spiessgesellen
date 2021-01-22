#include <iostream>
#include "solver.h"


using namespace std;

int main() {
	int n;
	cout << "Nummer des Beispiels: ";
	cin >> n;

	Solver S("../beispiele/spiesse" + to_string(n) + ".txt");
	S.analyzeAllInfos();
 	bool coherent = S.checkCoherence();
	
 	if (coherent) {
		bool solv = S.checkResult();
		S.printWishes();
		cout << "Lösbar: " << boolalpha << solv << "\n";
		if (solv) {
			set<int> result = S.getResult();

			cout << "Ergebnis:\n";
			for (auto x: result)
				cout << x << " ";
			cout << "\n";		
		}
 	}
	else {
		cerr << "Error: Es gibt Fehler in der Eingabedatei\n";
		return 0;
	}

}