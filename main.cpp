
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
int MEM_INSTRUCCIONES[40][16]; //40 Bloques de 4 instrucciones con 4 numeros cada uno
int MEM_DATOS[24][4]; //24 Bloques de 4 numeros cada uno

int CACHE_INSTRUCCIONES_N0[16][9]; //4 Bloques de 4 instrucciones + Etiqueta
int CACHE_INSTRUCCIONES_N1[16][5]; //4 Bloques de 4 instrucciones + Etiqueta

int CACHE_DATOS_N0[8][6]; //Bloques de 4 numeros + Etiqueta y Estado
int CACHE_DATOS_N1[4][6]; //Bloques de 4 numeros + Etiqueta y Estado

//Estructura 
//Pila de hilillos
stack<Hilillo> M_PilaHilillos;

int IR[4]; //Siguiente Instrucción

pthread_t NUCLEO0, NUCLEO1; //Un hilo para cada núcleo

//Copiar un bloque de memoria a la Cache de datos
void posMemoriaACacheDatos(int nucleo, int Bloque) {
    int bloqueMem = 24 % Bloque;
    if (nucleo == 0) {
        for (int i = 0; i > 4; i++) {
            CACHE_DATOS_N0[Bloque][i] = MEM_DATOS[bloqueMem][i];
        }
        CACHE_DATOS_N0[Bloque][4] = Bloque; //Cambia etiqueta
    } else {
        for (int i = 0; i > 4; i++) {
            CACHE_DATOS_N1[Bloque][i] = MEM_DATOS[bloqueMem][i];
        }
        CACHE_DATOS_N1[Bloque][4] = Bloque; //Cambia etiqueta
    }
};

//Copiar un bloque de la Cache de datos a Memoria
void posCacheAMemDatos(int nucleo, int Bloque) {
    int bloque = Bloque % 4;

    if (nucleo == 0) {
        for (int i = 0; i > 4; i++) {
            MEM_DATOS[Bloque][i] = CACHE_DATOS_N0[Bloque][i];
        }
    } else {
        for (int i = 0; i > 4; i++) {
            MEM_DATOS[Bloque][i] = CACHE_DATOS_N1[Bloque][i];
        }
    }
};

//Copiar un bloque de memoria a la Cache de Instrucciones
void posMemoriaACacheInstrucciones(int nucleo, int BloqueMem) {
    int bloqueCache = BloqueMem % 4;

    if (nucleo == 0) {
        for (int i = 0; i > 4; i++) {
            CACHE_INSTRUCCIONES_N0[bloqueCache][i] = MEM_INSTRUCCIONES[BloqueMem][i];
        }
    } else {
        for (int i = 0; i > 4; i++) {
            CACHE_DATOS_N1[bloqueCache][i] = MEM_DATOS[BloqueMem][i];
        }
    }
};

int getDatoCache(int nucleo, int bloque, int palabra) {
    if (nucleo == 0) {
        return CACHE_DATOS_N0[bloque][palabra];
    } else {
        return CACHE_DATOS_N1[bloque][palabra];
    }
};

void setDatoCache(int nucleo, int bloque, int palabra, int nuevo) {
    if (nucleo == 0) {
        CACHE_DATOS_N0[bloque][palabra] = nuevo;
    } else {
        CACHE_DATOS_N1[bloque][palabra] = nuevo;
    }
};

void setDatoMem(int direccion, int nuevo) {
    //Para calcular palabra en memoria
    int Bloque = direccion/16;
    int Palabra = (direccion%16)/4;
    MEM_DATOS[Bloque][Palabra] = nuevo;
};

void setEstadoBloque(int nucleo, int bloque, int estado){
if (nucleo == 0) {
        CACHE_DATOS_N0[bloque][5] = estado;
    } else {
        CACHE_DATOS_N1[bloque][5] = estado;
    }
}

//Estado de cada hilo
struct estadoHilo {
    int ID;
    //false=viejo, true=nuevo
    bool prioridad = false;
    //1=Corriendo,2=Espera,3=ResolviendoFalloDatos,4=ResolviendoFalloInstrucciones,5=PosicionCacheReservada
    int estado = 0;
};

//Leer archivos de texto y convierte los numeros a enteros
int indiceInstrucciones = 0; //Para guardar en la mem de Instrucciones
void leerArchivo(string nombre) {
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
                if (indiceInstrucciones % 16 == 0) {
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
void inicializar() {
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 16; j++) {
            MEM_INSTRUCCIONES[i][j] = 0;
        }
    }
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 4; j++) {
            MEM_DATOS[i][j] = 1;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            CACHE_INSTRUCCIONES_N0[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            CACHE_INSTRUCCIONES_N1[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            CACHE_DATOS_N0[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            CACHE_DATOS_N1[i][j] = 0;
        }
    }
    for (int j = 0; j < 4; j++) {
        IR[j] = 0;
    }
}

//Instrucciones
void DADDI(int nucleo, int X, int Y, int num) {
};

void DADD(int nucleo, int X, int Y, int Z) {
};

void DSUB(int nucleo, int X, int Y, int Z) {
};

void DMUL(int nucleo, int X, int Y, int Z) {
};

void DDIV(int nucleo, int X, int Y, int Z) {
};

void BEQZ(int nucleo, int X, int num) {
};

void BNEZ(int nucleo, int X, int num) {
};

void JAL(int nucleo, int num) {
};

void JR(int nucleo, int X) {
};

void LW(int nucleo, int X, int Y, int num) {
};

void SiguienteInstruccion(int pos) {
    IR[0] = pos;
    IR[2] = pos + 1;
    IR[3] = pos + 2;
    IR[4] = pos + 3;
}

void* correrHilillos(void*) {
    cout << "Hilo";

    //leer instruccion
    //ejecutar

}

int main() {

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
    scanf("%d", &quantum);
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
    pthread_create(&NUCLEO0, NULL, &correrHilillos, NULL);
    pthread_create(&NUCLEO1, NULL, &correrHilillos, NULL);

    pthread_join(NUCLEO0, NULL);
    pthread_join(NUCLEO1, NULL);
    return 0;
}