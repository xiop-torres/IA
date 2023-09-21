#include <assert.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "Game.h"
#include <iostream>
#include <limits>
#include <list>
#include "Tablero.h"
#include <time.h>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::min; 
using std::list;

//nivel max de profundidad en el alg alfa-beta
const int game::maxIterDepth = 2;

//constructr de la clase game que inicializa las variables miembro
game::game() : currentB(sptr<board>(new board())), bestM(NULL), tempBestM(NULL), maxdepth(0),
cdepth(0), timeUp(false), gameOver(false), reachedEnd(false), startTime(0), endTime(0),
startTimeD(0), endTimeD(0) {}
// currentB es un objeto tipo sptr<board> que representa el estado actual del tablero del juego

void game::play()
{
    gameOver = false;//establece gameover como falso
    currentB->startup(); //llamada al metodo para configurar el estado inicial del tablero
    //bucle que continua hasta que gameover sea verdadero.
    while (!gameOver)
        printGame();//llama printgame
}

//uestra mnsj del juego final indica si el jugador o la comp gano el juego
void game::endMessage()
{
    gameOver = true;
    cout << "Juego perdido." << endl;
    cout << endl;
    if (currentB->getTurn() == 'r')
        cout << "Computadora gana" << endl;
    else cout << "Tu ganaste" << endl;
}

//turno de la computadora , muestra los mov disponibles en el tablero
void game::computerTurn()
{
    cout << "Valor actual de maxdepth: " << maxdepth << endl;

    currentB->printMoves();
    cout << "La computadora hara un movimiento." << endl;
    if (currentB->mlist.size() == 1)
    {
        bestM = currentB->mlist.front();
        time(&startTime);
        time(&endTime);
        cdepth = 0;
    }
    else
    {
        time(&startTime);
        for (int i = 1; i != maxIterDepth; ++i)
        {
            time(&startTimeD);
            maxdepth = i;
            alphabeta(currentB, i, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
            time(&endTimeD);
            if (difftime(endTimeD, startTimeD) >= ((board::timeLimit) / 2))
            {
                time(&endTime);
                timeUp = true;
                break;
            }
            if (timeUp)
                break;
            else
                bestM = tempBestM;
            if (reachedEnd)
                break;
        }
    }
    assert(bestM != NULL);
    outputMessage();
}

//muestra los mnsj despues de cada mov
void game::outputMessage()
{
    currentB->makeMove(bestM);
    if (timeUp && cdepth != maxIterDepth && !reachedEnd)

        board::convertCommand(bestM->command);
    cout << endl;

    bestM = NULL;
    tempBestM = NULL;
    timeUp = false;
    reachedEnd = false;
}

//llama al metodo printBoard del objeto currentB para mostrar el estado actual del tablero
void game::printGame()
{
    //verifica si ha finalizado e juego pero si no ->
    currentB->printBoard();
    if (currentB->terminalTest())
        endMessage();
    else if (currentB->isComputerTurn()) //es el turno de la computadora
        computerTurn();
    else
        currentB->inputCommand(); //si no , es el turno del usuario
}

//Algoritmo utilizado es alfabeta , evalua posibles mov y determina la mejor jugada para la computadora
/*int game::alphabeta(sptr<board>& b, int depth, int alpha, int beta)
{
    if (depth != maxdepth && b->terminalTest())
    {
        //b->printBoard();
        reachedEnd = true;
        cdepth = maxdepth;
        if (b->getTurn() == 'r')
            return std::numeric_limits<int>::max();
        else return std::numeric_limits<int>::min();
    }
    reachedEnd = false;
    if (depth == 0)
        return b->evaluate();
    list<move*>::iterator iter = b->mlist.begin();

    int localalpha = std::numeric_limits<int>::min();
    int localbeta = std::numeric_limits<int>::max();

    if (b->getTurn() == 'b')
    {
        for (; iter != b->mlist.end(); ++iter)
        {
            time(&endTime);

            if (difftime(endTime, startTime) >= (board::timeLimit - 1))
            {
                timeUp = true;
                break;
            }

            b->makeMove(*iter);
            sptr<board> newB(new board(*b));
            int value = alphabeta(newB, depth - 1, alpha, min(localbeta, beta));
            b->undoMove(*iter);
            b->changeTurn();

            if (value > alpha)
            {
                alpha = value;
                if (depth == maxdepth)
                    tempBestM = (*iter);
            }

            if (alpha >= beta && depth < maxdepth)
                return alpha;
        }

        if (!timeUp && depth == maxdepth)
            cdepth = depth;

        return alpha;
    }

    else
    {
        for (; iter != b->mlist.end(); ++iter)
        {
            time(&endTime);
            if (difftime(endTime, startTime) >= (board::timeLimit - 1))
            {
                timeUp = true;
                break;
            }
            b->makeMove(*iter);
            sptr<board> newB(new board(*b));
            int value = alphabeta(newB, depth - 1, max(localalpha, alpha), beta);
            b->undoMove(*iter);
            b->changeTurn();

            if (value < beta)
            {
                beta = value;
                if (depth == maxdepth)
                    tempBestM = (*iter);
            }

            if (alpha >= beta)
                return beta;
        }

        if (!timeUp && depth == maxdepth)
            cdepth = depth;

        return beta;
    }
}
*/

int game::alphabeta(sptr<board>& b, int depth, int alpha, int beta)
{
    // Verifica si la profundidad no es máxima y se ha alcanzado un estado terminal.
    if (depth != maxdepth && b->terminalTest())
    {
        reachedEnd = true; // Indica que se ha alcanzado el final.
        cdepth = maxdepth; // Establece la profundidad actual como la máxima.
        if (b->getTurn() == 'r')
            return std::numeric_limits<int>::max(); // Valor máximo para el jugador 'r'.
        else
            return std::numeric_limits<int>::min(); // Valor mínimo para el jugador 'b'.
    }
    reachedEnd = false; // No se ha alcanzado el final.

    // Cuando la profundidad llega a 0, evalúa el estado actual del tablero.
    if (depth == 0)
        return b->evaluate(); // Evalúa y devuelve la puntuación del tablero actual.

    list<move*>::iterator iter = b->mlist.begin(); // Inicializa un iterador para la lista de movimientos.

    int localalpha = std::numeric_limits<int>::min(); // Inicializa el valor local de alpha.
    int localbeta = std::numeric_limits<int>::max(); // Inicializa el valor local de beta.

    if (b->getTurn() == 'b') // Si es el turno del jugador 'b'.
    {
        for (; iter != b->mlist.end(); ++iter)
        {
            time(&endTime);

            if (difftime(endTime, startTime) >= (board::timeLimit - 1))
            {
                timeUp = true; // Se ha agotado el tiempo de cálculo.
                break;
            }

            b->makeMove(*iter); // Realiza el movimiento en el tablero.
            sptr<board> newB(new board(*b)); // Crea una copia del tablero actual.
            int value = alphabeta(newB, depth - 1, alpha, min(localbeta, beta)); // Llama recursivamente a Alpha-Beta.
            b->undoMove(*iter); // Deshace el movimiento en el tablero.
            b->changeTurn(); // Cambia el turno al siguiente jugador.

            if (value > alpha)
            {
                alpha = value; // Actualiza el valor de alpha.
                if (depth == maxdepth)
                    tempBestM = (*iter); // Almacena el mejor movimiento en la profundidad máxima.
            }

            if (alpha >= beta && depth < maxdepth)
                return alpha; // Realiza la poda alpha-beta.
        }

        if (!timeUp && depth == maxdepth)
            cdepth = depth; // Actualiza la profundidad actual.

        return alpha; // Retorna el valor de alpha.
    }
    else // Si es el turno del jugador 'r'.
    {
        for (; iter != b->mlist.end(); ++iter)
        {
            time(&endTime);
            if (difftime(endTime, startTime) >= (board::timeLimit - 1))
            {
                timeUp = true; // Se ha agotado el tiempo de cálculo.
                break;
            }
            b->makeMove(*iter); // Realiza el movimiento en el tablero.
            sptr<board> newB(new board(*b)); // Crea una copia del tablero actual.
            int value = alphabeta(newB, depth - 1, max(localalpha, alpha), beta); // Llama recursivamente a Alpha-Beta.
            b->undoMove(*iter); // Deshace el movimiento en el tablero.
            b->changeTurn(); // Cambia el turno al siguiente jugador.

            if (value < beta)
            {
                beta = value; // Actualiza el valor de beta.
                if (depth == maxdepth)
                    tempBestM = (*iter); // Almacena el mejor movimiento en la profundidad máxima.
            }

            if (alpha >= beta)
                return beta; // Realiza la poda alpha-beta.
        }

        if (!timeUp && depth == maxdepth)
            cdepth = depth; // Actualiza la profundidad actual.

        return beta; // Retorna el valor de beta.
    }
}



