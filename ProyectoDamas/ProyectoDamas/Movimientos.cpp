//Librerias necesarias
#include <stdio.h>
#include <assert.h>
#include "Tablero.h"
#include <cctype>
#include <iostream>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::toupper;
// Funcion para verificar los movimiento vecinos de una pieza en la posicion(x,y)
void board::checkNeighbors(int& x, int& y)
{
    //Si la pieza es 'b' o 'R'(rey negro) se generan mov vecinos hacia abajo.
    if (tolower(arr[x][y]) == 'b' || arr[x][y] == 'R')
    {
        if (x % 2 == 0)
        {
            createMove(x, y, x + 1, y);
            createMove(x, y, x + 1, y + 1);
        }
        else
        {
            createMove(x, y, x + 1, y);
            createMove(x, y, x + 1, y - 1);
        }
    }
    //Si la pieza es 'r'o'B'(rey rojo) se generan movimientos hacia arriba
    if (tolower(arr[x][y]) == 'r' || arr[x][y] == 'B')
    {
        if (x % 2 == 0)
        {
            createMove(x, y, x - 1, y);
            createMove(x, y, x - 1, y + 1);
        }
        else
        {
            createMove(x, y, x - 1, y);
            createMove(x, y, x - 1, y - 1);
        }
    }
}
// Funcion para crear un movimiento desde la posicion(xi,yi) a (xf,yf)
void board::createMove(const int& xi, const int& yi, int xf, int yf)
{
    //// Si la posición (xf, yf) es válida y está vacía 'e', se crea un movim
    if (isValidPos(xf, yf) && arr[xf][yf] == 'e')
    {
        move* m = new move(arr[xi][yi], xi, yi, xf, yf);
        mlist.push_back(m);
        convert(xi, yi, m->command);
        convert(xf, yf, m->command);
        m->command += "-1";
    }
}//agregando el mov a la  lista 'mlist' 

//Funcion para listar todos los movimientos posibles para el jugador actual.
bool board::listMoves()
{   
    //Limpia la lista de movimientos
    while (!mlist.empty())
    {
        delete mlist.front();
        mlist.pop_front();
    }
    // Si el color actual es 'b'-negro, se exploran las piezas negras.
    if (color == 'b')
    {
        for (int i = 0; i != 8; ++i)
            for (int j = 0; j != 4; ++j)
                if (arr[i][j] == color || arr[i][j] == toupper(color))
                    checkNeighbors(i, j);
    }
    // Si el color actual es 'r' (rojo), se exploran las piezas rojas
    else
    {
        for (int i = 7; i != -1; --i)
            for (int j = 0; j != 4; ++j)
                if (arr[i][j] == color || arr[i][j] == toupper(color))
                    checkNeighbors(i, j);
    }
    //Si no se encuentran movimientos la función devuelve false sino devuelve true.
    if (mlist.empty())
        return false;
    return true;
}

