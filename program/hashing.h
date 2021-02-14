#pragma once

#ifndef HASHING_H
#define HASHING_H

#include<bits/stdc++.h>

#define pb push_back

using namespace std;

class Hashing {

protected:
	//die maximale Anzahl an Obstsorten
	static const int MAXN = 26;

	//n -- die Anzahl an Obstsorten
	//m -- die Anzahl der Spießkombinationen
	int n, m;

	//die Spießkombinationen als < die Menge der Obstorten > 
	//	und < die Menge der Indizes >
	vector< pair<set<int>, set<int>> > infos;

	//die Liste der im Programm verwendeten Indizes
	//	zur Darstellung der Obstsorten und der Indizes der Obstsorten
	vector<int> used;

	//die Menge der gewünschten Obstsorten
	set<int> wishes;

	//die Hashtabelle für jeden Obstnamen und seinen internen Index
	vector<pair<string, int>> hashtable;

	//die Liste, in der an jeder Stelle i der Obstname mit dem
	//	internen Index i steht
	vector<string> ID2Fruit;

	//diese Methode ermittelt die  Stelle in der Hashtabelle 
	//	für einen String 
	int hash(string s);

	//diese Methode speichert einen String in der Hashtabelle
	void saveHash(string s);

	//diese Methode gibt die Stelle eines Strings in der
	//	Hashtabelle zurück
	int getHash(string s);

	//diese Methode gibt den internen Index einer Obstsorte zurück
	int getIndex(string s);

public:
	Hashing(void) {}
	~Hashing(void){}	

	//diese Methode liest die Informationen aus der Datei ein 
	void readFile(string path);

};

#endif