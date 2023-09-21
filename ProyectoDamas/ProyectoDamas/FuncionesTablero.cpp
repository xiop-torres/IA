//librerias utilizadas
#include <stdio.h>
#include <assert.h>
#include "Tablero.h"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <limits>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::list;
using std::string;
using std::tolower;

//// Función para convertir y mostrar un comando en formato legible.
void board::convertCommand(const string& s)
{   
    //El comando se pasa como una cadena 's' y se procesa para mostrar 
    //las coordenadas de movimiento de manera legible.
    string::const_iterator it = s.begin();
    cout << "(" << (*it) << ", ";
    it += 2;
    cout << (*it) << ") ";
    it += 2;
    while (*it != '-')
    {
        cout << "-> (" << (*it) << ", ";
        it += 2;
        cout << (*it) << ") ";
        it += 2;
    }
}

//Función para recibir la entrada del jugador y procesarla.
void board::inputCommand()
{
    printMoves();
    string m;
    getline(cin, m);
    list<move*>::iterator it = mlist.begin();
    while (it != mlist.end())
    {   
        if ((*it)->command == m)
        {   
            cout << "Has elegido el siguiente movimiento: ";
            convertCommand((*it)->command);
            cout << endl;
            break;
        }
        ++it;
        if (it == mlist.end())
        {
            getline(cin, m);
            it = mlist.begin();
        }
    }
    makeMove(*it);
}

/// Función para imprimir el estado actual del tablero de juego.
void board::printBoard()
{
    int count = 0;
    cout << "       " << count;
    ++count;
    while (count != 8)
    {
        cout << "     " << count++;
    }
    cout << " " << endl;
    string lineEven = "    |XXXXX|     |XXXXX|     |XXXXX|     |XXXXX|     |";
    string lineOdd = "    |     |XXXXX|     |XXXXX|     |XXXXX|     |XXXXX|";
    string linebreak = "    -------------------------------------------------";


    cout << linebreak << endl;
    for (int i = 0; i != 8; ++i)
    {
        printline(i, lineEven, lineOdd);
        if (i != 7)
            cout << linebreak << endl;
    }
    cout << linebreak << endl;
    cout << endl;
}

// Función para imprimir los movimientos disponibles para el jugador actual
void board::printMoves()
{
    if (color == 'b')
        cout << "Juega la computadora" << endl;
    else cout << "Juega tu" << endl;
    cout << "Movimientos permitidos:" << endl;
    list<move*>::const_iterator it = mlist.begin(); //Muestra de lista de mov válidos que el jugador pueda realizar
    for (; it != mlist.end(); ++it)
    {
        //cout << "Move: ";
        convertCommand((*it)->command);
        cout << endl;
    }
    cout << endl;
}

// Función para realizar un movimiento en el tablero
void board::makeMove(move* m)
{
    if (!m->jpoints.empty())
    {
        list<jump*>::iterator it = m->jpoints.begin();
        for (; it != m->jpoints.end(); ++it)
            arr[(*it)->x][(*it)->y] = 'e';
    }

    char c = arr[m->xi][m->yi];
    arr[m->xi][m->yi] = 'e';
    arr[m->xf][m->yf] = c;

    handleKinging(m->xf, m->yf);
    changeTurn();
}

// Función para deshacer un movimiento en el tablero
void board::undoMove(move* m)
{
    if (!m->jpoints.empty())
    {
        for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
        {
            arr[(*it)->xs][(*it)->ys] = 'e';
            arr[(*it)->x][(*it)->y] = (*it)->c;
            arr[(*it)->xend][(*it)->yend] = 'e';
        }
    }
    arr[m->xf][m->yf] = 'e';
    arr[m->xi][m->yi] = m->mP;
}

// Función para evaluar la estrategia de ocupar esquinas diagonales.
int board::cornerDiagonal(char losing, char winning)
{
    int c = 0;
    if (tolower(arr[0][0]) == losing || tolower(arr[1][0]) == losing)
    {
        c += 9;
        if (tolower(arr[0][0]) == winning)
            c -= 3;
        if (tolower(arr[1][0]) == winning)
            c -= 3;
        if (tolower(arr[1][1]) == winning)
            c -= 1;
        if (tolower(arr[2][0]) == winning)
            c -= 1;
        if (tolower(arr[2][1]) == winning)
            c -= 1;
        if (tolower(arr[3][1]) == winning)
            c -= 1;
    }
    if (tolower(arr[6][3]) == losing || tolower(arr[7][3]) == losing)
    {
        c += 9;
        if (tolower(arr[4][2]) == winning)
            c -= 1;
        if (tolower(arr[5][2]) == winning)
            c -= 1;
        if (tolower(arr[5][3]) == winning)
            c -= 1;
        if (tolower(arr[6][2]) == winning)
            c -= 1;
        if (tolower(arr[6][3]) == winning)
            c -= 3;
        if (tolower(arr[7][3]) == winning)
            c -= 3;
    }
    return c;
}

// Función para evaluar la posición actual del tablero y calcular una puntuación
int board::evaluate()
{
    int a1 = 0, a2 = 0, b = 0, c = 0, d = 0;
    for (int i = 0; i != 8; ++i)
        for (int j = 0; j != 4; ++j)
        {
            if (arr[i][j] == 'b')
            {
                a1 += 2;
                if (i == 0)
                    b += 9;
                else b += i;
                c += 1;
            }
            else if (arr[i][j] == 'r')
            {
                a2 -= 2;
                if (i == 7)
                    b -= 9;
                else b -= (7 - i);
                c -= 1;
            }
            else if (arr[i][j] == 'B')
            {
                a1 += 3;
                c += 1;
            }
            else if (arr[i][j] == 'R')
            {
                a2 -= 3;
                c -= 1;
            }
        }
    if (c > 0 && a2 >= -8)
        d -= cornerDiagonal('r', 'b');
    else if (c < 0 && a1 <= 8)
        d += cornerDiagonal('b', 'r');
    a1 *= 100000000;
    a2 *= 100000000;
    b *= 1000000;
    c *= 10000;
    d *= 100;
    int e = rand() % 100;
    if (color == 'r')
        e = -e;
    return a1 + a2 + b + c + d + e;
}

// Función para configurar el estado inicial del juego.
void board::startup()
{
    reset();
    whoComputer();
}




