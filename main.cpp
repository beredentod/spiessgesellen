#include <iostream>
#include "solver.h"


using namespace std;

int main() {
	//die Nummer des Beispiels
	int n;
	cout << "Nummer des Beispiels: ";
	cin >> n;

	//eine Lösung wird zu einer Textdatei mit Eingabe gefunden 
	Solver S("../beispiele/spiesse" + to_string(n) + ".txt");

	//es werden Informationen aus allen Spießkombinationen bearbeitet
	S.analyzeAllInfos();

	//es wird geprüft, ob die Eingabe keinen Widerspruch ergibt
 	bool coherent = S.checkCoherence();
	
	//falls die Eingabe korrekt ist
 	if (coherent) {
 		//es wird geprüft, ob eine eindeutige Lösung für die Eingabe existiert
		bool solv = S.checkResult();

		//die Menge der gewünschten Obstsorten
		S.printWishes();

		cout << "Lösbar: " << boolalpha << solv << "\n";
		if (solv) {
			//die Menge der Indizes der gewünschten Obstsorten
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