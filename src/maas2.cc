/**
 * Auteur: Brent Maas (s1826247)
 * Bestand: maas2.cc
 * Functie: 
 * Getest op: Windows 10 + MSys2 (MinGW64) met g++ 7.3.0
 * Getest met: g++ -o Assignment1 maas1.cc -std=c++17 -Wall -Wextra
 * Laatst bewerkt: 20/09/2018
 */

#include <iostream>
#include <fstream>
#include <string>
#include <climits>

void infoBlokje(){
	std::cout << "/////////////////// INFOBLOK ///////////////////\n"
	"Naam: Brent Maas (s1826247)\n"
	"Jaar van aankomst: 2016\n"
	"Studierichting: Natuur- & Sterrenkunde + minor Data Science\n"
	"Studentnummer: s1826247\n"
	"Opgave: Pincode\n"
	"\n"
	"<functie>\n"
	"Laatst bewerkt: 20/09/2018\n"
	"////////////////////////////////////////////////\n" << std::endl;
}
		
void coderen(){
	
}

void decoderen(){
	
}

int main(){
	infoBlokje();
	
	std::cout << "Wilt u een bestand [C]oderen of [D]ecoderen? ";
	char keuze;
	std::cin >> keuze;
	
	if(keuze == 'c' || keuze == 'C') coderen();
	else if(keuze == 'd' || keuze == 'D') decoderen();
	else{
		std::cout << "Ongeldige keuze: " << keuze << std::endl;
		return 1;
	}
	
	return 0;
}