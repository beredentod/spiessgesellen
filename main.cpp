#include <iostream>
#include "solver.h"


using namespace std;

int main() {
	int n;
	cout << "Nummer des Beispiels: ";
	cin >> n;

	Solver S(n);
	S.analyzeAllInfos();
	bool solv = S.checkResult();

	cout << "Solvable: " << boolalpha << solv << "\n";
	if (solv) {
		set<int> result = S.getResult();

		cout << "Result:\n";
		for (auto x: result)
			cout << x << " ";
		cout << "\n";		
	}
}