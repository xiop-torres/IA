#include <stdio.h>
#include <assert.h>
#include "Tablero.h"
#include <iostream>
#include <list>
#include <string>

using namespace std;
// Función para convertir las coordenadas (x, y) en formato de cadena y agrega a la cadena 's'
void board::convert(const int& x, const int& y, string& s)
{
    char c1 = '0' + x;
    char c2;
    if (x % 2 == 0)
    {
        c2 = '0' + (2 * y + 1);
    }
    else
    {
        c2 = '0' + (2 * y);
    }
    //Coordenada se formatean como 2 numeros separados por espacios
    s += c1;
    s += ' ';
    s += c2;
    s += ' ';
}
//Esta función convierte la coordenada 'y' en función de la fila 'x'. 
inline int board::convertY(const int& x, const int& y)
{
    if (x % 2 == 0)
        return (2 * y + 1);
    else return (2 * y);
}

//Esta función imprime una línea del tablero con el número de fila 'i'.
void board::printline(const int& i, const string& lineEven, const string& lineOdd)
{
    if (i % 2 == 0)
    {
        cout << lineEven << endl;
        cout << "  " << i << " |XXXXX|";
        for (int j = 0; j != 3; ++j)
        {
            cout << "  ";
            printcolor(arr[i][j]);
            cout << "  |XXXXX|";
        }
        cout << "  ";
        printcolor(arr[i][3]);
        cout << "  |" << endl;;
        cout << lineEven << endl;
    }
    else
    {
        cout << lineOdd << endl;
        cout << "  " << i << " |";
        for (int j = 0; j != 3; ++j)
        {
            cout << "  ";
            printcolor(arr[i][j]);
            cout << "  |XXXXX|";
        }
        cout << "  ";
        printcolor(arr[i][3]);
        cout << "  |XXXXX|" << endl;;
        cout << lineOdd << endl;
    }
}

// Función para imprimir un carácter en un color específico en la consola.
void board::printcolor(const char& c)
{
    if (c == 'e')
        cout << ' ';
    else if (c == 'r' || c == 'R')
    {
        string default_console = "\033[0m";
        string color = "\033[1;31m";
        cout << color << c;
        cout << default_console;
    }
    else
    {
        string default_console = "\033[0m";
        string color = "\033[1;33m";
        cout << color << c;
        cout << default_console;
    }
}

//Esta funcion determina qué jugador es lacomputadora y configura de tiempo para la jugada de la compu.
void board::whoComputer()
{
    bool b = true;
    char c = ' ';
    while (b)
    {
        board::isComputer[0] = true;
        b = false;
    }
    b = true;
    c = ' ';
    while (b)
    {
        board::isComputer[1] = false;
        b = false;
    }
    if (board::isComputer[0] == true || board::isComputer[1] == true)
    {
        //limite de tiempo 3 segundos
        timeLimit = 3; 
    }
}
