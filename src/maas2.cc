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

//Machtfunctie voor integers voor positieve exp
int pow(int base, int exp){
	int res = 1;
	for(int i = 0;i < exp;i++) res *= base;
	return res;
}

//Returnt het getal van i-ste orde van getal
//Bijv: intAt(12345, 3) = 2, intAt(12345, 1) = 4
int intAt(int getal, int i){
	return getal % pow(10, i + 1) / pow(10, i);
}

//Draai een integer om als ware het een string
int reverse(int getal){
	int order = 10;
	while(getal % pow(10, order) == getal) order--;
	int res = 0;
	for(int i = 0;i < order + 1;i++) res += intAt(getal, order - i) * pow(10, i);
	return res;
}

//Returnt het aantal iteraties voordat het gegeven getal een palindroom is.
//In het geval dat dit te lang duurt (boven INT_MAX uitkomt), wordt de negatieve
//waarde van de huidige iteratie - 1 gegeven.
void lychrel(int getal){
	int buffer = getal, rev, i;
	for(i = 0;rev >= 0 && buffer >= 0 && getal <= INT_MAX - rev;i++){
		rev = reverse(buffer);
		if(buffer == rev){
			std::cout << getal << " is een Lychrel-getal na " << i << " iteraties" << std::endl;
			return;
		}
		buffer += rev;
	}
	std::cout << getal << " is een potentieel Lychrel-getal na " << i << " iteraties (INT_MAX bereikt)" << std::endl;
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