#pragma once
#ifndef game_h
#define game_h
#include <limits>
#include <list>
#include <stdio.h>
#include "Tablero.h"
#include "time.h"
#include <assert.h>

template <class T>

//Se deifne una clase llamada sptr que gestiona punteros
class sptr
{
private:
    T* pData;
public:
    sptr(T* pVal) : pData(pVal) {}
    ~sptr() { delete pData; }
    T& operator*() { return *pData; }
    T* operator->() { return pData; }
};

class game
{
    //Se declara sptr<board> se utiliza
    /*sptr<board> currentB;
    static const int maxIterDepth;
    move* bestM;
    move* tempBestM;
    int maxdepth;
    int cdepth;
    bool timeUp;
    bool gameOver;
    bool reachedEnd;
    time_t startTime;
    time_t endTime;
    time_t startTimeD;
    time_t endTimeD;*/
    sptr<board> currentB; // Puntero inteligente al tablero actual.
    static const int maxIterDepth; // Profundidad máxima de iteración para Alpha-Beta.
    move* bestM; // Mejor movimiento encontrado.
    move* tempBestM; // Movimiento temporal encontrado.
    int maxdepth; // Profundidad máxima actual para Alpha-Beta.
    int cdepth; // Profundidad actual durante la búsqueda.
    bool timeUp; // Indica si se ha agotado el tiempo de cálculo.
    bool gameOver; // Indica si el juego ha terminado.
    bool reachedEnd; // Indica si se ha alcanzado el final de la búsqueda.
    time_t startTime; // Tiempo de inicio de la búsqueda.
    time_t endTime; // Tiempo de finalización de la búsqueda.
    time_t startTimeD; // Tiempo de inicio de la profundidad actual.
    time_t endTimeD; // Tiempo de finalización de la profundidad actual.


    /*
    void printGame();
    void outputMessage();
    void computerTurn();
    int alphabeta(sptr<board>&, int, int, int);
    void endMessage();*/

    // Funciones privadas para el manejo del juego.
    void printGame(); // Imprime el estado actual del juego.
    void outputMessage(); // Muestra el mensaje de salida del juego.
    void computerTurn(); // Realiza el turno de la computadora.

    // Función de búsqueda Alpha-Beta para la toma de decisiones de la computadora.
    int alphabeta(sptr<board>&, int, int, int);

    // Función para mostrar el mensaje de finalización del juego.
    void endMessage();
public:
    game();// Constructor de la clase game.
    void play(); // Inicia el juego y permite a los jugadores realizar movimientos.

};


#endif 
