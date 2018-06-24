
#include <fstream>
#include <string>
#include <iostream>
#include <stack>

#include <stdio.h>
#include <string.h>
#include "Hilillo.h"

#include "pthread.h"


using namespace std;
//int MEM_INSTRUCCIONES[40][4];
int MEM_INSTRUCCIONES[40][16];
int MEM_DATOS[24][4];

int CACHE_INSTRUCCIONES_N0[16][9];//Bloques de 4 instrucciones + Etiqueta
int CACHE_INSTRUCCIONES_N1[16][5];//Bloques de 4 instrucciones + Etiqueta

int CACHE_DATOS_N0[8][6];//Bloques de 4 numeros + Etiqueta y Estado
int CACHE_DATOS_N1[4][6];//Bloques de 4 numeros + Etiqueta y Estado

//Pila de hilillos
    stack<Hilillo> M_PilaHilillos;
    
    int IR[4];//Siguiente Instrucción
    
int posMemoriaACache(int nucleo,int pos){

    return 0;
};

int posCacheAMemoria(int nucleo,int pos){
    return 0;
};

struct estadoHilo {
  int ID;
  //false=viejo, true=nuevo
  bool prioridad=false;
  //1=Corriendo,2=Espera,3=ResolviendoFalloDatos,4=ResolviendoFalloInstrucciones,5=PosicionCacheReservada
  int estado=0;
};

int indiceInstrucciones=0;//Para guardar en la mem de Instrucciones

//Leer archivos de texto y convierte los numeros a enteros
void leerArchivo(string nombre){
    ifstream file(nombre);
    string temp;
    int filaInstrucciones = 0;
    while (getline(file, temp)) {
        char * tab2 = new char [temp.length() + 1];
        strcpy(tab2, temp.c_str());
        string parte = "";
        for (int i = 0; i < temp.length() + 1; i++) {
            if (temp[i] != ' ') {
                parte = parte + temp[i];
            } else {

                int numero = stoi(parte);
                MEM_INSTRUCCIONES[filaInstrucciones][indiceInstrucciones] = numero;
                indiceInstrucciones++;
                parte = "";
                if(indiceInstrucciones%16==0){
                    filaInstrucciones++;
                }
            }
        }
        int numero = stoi(parte);
        //cout << numero << "\n";   //imprime el último numero de cada linea y cambio de linea
          MEM_INSTRUCCIONES[filaInstrucciones][indiceInstrucciones] = numero;
               indiceInstrucciones++;
    }
};
//Metodo definido para inicializar todas las matrices 
void inicializar(){
    for(int i=0;i<40;i++){
        for(int j=0;j<16;j++){
            MEM_INSTRUCCIONES[i][j]=0;
        }
    }
    for(int i=0;i<24;i++){
        for(int j=0;j<4;j++){
            MEM_DATOS[i][j]=1;
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<8;j++){
             CACHE_INSTRUCCIONES_N0[i][j]=0;
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
             CACHE_INSTRUCCIONES_N1[i][j]=0;
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<8;j++){
             CACHE_DATOS_N0[i][j]=0;
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
             CACHE_DATOS_N1[i][j]=0;
        }
    }
    for(int j=0;j<4;j++){
             IR[j]=0;
        }
}



//Instrucciones
void DADDI(int nucleo, int X, int Y, int num){};
void DADD(int nucleo, int X, int Y, int Z){};
void DSUB(int nucleo, int X, int Y, int Z){};
void DMUL(int nucleo, int X, int Y, int Z){};
void DDIV(int nucleo, int X, int Y, int Z){};
void BEQZ(int nucleo, int X, int num){};
void BNEZ(int nucleo, int X, int num){};
void JAL(int nucleo, int num){};
void JR(int nucleo, int X){};
void LW(int nucleo, int X, int Y, int num){};

void SiguienteInstruccion(int pos){
    IR[0]=pos;
    IR[2]=pos+1;
       IR[3]=pos+2;
          IR[4]=pos+3;
}

void* correrHilillos(void*){
    cout << "Hilo";
    //leer instruccion
    //ejecutar
    
}

int main() {
    
    //Inicializa todas las matrices 
    //Estructura 
    //Pila de hilillos
    stack<Hilillo> M_PilaHilillos;
    //Estado del hilo
    estadoHilo m_EstadoHilo;
    //Se llena pila de Hilillos
    Hilillo hilo_0;
    M_PilaHilillos.push(hilo_0);
    Hilillo hilo_1;
    M_PilaHilillos.push(hilo_1);
    Hilillo hilo_2;
    M_PilaHilillos.push(hilo_2);
    Hilillo hilo_3;
    M_PilaHilillos.push(hilo_3);
    Hilillo hilo_4;
    M_PilaHilillos.push(hilo_4);
    Hilillo hilo_5;
    M_PilaHilillos.push(hilo_5);
    //Hin
        
    inicializar();
    leerArchivo("0.txt");
    leerArchivo("1.txt");
    leerArchivo("3.txt");
    leerArchivo("4.txt");
    leerArchivo("5.txt");
   
    int quantum = 0;
    printf("Digite el quantum: ");
    scanf("%d",&quantum);
    /*
    //Prueba de impresion matriz de instrucciones
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 16; j++) {
            printf("%d ", MEM_INSTRUCCIONES[i][j]);
            if ((j + 1) % 16 == 0) {
                printf("\n");
            }
        }  
    }
    */

 pthread_t NUCLEO0, NUCLEO1; //Un hilo para cada núcleo

    pthread_create(&NUCLEO0, NULL, &correrHilillos, NULL);
    pthread_create(&NUCLEO1, NULL, &correrHilillos, NULL);
    pthread_join(NUCLEO0, NULL);   
pthread_join(NUCLEO1, NULL); 
    return 0;
    
}