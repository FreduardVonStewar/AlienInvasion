#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
/**
 *	Clase para cada rectángulo del mapa.
 */
class Rectangle{
	public:
		Rectangle(){}
		Rectangle(char w,int x,int y){
			W = w;
			Xi = x;
			Yi = y;
			Xf = x+1;
			Yf = y+1;
			Width = 1;
			Height = 1;
		}

		int getXi(){return Xi;}
		int getYi(){return Yi;}
		int getXf(){return Xf;}
		int getYf(){return Yf;}
		char getW(){return W;}
		int getWidth(){return Width;}
		int getHeight(){return Height;}
		float getPx(){return Xi+((Xf-Xi)/2.0f);}
		float getPy(){return Yi+((Yf-Yi)/2.0f);}
		int getSize(){return Width*Height;}
		void recalculateSize(int x, int y){
			if(x < Xi){
				Xi = x;
			}else if(x+1 > Xf){
				Xf = x+1;
			}
			if(y < Yi){
				Yi = y;
			}else if(y+1 > Yf){
				Yf = y+1;
			}
			Width = (Xf - Xi);
			Height = (Yf - Yi);
		}
	private:
		char W;
		int Xi,Yi,Xf,Yf,Width,Height;
};
/**
 *	Guarda el resultado de cada mapa en un archivo de salida
 *  @param MAP El mapa a guardar
 *	@param map_size El tamaño del mapa a guardar
 *	@param scale Escala a multiplicar con los valores centrales Px,Py de cada rectángulo
 *  @param output referencia del archivo de salida
 */
void showMap(Rectangle **MAP, int map_size, float scale, ofstream &output){
	for(int i = 0;i < map_size; i++){
		if(MAP[i] != NULL){
			output << MAP[i]->getW() << ":" << MAP[i]->getPy()*scale << "," << MAP[i]->getPx()*scale;
			if(i<map_size-1){output << ";";}
		}
	}
	output << "\n";
}
/**
 *	Organiza el mapa de rectángulos en orden ascendente.
 *	@param MAP Mapa de rectángulos
 *	@param li Lado izquiedo del mapa
 *	@param ld Lado derecho del mapa
 */
void quickSort(Rectangle **MAP, int li, int ld){
	int i,j;
	Rectangle *pvt,*aux;
	pvt = MAP[(int)(li+ld)/2];
	i=li;
	j=ld;
	do{
		while(MAP[i]->getSize()<pvt->getSize())i++;
		while(MAP[j]->getSize()>pvt->getSize())j--;
		if(i<=j){
			aux = MAP[i];
			MAP[i]=MAP[j];
			MAP[j]=aux;
			i++;
			j--;
		}
	}while(i<j);
	if(li<j){
		quickSort(MAP,li,j);
	}
	if(i<ld){
		quickSort(MAP,i,ld);
	}
}

/**
 *	Ordena alfabéticamente el Mapa de rectángulos que tengan el mismo tamaño
 *	@param MAP Mapa de rectángulos
 *	@param map_size Tamaño del mapa de rectángulos
 */
void riSort(Rectangle **MAP, int map_size){
    int i,j,k;
    char Wi,Wj;
    Rectangle *aux,*saux;
    for(i=0;i<map_size-1;i++){
		j=i+1;
		k=-1;
		Wi = MAP[i]->getW();
		if(Wi >= 97)Wi-=71;
		else Wi-=65;
		while(j<map_size){
			Wj = MAP[j]->getW();
			if(Wj >= 97)Wj-=71;
			else Wj-=65;
			if(Wj<Wi && MAP[i]->getSize()==MAP[j]->getSize()){
				k=j;
			}
			++j;
		}
		if(k>=0){
			aux = MAP[k];
			MAP[k] = MAP[i];
			MAP[i] = aux;
			i--;
		}
    }
}
int main(){
	ifstream file;
	ofstream output;
	file.open("input.txt");
	output.open("output.txt");
	output << fixed << setprecision(3);
	int N,X,Y,i,j,k,map_size,kk;
	float Z;
	char W;
	Rectangle **MAP,**RESULT_MAP;
	MAP = new Rectangle*[52];
	for(i = 0; i < 52; i++){
		MAP[i] = NULL;
	}
	file >> N;
	while(N>0){
		map_size=0;
		--N;
		for(i = 0; i < 52; i++){
			if(MAP[i] != NULL){
                delete MAP[i];
				MAP[i] = NULL;
			}

		}
		file >> X;
		file >> Y;
		file >> Z;
		for(i = 0; i < X; i++){
			for(j = 0; j < Y; j++){
				file >> W;
				if((W >= 65 && W <= 90)||(W>=97 &&W<=122)){
					if(W>=97){
						k = W-71;
					}else if(W>=65){
						k = W-65;
					}
					if(MAP[k] == NULL){
						MAP[k] = new Rectangle(W,j,i);
						++map_size;
					}else{
						MAP[k]->recalculateSize(j,i);
					}
				}
			}
		}
		j=0;
		RESULT_MAP = new Rectangle*[map_size];
		for(i=0;i<52;i++){
			if(MAP[i]!=NULL){
				RESULT_MAP[j++] = MAP[i];
			}
		}
        quickSort(RESULT_MAP,0,map_size-1);
        riSort(RESULT_MAP,map_size);
		showMap(RESULT_MAP,map_size,Z,output);
		delete [] RESULT_MAP;
	}
	file.close();
	cout << "Done." << endl;
	return 0;
}
