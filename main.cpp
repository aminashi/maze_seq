#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <time.h>
#include <cstdlib>

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

class Cell{
    public:
    	Cell(int wiersz, int kolumna, int wartosc){
    		this->wiersz = wiersz;
    		this->kolumna = kolumna;
    		this->wartosc = wartosc;
			}
	int get_wiersz(){  	return wiersz;}
	int get_kolumna(){	return kolumna;}
	int get_wartosc(){ 	return wartosc;}
	int set_wiersz(int r){  return wiersz=r;}
	int set_kolumna(int c){	return kolumna=c;}
	int set_wartosc(int w){ return wartosc=w;}
    private:
		int wiersz;
     	int kolumna;
    	int wartosc;
};


void readMaze(vector< vector<char> > &labirynt, int &startX, int &startY){
	
	ifstream file("MAZE.txt"); //otworz plik
	if (file.is_open()){
    vector <char> vec; // wektor pomocniczy potrzebny

    string line;
    char i;
    while (getline( file, line) )
    {
       stringstream ss(line); // Odczytaj linie z  pliku

       while(ss >> i){ // Wyciagnij chary z jednej wczytanej linii
         vec.push_back(i);
         if(i=='0'){ //znalezienie poczatku labiryntu
         	startY=vec.size()-1;
			startX=labirynt.size();	
			cout<< "startY: "<< startY << " startX: " << startX << endl;	
			 }
    }
       labirynt.push_back(vec); //dodanie pomocniczego wektora do labiryntu
       vec.clear(); //wyczyszczenie pomocniczego wektora

    }
}
else cout<<"nie moge otworzyc pliku z labiryntem!"<<endl;
  file.close(); //zamknij plik
}

	//labirynt zawiera:
	//labirynt.size() - ilosc wierszy
	//labirynt[i] is - wiersz i-ty
	//labirynt[i].size() - numer kolumn w wierszu i-tym
	//labirynt[i][j]  - element z j-tej kolumny i i-tego wiersza

void printMaze (vector< vector<char> > &labirynt){
		for(int i=0;i<labirynt.size(); i++) {
	      	for (int j=0;j<labirynt[i].size(); j++){
        	cout << labirynt[i][j] << " "; 
        }
	  cout << endl;
   }
}

void writeMaze (vector< vector<char> > &labirynt, double difference,double differenceLiczenia,double differenceCzytania){
	
	ofstream file("MAZE_SOLVED.txt"); //otworz plik
	if (file.is_open()){
		for(int i=0;i<labirynt.size(); i++) {
	      	for (int j=0;j<labirynt[i].size(); j++){
        	file << labirynt[i][j] << " "; 
        	}
	  	file << endl;
	   }
		file<< "Ca³kowity czas dzia³ania programu: " << difference << " sekundy."<<endl;
		file<< "Ca³kowity czas wykonywania obliczeñ: " << differenceLiczenia << " sekundy."<<endl;
		file<< "Ca³kowity czas odczytywania danych z pliku: " << differenceCzytania << " sekundy."<<endl;
	   }
   else cout<< "nie moge otworzyc pliku do zapisu!"<<endl;

   file.close();

}


int main(int argc, char** argv) {
	
	double difference, differenceLiczenia, differenceCzytania;
	cout.setf(ios::fixed); //notacja typu 0.23432 zamiast 3.14159e+000 
	cout.precision(8); //iloœæ msc po przecinku
	clock_t start, end, startCzasuLiczenia, koniecCzasuLiczenia, startCzasuCzytaniaZPliku, koniecCzasuCzytaniaZPliku; //zmienne zegarowe
	start = clock(); //czas startu

	bool koniec = false;
	int newX;
	int newY;
	
	int index = 0; //zaczynamy od indeksu 0 - punktu poczatkowego
	vector< vector<char> > labirynt; // wektor wektorow - tu bedzie zawartosc calego pliku MAZE.txt

	startCzasuCzytaniaZPliku=clock();	
	readMaze(labirynt,newX,newY); //wczytaj labirynt
	koniecCzasuCzytaniaZPliku=clock();
	
	printMaze(labirynt); //wyswietl labirynt
		
	vector<Cell> lista;
	
	Cell startKom(newX,newY,index);
	Cell komorka=startKom;
	lista.push_back(startKom);

startCzasuLiczenia=clock();	
for(int l=0;l<lista.size();l++){
	if (koniec==true) {
		break;
	}
	//cout<<lista.size()<<" lista\n";
	newX=lista[l].get_wiersz();
	newY=lista[l].get_kolumna();
	index=lista[l].get_wartosc();
	//cout<<lista[l].get_wiersz()<<" "<<lista[l].get_kolumna()<<" "<<lista[l].get_wartosc()<<endl;
	for (int i=0;i<2;i++){
			if (koniec==true){
				break;
			}
			for (int delta = -1; delta < 3; delta+=3){       
                if (i<1){
                	newX = (newX + delta);
                	newY = newY;	
                }
					else{	
						newX = newX + delta%2;
						newY = (newY + delta);			
					}		
				if(newY>=labirynt[newX].size() || newX>=labirynt.size() || (labirynt[newX][newY]!='_' && labirynt[newX][newY]!='S'))//sprawdzenie czy punkt nie wybiega poza zakres,
					continue;
				else if(labirynt[newX][newY] == 'S'){ //jesli dojdziemy do punktu koncowego
					cout<<"Znalazlem droge!"<<endl;
					//printMaze(labirynt);
					koniec=true;
					Cell meta(newX,newY,index+1);
					lista.push_back(meta);
					break;
				}																			   //czy nie jest sciana i czy nie zostal juz "odwiedzony"
                komorka.set_wiersz(newX);
				komorka.set_kolumna(newY);
				komorka.set_wartosc(index+1);
                lista.push_back(komorka); //dodatnie do listy potencjalnie dobrej komorki drogi

				labirynt[newX][newY]=index+1+'0';
				
			//	cout<<komorka.get_wiersz()<<" i ";
            //    cout<<komorka.get_kolumna()<<" i ";
            //    cout<<komorka.get_wartosc()<<"\n";
            //    printMaze(labirynt);
		  	}
		}
	}

	int r=newX;
	int c=newY;
	int w=index+1;
	
	for(int i=lista.size()-2;i>0;i--){ //odczytanie z listy najkrostszej drogi
				
		int tempr=lista[i].get_wiersz();
		int tempc=lista[i].get_kolumna();
		int tempw=lista[i].get_wartosc();
		//cout<<lista.size()<<" lista\n";
		//cout<<r<<" "<<c<<" "<<w<<endl;
		//cout<<tempr<<" "<<tempc<<" "<<tempw<<endl;
		//cout<<abs(r-tempr)<<" "<<abs(c-tempc)<<" "<<w-tempw<<endl;
		
		if((abs(r-tempr)==1 && abs(c-tempc)==0 && w-tempw==1) || (abs(r-tempr)==0 && abs(c-tempc)==1 && w-tempw==1)){
			labirynt[tempr][tempc]='o';
			r=tempr;
			c=tempc;
			w=tempw;
	//		printMaze(labirynt);
		}
		
		
	}

	koniecCzasuLiczenia=clock();
	end = clock(); //czas koñca
	difference = ((end - start) / (double)CLOCKS_PER_SEC);
	differenceLiczenia=((koniecCzasuLiczenia - startCzasuLiczenia) / (double)CLOCKS_PER_SEC);
	differenceCzytania= ((koniecCzasuCzytaniaZPliku - startCzasuCzytaniaZPliku) / (double)CLOCKS_PER_SEC);
	

		cout << "Calkowity czas dzialania programu: " << difference << " sekundy."<<endl;
		cout << "Calkowity czas wykonywania obliczen: " << differenceLiczenia << " sekundy."<<endl;
		cout << "Calkowity czas odczytywania danych z pliku: " << differenceCzytania << " sekundy."<<endl;

		
	writeMaze(labirynt,difference,differenceLiczenia,differenceCzytania);
	printMaze(labirynt);
	system("pause");
	
	return 0;
}
