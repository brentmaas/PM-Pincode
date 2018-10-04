/**
 * Auteur: Brent Maas (s1826247)
 * Bestand: maas2.cc
 * Functie: 
 * Getest op: Windows 10 + MSys2 (MinGW64) met g++ 7.3.0
 * Getest met: g++ -o Assignment1 maas1.cc -std=c++17 -Wall -Wextra
 * Laatst bewerkt: 04/10/2018
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

//Geeft de gecodeerde versie van karakter, als karakter het index-de karakter in het
//bestand is met de gegeven pincode
char codeerEnkel(char karakter, int pincode, int index){
	return (karakter + intAt(pincode, 3 - (index % 4))) % 256;
}

int coderen(const char* invoerBestand, const char* uitvoerBestand, const int pincode){
	std::fstream invoer(invoerBestand, std::ios_base::in);	
	if(!invoer.is_open()){
		std::cout << "Kon invoerbestand niet openen: " << invoerBestand << std::endl;
		return 1;
	}
	std::fstream uitvoer(uitvoerBestand, std::ios_base::out);
	if(!uitvoer.is_open()){
		std::cout << "Kon uitvoerbestand niet openen: " << uitvoerBestand << std::endl;
		return 1;
	}
	
	char current;
	int code = pincode, regels = 0, karakters = 0, index = 0, numBuf = 0;
	while(true){
		current = invoer.get();
		if(invoer.eof()) break;
		if(current == '\n'){
			//std::cout << current;
			uitvoer << current;
			index = 0;
			regels++;
		}else{
			//std::cout << codeerEnkel(current, code, index);
			uitvoer << codeerEnkel(current, code, index);
			index++;
		}
		//0 is op waarde 48 in de asciitabel
		if(current >= '0' && current <= '9') numBuf = numBuf * 10 + (current - 48);
		else if(numBuf != 0){
			lychrel(numBuf);
			if(numBuf > 0 && numBuf < 10000){
				code = numBuf;
				index = 0;
			}
			numBuf = 0;
		}
		
		karakters++;
	}
	
	std::cout << "Het invoerbestand had " << karakters << " karakters in " << regels << " regels." << std::endl;
	
	std::cout << std::endl;
	
	invoer.close();
	uitvoer.close();
	
	return 0;
}

//Geeft de gedecodeerde versie van karakter, als karakter het index-de karakter in het
//bestand is met de gegeven pincode
char decodeerEnkel(char karakter, int pincode, int index){
	//De +256 omdat -1 % 256 = -1 en dat is geen geldige asciikarakter
	return (karakter - intAt(pincode, 3 - (index % 4)) + 256) % 256;
}

int decoderen(const char* invoerBestand, const char* uitvoerBestand, const int pincode){
	
	
	return 0;
}

int main(){
	infoBlokje();
	
	std::cout << "Wilt u een bestand [C]oderen of [D]ecoderen? ";
	char keuze;
	std::cin >> keuze;
	if(keuze != 'c' && keuze != 'C' && keuze != 'd' && keuze != 'D'){
		std::cout << "Ongeldige keuze: " << keuze << std::endl;
		return 1;
	}
	
	std::string invoerFile, uitvoerFile;
	int pincode;
	std::cout << "Invoerbestand: ";
	std::cin >> invoerFile;
	std::cout << "Uitvoerbestand: ";
	std::cin >> uitvoerFile;
	std::cout << "Pincode: ";
	std::cin >> pincode;
	if(pincode < -1 && pincode > 9999){
		std::cout << "Ongeldige pincode: " << pincode << std::endl;
		return 1;
	}
	
	if(keuze == 'c' || keuze == 'C') return coderen(invoerFile.c_str(), uitvoerFile.c_str(), pincode);
	return decoderen(invoerFile.c_str(), uitvoerFile.c_str(), pincode);
}