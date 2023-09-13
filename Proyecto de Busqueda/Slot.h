#ifndef SLOT_H
#define SLOT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
//Definimos la estructura Slot
struct Slot
{
    // to draw
    //vector paraalmacenar los vertices del cada casilla
    std::vector<float> vertices;
    //inst como dibujar la casilla / dibujar triangulos GL_TRIANGLES
    GLenum primitive;
    //definir vector en que orden se conectan los vértices para dibujar triangulos
    std::vector<unsigned int> indices;
    std::vector<unsigned int> indicesF;//lineas
    glm::vec4 color; // color delacasilla
    glm::vec4 colorF; //color del contorno de la casilla
    
    Slot() {}
    //definimos la casilla , cuadr,color y contorno.
    Slot(float x, float y, float z, float length) {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);

        vertices.push_back(x+length);
        vertices.push_back(y);
        vertices.push_back(z);

        vertices.push_back(x + length);
        vertices.push_back(y + length);
        vertices.push_back(z);

        vertices.push_back(x);
        vertices.push_back(y+length);
        vertices.push_back(z);

        color = glm::vec4(255.0f, 255.0f,255.0f, 1.0f);
        //def 310 p.triang 321 seg
        indices = { 3,1,0,3,2,1 };
        //define color
        colorF = glm::vec4(0.0f, 0.0f,0.0f, 1.0f);
        //defi list indices para determinar el orden que se debe conectar los vertices
        //para formar las lineas.
        indicesF = { 0,1,1,2,2,3,3,0};
    }
    ~Slot() {}

    //visualizar el objeto Slot en la ventana
    void draw()
    {
        //envia coordenadas de los vertices al GPU para utilizar en la visual
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        //determinar en qué orden se debe conectar los vertices paraformar triangulos
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    // visualizar
    void drawF()
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesF.size() * sizeof(unsigned int), &indicesF[0], GL_STATIC_DRAW);
        glDrawElements(GL_LINES, indicesF.size(), GL_UNSIGNED_INT, 0);
    }

    //definir color
    void update_color(glm::vec4 c) {
        color = c;
    }

   
};
#endif