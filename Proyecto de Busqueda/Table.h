#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Slot.h"
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

struct Table
{   
    //tabla compuesta por una matriz de objetos 

    Slot** slots;//puntero a un puntero que apunta a una matriz de objetos Slot
    
    Table(float x, float y, float z, float length) {
        slots = new Slot*[100];//reserva dinarreglo de 100 punteros 
        for (int i = 0; i < 100;i++) { //filas
            slots[i] = new Slot[100];
            float aux = x;
            for (int j = 0; j < 100;j++) { //recorre columnas de la tabla
                slots[i][j] = Slot(aux, y, z, length);
                aux += length;
            }
            y -= length;
        }
    }
    ~Table() {}

   //dibuja todos los Slot de la tabla en OpenGL

    void draw()
    {
        for (int i = 0; i < 100;i++) {
            for (int j = 0; j < 100;j++) {
                slots[i][j].draw();
            }
        }
    }
    //void update_color() {
       // color = glm::vec4(generate_random_float(), generate_random_float(), generate_random_float(), 1.0f);
    //}
  
    
};