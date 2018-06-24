/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Hilillo.cpp
 * Author: Diego
 * 
 * Created on 21 de junio de 2018, 11:55 AM
 */

#include "Hilillo.h"

Hilillo::Hilillo() {
    //Inicializa el hilillo
    this->ID = 0;
    this->PC = 384;
    for (int i = 0; i < 32; i++) {
        this->REGISTROS[i] = 0;
    }
    this->duracion = 0;
}

Hilillo::Hilillo(const Hilillo& orig) {
}

Hilillo::Hilillo(int id, int posicion) {
      //Inicializa el hilillo
    this->ID = id;
    this->PC = posicion;
    for (int i = 0; i < 32; i++) {
        this->REGISTROS[i] = 0;
    }
    this->duracion = 0;
}

Hilillo::~Hilillo() {
}




