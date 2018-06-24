/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Hilillo.h
 * Author: Diego
 *
 * Created on 21 de junio de 2018, 11:55 AM
 */

#ifndef HILILLO_H
#define HILILLO_H

class Hilillo {
public:
    Hilillo();
    Hilillo(const Hilillo& orig);
    virtual ~Hilillo();
    Hilillo(int id, int posicion);
    int PC;
    int REGISTROS[32];
    int ID;
    int duracion;
    
    
private:

};

#endif /* HILILLO_H */

