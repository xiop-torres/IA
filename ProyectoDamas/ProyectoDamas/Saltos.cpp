#include <stdio.h>
#include <assert.h>
#include "Tablero.h"
#include <iostream>
#include <cctype>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::toupper;

// Función para crear una clave numérica única segun las coordenadas de un salto
inline int board::createkey(int xs, int ys, int xj, int yj, int xe, int ye)
{
    return ye + xe * 10 + yj * 100 + xj * 1000 + ys * 10000 + xs * 100000;
}

//Funcion para invertir una clave numerica
int board::reverse(int i)
{
    int num = 0;
    while (i / 100 > 0)
    {
        num += i % 100;
        num *= 100;
        i /= 100;
    }
    num += i;
    return num;

}//Para luego verificar si un salto ha sido registrado previamente

//Funcion para crear un salto y agregarlo a la lista de saltos
void board::createJump(list<jump*>& jlist, char c, int xs, int ys, int xj, int yj, int xe, int ye, jump* jp)
{
    arr[xs][ys] = 'e';
    int key = createkey(xs + 1, ys + 1, xj + 1, yj + 1, xe + 1, ye + 1);
    jump* jcheck = jp;
    while (jcheck != NULL)
    {
        if (key == jcheck->key || key == reverse(jcheck->key))
            return;
        jcheck = jcheck->prev;
    }
    jump* j = new jump(c, arr[xj][yj], xs, ys, xj, yj, xe, ye, jp, key);
    if (jp != NULL)
        jp->noNext = false;
    jlist.push_front(j); //agrega a la lista de saltos
    jumpAvailable(jlist, c, xe, ye, j);
}

//Funcion para crear movimiento en los saltos disponibles
//Cada mov consisente en una secuencia de saltos consecutivos
void board::createJumpMove(list<jump*>& jlist)
{
    if (!jlist.empty())
    {
        list<jump*>::const_iterator it = jlist.begin();
        for (; it != jlist.end(); ++it)
        {
            if ((*it)->noNext)
            {
                move* m = new move((*it)->jumpingPiece, -1, -1, -1, -1);
                jump* jp = (*it);
                while (jp != NULL)
                {
                    m->jpoints.push_front(jp);
                    ++jp->numTimes;
                    jp = jp->prev;
                }
                m->xi = m->jpoints.front()->xs;
                m->yi = m->jpoints.front()->ys;
                convert(m->jpoints.front()->xs, m->jpoints.front()->ys, m->command);
                for (list<jump*>::iterator it = m->jpoints.begin(); it != m->jpoints.end(); ++it)
                {
                    convert((*it)->xend, (*it)->yend, m->command);
                    if ((*it)->noNext)
                    {
                        m->xf = (*it)->xend;
                        m->yf = (*it)->yend;
                    }
                }

                mlist.push_back(m);
                m->command += "-1";
                undoMove(m);
            }
        }
    }
}

//Funcion que verifica y crea saltos disponibles parauna pieza en la posicion x y
void board::jumpAvailable(list<jump*>& jlist, char c, int x, int y, jump* jp = NULL)
{
    if (tolower(c) == 'b' || c == 'R')
    {
        if (x % 2 == 0)
        {
            if (jumpConditions(x + 1, y, x + 2, y - 1))
                //
                createJump(jlist, c, x, y, x + 1, y, x + 2, y - 1, jp);
            if (jumpConditions(x + 1, y + 1, x + 2, y + 1))
                createJump(jlist, c, x, y, x + 1, y + 1, x + 2, y + 1, jp);
        }
        else
        {
            if (jumpConditions(x + 1, y - 1, x + 2, y - 1))
                createJump(jlist, c, x, y, x + 1, y - 1, x + 2, y - 1, jp);
            if (jumpConditions(x + 1, y, x + 2, y + 1))
                createJump(jlist, c, x, y, x + 1, y, x + 2, y + 1, jp);
        }
    }
    if (tolower(c) == 'r' || c == 'B')
    {
        if (x % 2 == 0)	//even x
        {
            if (jumpConditions(x - 1, y + 1, x - 2, y + 1))
                createJump(jlist, c, x, y, x - 1, y + 1, x - 2, y + 1, jp);
            if (jumpConditions(x - 1, y, x - 2, y - 1))
                createJump(jlist, c, x, y, x - 1, y, x - 2, y - 1, jp);
        }
        else
        {
            if (jumpConditions(x - 1, y - 1, x - 2, y - 1))
                createJump(jlist, c, x, y, x - 1, y - 1, x - 2, y - 1, jp);
            if (jumpConditions(x - 1, y, x - 2, y + 1))
                createJump(jlist, c, x, y, x - 1, y, x - 2, y + 1, jp);
        }
    }
}
//Funcion que verifica si hay saltos disponibles en el tablero
bool board::jumpsAvailable()
{
    while (!mlist.empty()) //Limpia la lista de mov mlist
    {
        delete mlist.front();
        mlist.pop_front();
    }
    //Verifica si cada pieza puede realizar saltos llamando a jumpAvailable()
    for (int i = 0; i != 8; ++i)
    {
        for (int j = 0; j != 4; ++j)
        {
            if (arr[i][j] == color || arr[i][j] == toupper(color))
            {
                list<jump*> jlist;
                jumpAvailable(jlist, arr[i][j], i, j, NULL);
                createJumpMove(jlist);
            }
        }
    }
    if (mlist.empty())
        return false;
    return true;
}
//Funcion para verificar si se cumplen las condiciones para un salto
bool board::jumpConditions(int xj, int yj, int xe, int ye)
{
    //Verifica si las coordenadas son validas,si la casilla de inicio no esta vacia 
    //si pertence al oponente , si la casilla de destino esta vacia , si la pieza no es delmismo color que la casilla de destino.
    if (isValidPos(xj, yj) && isValidPos(xe, ye) && arr[xj][yj] != 'e' &&
        arr[xj][yj] != color && arr[xe][ye] == 'e' && arr[xj][yj] != std::toupper(color))
        return true;
    return false;
}


