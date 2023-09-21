#ifndef tablero_h
#define tablero_h
//Librerias necesariaspara el funcionamiento delcodigo
#include <stdio.h>
#include <cctype>
#include <fstream>
#include <iostream>
#include <list> //listas enlazadas
#include <string>//manipulacion de cadenas

//Declaramos una clase Jump que representara un salto en eljuego
class jump
{
    jump* prev; //Puntero al salto anterior
    char jumpingPiece; //Caracter para representar la pienza que esta saltando
    bool noNext;// Bool que indica si no hay más saltos despues de este.
    int numTimes; //Nro de veces que ha realizado el salto
    char c;
    int xs, ys; //Coordenadas iniciales
    int x, y;//Coordenadas intermedia durante el salto
    int xend, yend; //Coordernadas finales despues del salto
    
    int key; //clave
    //Constructor
    jump(char jpingp, char piece, int xs, int ys, int xc, int yc, int xe, int ye, jump* p, int k) :
        prev(p), jumpingPiece(jpingp), noNext(true), numTimes(0), c(piece), xs(xs), ys(ys),
        x(xc), y(yc), xend(xe), yend(ye), key(k) {}

    // Clases amigas : Move y Board pueden acceder a los miembros privados de clase Jump.
    friend class move;
    friend class board;
};
//Declaracion de Clase Move que representará los movimiento en eljuego.
class move
{
    char mP; //Pieza que se está moviendo
    int xi, yi; // Coordenadas iniciales del movimiento.
    int xf, yf;// Coordenadas finales del movimiento
    std::string command;
    //Lista de puntero a objetos 'Jump' que representa los saltos 
    std::list<jump*> jpoints;
    //Constructor move()
    move(char c, int xs, int ys, int xe, int ye) : mP(c), xi(xs), yi(ys), xf(xe), yf(ye) {}
    //Destructor
    ~move();
    //Clases amigas
    friend class board;
    friend class game;
};
//Declaracion de clase Board que representara al tablero
class board
{
    char arr[8][4];//Matriz para estado actual del tablero
    char color; //Indica el color del jugado actual rojo o negro
    static bool isComputer[2]; // Boolean para inidicar si los mov son de la computadora o usuario
    //Constructor
    board();
    //Destructor
    ~board();
    board(const board&);

    void changeTurn()
    {
        if (color == 'r')
            color = 'b';
        else
            color = 'r';
    }

    static void convertCommand(const std::string&);

    bool terminalTest()
    {
        if (!movesAvailable())
            return true;
        return false;
    }
    bool movesAvailable()
    {
        if (jumpsAvailable())
            return true;
        if (listMoves())
            return true;
        return false;
    }
    void handleKinging(const int& x, const int& y)
    {
        if (x == 0 && arr[x][y] == 'r')
            arr[x][y] = 'R';
        if (x == 7 && arr[x][y] == 'b')
            arr[x][y] = 'B';
    }
    bool isValidPos(int i, int j)
    {
        if (i >= 0 && i < 8 && j >= 0 && j < 4)
            return true;
        else return false;
    }
    void reset();
    void modifyBoard(std::ifstream&);
    void remove_carriage_return(std::string&);
    int createkey(int, int, int, int, int, int);
    int reverse(int);
    void createJump(std::list<jump*>&, char, int, int, int, int, int, int, jump*);
    void createJumpMove(std::list<jump*>&);
    void jumpAvailable(std::list<jump*>&, char c, int, int, jump*);
    bool jumpsAvailable();
    bool jumpConditions(int, int, int, int);
    void checkNeighbors(int&, int&);
    void createMove(const int&, const int&, int, int);
    bool listMoves();
    void convert(const int&, const int&, std::string&);
    int convertY(const int& x, const int& y);
    void printline(const int&, const std::string&, const std::string&);
    void printcolor(const char&);
    static void whoComputer();
    static int timeLimit; //limite de tiempo para las jugadas
    std::list<move*> mlist;//Lista de punteros a obj 'move' que almacena movim del juego
    bool isComputerTurn()
    {
        if (color == 'b' && isComputer[0])
            return true;
        if (color == 'r' && isComputer[1])
            return true;
        return false;
    }
    void inputCommand();
    void printBoard();
    void printMoves();
    void makeMove(move*);
    void undoMove(move*);
    int cornerDiagonal(char, char);
    int evaluate();
    void startup();
    char getTurn()
    {
        return color;
    }
    friend class game;
    template <class T> friend class sptr;
};


#endif /* board_hpp */
