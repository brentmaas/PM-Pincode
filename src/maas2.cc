/**
 * Auteur: Brent Maas (s1826247)
 * Bestand: maas2.cc
 * Functie: (De)Coderen van bestanden met behulp van pincodes, het controleren
 * op Lychrelgetallen in de ongecodeerde versie, en het eventueel herstellen
 * van pincodes
 * Getest op: Windows 10 + MSys2 (MinGW64) met g++ 8.2.0
 * Getest met: g++ -o Assignment1 maas1.cc -std=c++17 -Wall -Wextra
 * Laatst bewerkt: 10/10/2018
 */

#include <iostream>
#include <fstream>
#include <string>
#include <climits>

//Voert het infoblok uit naar de standaarduitvoer
void infoBlokje(){
	std::cout << "/////////////////// INFOBLOK ///////////////////\n"
	"Naam: Brent Maas (s1826247)\n"
	"Jaar van aankomst: 2016\n"
	"Studierichting: Natuur- & Sterrenkunde + minor Data Science\n"
	"Studentnummer: s1826247\n"
	"Opgave: Pincode\n"
	"\n"
	"Dit programma kan gegeven bestanden coderen met behulp van een pincode\n"
	"en deze bestanden op vergelijkbare wijze decoderen. Als de pincode\n"
	"onbekend of verloren is, kan het programma een poging doen deze te\n"
	"herstellen.\n"
	"Laatst bewerkt: 10/10/2018\n"
	"////////////////////////////////////////////////\n" << std::endl;
}

//Machtfunctie voor integers voor positieve exponent
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

//Draai een integer om: bijv. 123 wordt 321
int reverse(int getal){
	int order = 10; //MAX_INT is van orde 10, hoger dus niet mogelijk
	while(getal % pow(10, order) == getal) order--;
	int res = 0;
	for(int i = 0;i < order + 1;i++) res += intAt(getal, order-i) * pow(10, i);
	return res;
}

//Berekent het aantal iteraties voordat het gegeven getal een palindroom is
//(een Lychrelgetal) en vertelt dit aan de standaarduitvoer. In het geval dat
//dit te lang duurt (boven INT_MAX uitkomt), wordt dit als potentieel
//Lychrelgetal gegeven met de iteratie waar het fout gaat
void lychrel(int getal){
	int buffer = getal, rev, i;
	//Zolang het omgedraaide getal groter is dan nul (kleiner dan nul betekent
	//mogelijk een integer overflow), de buffer groter is dan nul (idem), en
	//de twee samen niet groter zijn dan INT_MAX
	for(i = 0;rev >= 0 && buffer >= 0 && getal <= INT_MAX - rev;i++){
		rev = reverse(buffer);
		if(buffer == rev){ //Het getal is nu een palidroom, melden en stoppen
			std::cout << getal << " is een Lychrel-getal na " << i <<
					" iteraties" << std::endl;
			return;
		}
		buffer += rev;
	}
	//Vanaf hier is buffer + rev groter dan INT_MAX, omdat ze dat opgeteld
	//simpelweg zijn, of omdat één van de twee is geoverflowd
	std::cout << getal << " is een potentieel Lychrel-getal na " << i <<
			" iteraties (INT_MAX bereikt)" << std::endl;
}

//Controleer of een char een getal is
bool isGetal(char c){
	return c >= '0' && c <= '9';
}

//Geeft de gecodeerde versie van karakter, als karakter het index-de karakter
//in de regel is met de gegeven pincode
char codeerEnkel(char karakter, int pincode, int index){
	return (karakter + intAt(pincode, 3 - (index % 4))) % 256;
}

//De hoofdfunctie voor het coderen
int coderen(const char* invoerBestand, const char* uitvoerBestand){
	//Vraag om een pincode
	int pincode;
	std::cout << "Pincode: ";
	std::cin >> pincode;
	if(pincode < 0 || pincode > 9999){
		std::cout << "Ongeldige pincode: " << pincode << std::endl;
		return 1;
	}
	
	//Open de filestreams voor het invoer- en uitvoerbestand en kijk of deze
	//goed geopend zijn
	std::fstream invoer(invoerBestand, std::ios_base::in);	
	if(!invoer.is_open()){
		std::cout << "Kon invoerbestand niet openen: " << invoerBestand <<
				std::endl;
		return 1;
	}
	std::fstream uitvoer(uitvoerBestand, std::ios_base::out);
	if(!uitvoer.is_open()){
		std::cout << "Kon uitvoerbestand niet openen: " << uitvoerBestand <<
				std::endl;
		return 1;
	}
	
	int code = pincode, regels = 0, karakters = 0, index = 0, numBuf = 0;
	while(true){
		char current = invoer.get(); //Lees karakter
		//Stop als het laatste karakter het einde van het bestand aangaf
		if(invoer.eof()) break;
		//Bij een nieuwe regel, schrijf een nieuwe regel, reset de pincodeindex
		//en tel een extra regel
		if(current == '\n'){
			uitvoer << current;
			index = 0;
			regels++;
		}else{ //Anders schrijf een gecodeerde regel en verhoog de index met 1
			uitvoer << codeerEnkel(current, code, index);
			index++;
		}
		//Wanneer een getal is gevonden, vermenigvuldig de nummerbuffer met 10
		//en tel het gevonden getal er bij op
		//0 is op waarde 48 in de asciitabel
		if(isGetal(current)) numBuf = numBuf * 10 + (current - 48);
		//Anders als de buffer ongelijk aan nul is, is een getal beeindigd en
		//wordt deze gecontroleerd
		else if(numBuf != 0){
			lychrel(numBuf); //Controleer getal
			//Als het getal een valide pincode is, neem deze als nieuwe pincode
			//en reset de pincodeindex
			if(numBuf > 0 && numBuf < 10000){
				code = numBuf;
				index = 0;
			}
			numBuf = 0; //Reset nummerbuffer
		}
		
		karakters++; //Tel extra karakter op
	}
	
	//Voer het aantal regels en karakters uit naar de standaarduitvoer
	std::cout << "Het invoerbestand had " << karakters << " karakters in " <<
			regels << " regels." << std::endl;
	
	//Sluit de filestreams
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

//De hoofdfunctie voor het decoderen
int decoderen(const char* invoerBestand, const char* uitvoerBestand){
	//Vraag om een pincode
	int pincode;
	std::cout << "Pincode (mocht u deze vergeten zijn, voer \'-1\' in): ";
	std::cin >> pincode;
	if(pincode < -1 || pincode > 9999){
		std::cout << "Ongeldige pincode: " << pincode << std::endl;
		return 1;
	}
	
	if(pincode >= 0){ //Normaal decoderen
		//Open de filestreams voor het invoer- en uitvoerbestand en kijk of
		//deze goed geopend zijn
		std::fstream invoer(invoerBestand, std::ios_base::in);
		if(!invoer.is_open()){
			std::cout << "Kon invoerbestand niet openen: " << invoerBestand <<
					std::endl;
			return 1;
		}
		std::fstream uitvoer(uitvoerBestand, std::ios_base::out);
		if(!uitvoer.is_open()){
			std::cout << "Kon uitvoerbestand niet openen: " << uitvoerBestand <<
					std::endl;
			return 1;
		}
		
		int code = pincode, regels = 0, karakters = 0, index = 0, numBuf = 0;
		while(true){
			char current = invoer.get(); //Lees karakter
			//Stop als het laatste karakter het einde van het bestand aangaf
			if(invoer.eof()) break;
			//Bereken het gedecodeerde karakter
			char currentDec = decodeerEnkel(current, code, index);
			//Bij een nieuwe regel, schrijf een nieuwe regel, reset de
			//pincodeindex en tel een extra regel
			if(current == '\n'){
				uitvoer << current;
				index = 0;
				regels++;
			//Anders schrijf een gedecodeerde regel en verhoog de index met 1
			}else{
				uitvoer << currentDec;
				index++;
			}
			//Wanneer een getal is gevonden, vermenigvuldig de nummerbuffer met
			//10 en tel het gevonden getal er bij op
			//0 is op waarde 48 in de asciitabel
			if(isGetal(currentDec)) numBuf = 10 * numBuf + (currentDec - 48);
			//Anders als de buffer ongelijk aan nul is, is een getal beeindigd
			else if(numBuf != 0){
				//Als het getal een valide pincode is, neem deze als nieuwe
				//pincode en reset de pincodeindex
				if(numBuf > 0 && numBuf < 10000){
					code = numBuf;
					index = 0;
				}
				numBuf = 0; //Reset nummerbuffer
			}
			
			karakters++; //Tel extra karakter op
		}
		
		//Sluit de filestreams
		invoer.close();
		uitvoer.close();
	}else{ //Pincode herstellen
		int besteCode = -1, besteScore = -1, regels = 0, karakters = 0;
		for(int pincode = 0;pincode < 10000;pincode++){
			//Open de filestream voor het invoerbestand en kijk of deze goed
			//geopend is
			std::fstream invoer(invoerBestand, std::ios_base::in);
			if(!invoer.is_open()){
				std::cout << "Kon invoerbestand niet openen: " << invoerBestand
						<< std::endl;
				return 1;
			}
			
			int code = pincode, index = 0, numBuf = 0, score = 0;
			//Waarde om bij de houden hoeveel van "the" is gevonden
			//0: "", 1: "t", 2: "th"
			int the = 0;
			while(true){
				char current = invoer.get(); //Lees karakter
				//Stop als het laatste karakter het einde van het bestand
				//aangaf
				if(invoer.eof()) break;
				//Bereken het gedecodeerde karakter
				char currentDec = decodeerEnkel(current, code, index);
				//Bij een nieuwe regel, reset de pincodeindex
				if(current == '\n'){
					index = 0;
					//Tel een regel bij de eerste iteratie
					if(pincode == 0) regels++;
				}else{
					//Controleer of het gedecodeerde karakter de volgende
					//letter van "the" is, behalve de "e"
					if((the == 0 && (currentDec == 't' || currentDec == 'T'))
							|| (the == 1 && (currentDec == 'h'
							||currentDec == 'H'))) the++;
					//Als het gedecodeerde karakter de "e" is, en dit is de
					//volgende letter van "the", verhoog de score van de
					//huidige pincode en reset de the-counter.
					else if(the == 2 && (currentDec == 'e' ||
							currentDec == 'E')){
						score++;
						the = 0;
					//Anders was het gedecodeerde karakter niet de volgende
					//letter van "the" en wordt de counter ook gereset
					}else{
						the = 0;
					}
					index++; //Verhoog de pincodeindex
				}
				//Wanneer een getal is gevonden, vermenigvuldig de nummerbuffer
				//met 10 en tel het gevonden getal er bij op
				//0 is op waarde 48 op de asciitabel
				if(isGetal(currentDec)) numBuf = 10 * numBuf + (currentDec-48);
				//Anders als de buffer ongelijk aan nul is, is een getal
				//beeindigd
				else if(numBuf != 0){
					//Als het getal een valide pincode is, neem deze als nieuwe
					//pincode en reset de pincodeindex
					if(numBuf > 0 && numBuf < 10000){
						code = numBuf;
						index = 0;
					}
					numBuf = 0; //Reset de nummerbuffer
				}
				
				if(pincode == 0) karakters++; //Tellen is maar eenmaal nodig
			}
			//Als de score van deze pincode beter is dan de beste score tot nu
			//toe, is de huidige pincode met zijn score de nieuwe beste pincode
			if(score > besteScore){
				besteScore = score;
				besteCode = pincode;
			}
			
			//Sluit de filestream
			invoer.close();
		}
		
		//Voer de beste pincode uit naar de standaarduitvoer
		std::cout << "De pincode voor het bestand was " << besteCode <<
				std::endl;
	}
	
	return 0;
}

int main(){
	//Print het infoblok
	infoBlokje();
	
	//Vraag of de gebruiker wil coderen of decoderen
	std::cout << "Wilt u een bestand [C]oderen of [D]ecoderen? ";
	char keuze;
	std::cin >> keuze;
	if(keuze != 'c' && keuze != 'C' && keuze != 'd' && keuze != 'D'){
		std::cout << "Ongeldige keuze: " << keuze << std::endl;
		return 1;
	}
	
	//Vraag naar de invoer- en uitvoerbestanden
	std::string invoerFile, uitvoerFile;
	std::cout << "Invoerbestand:" << std::endl;
	std::cin >> invoerFile;
	std::cout << "Uitvoerbestand:" << std::endl;
	std::cin >> uitvoerFile;
	
	//Als de gebruiker wilde coderen, codeer, anders decodeer
	if(keuze == 'c' || keuze == 'C')
		return coderen(invoerFile.c_str(), uitvoerFile.c_str());
	return decoderen(invoerFile.c_str(), uitvoerFile.c_str());
}