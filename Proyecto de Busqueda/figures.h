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

float norm = 0;
float speed = 0.0005;
float origin_speed = 0.0005;
float error = 0.001;

float generate_random_float(float down = -1.0f, float up = 1.0f)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distribution(down, up);
    return distribution(generator);
}
int generate_random_int(int down = 0, int up = 10)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(down, up);
    return distribution(generator);
}
bool between(glm::vec3 a, glm::vec3 b, float error = 0.01)
{
    if (glm::abs(b.x - a.x) <= error && glm::abs(b.y - a.y) <= error)
        return true;
    return false;
}

std::vector<float> generate_cicle_vertices(float radius = 0.5f, int amount_vertices = 36)
{
    // set variables
    std::vector<float> circle_vertices; // output
    float angle = 0; // turning angle
    float angle_increment = glm::radians(360.0f / amount_vertices); // angle between vertices // grados * PI / 180 -> radians;
    float x = radius, y = 0.0f, z = 0.0f;
    //glm::vec2 pivot_point(radius, 0); 
    glm::mat2x2 R; // rotation matrix
    // push origin
    circle_vertices.push_back(0.0f);
    circle_vertices.push_back(0.0f);
    circle_vertices.push_back(0.0f);
    // make circle
    for (int i = 0; i < amount_vertices; i++)
    {
        // updating matrix with new angle
        R = glm::mat2x2(glm::cos(angle), -glm::sin(angle), glm::sin(angle), glm::cos(angle));
        // push new vertex
        circle_vertices.push_back(x * R[0][0] + y * R[0][1]);
        circle_vertices.push_back(x * R[1][0] + y * R[1][1]);
        circle_vertices.push_back(z);
        // increment angle
        angle += angle_increment;
    }
    return circle_vertices;
}
std::vector<float> generate_spiral_vertices(float angle_increment = 5.0f, int amount_vertices = 36)
{
    // set variables
    std::vector<float> spiral_vertices; // output
    float angle = 0; // turning angle
    //float angle_increment = glm::radians(360.0f / amount_vertices); // angle between vertices // grados * PI / 180 -> radians;
    float x = 0.0f, y = 0.0f, z = 0.0f, ro = 0.0f;

    for (int i = 0; i < amount_vertices; ++i)
    {
        x = ro * glm::cos(angle);
        y = ro * glm::sin(angle);

        spiral_vertices.push_back(x);
        spiral_vertices.push_back(y);
        spiral_vertices.push_back(z);

        angle += angle_increment;
        ro = angle / 10;
    }

    return spiral_vertices;
}
std::vector<float> generate_star_vertices(int corners, float longinus)
{
    std::vector<float> star_vertices = generate_cicle_vertices(0.2f, corners);
    float x, y;
    glm::vec2 o, d;

    for (int i = 3, j = 6, s = star_vertices.size(); i < s; i += 3, j += 3)
    {
        d = glm::vec2((star_vertices[i] + star_vertices[j]) / 2, (star_vertices[i + 1] + star_vertices[j + 1]) / 2);
        o = glm::vec2(star_vertices[j + 1] - star_vertices[i + 1], -(star_vertices[j] - star_vertices[i]));
        d += o * longinus;

        star_vertices.push_back(d.x);
        star_vertices.push_back(d.y);
        star_vertices.push_back(0.0f);
        //std::cout << i << " " << j << std::endl;
        if (j + 3 >= s)
            j = 0;
    }
    return star_vertices;
}
std::vector<float> generate_heart_vertices(float a)
{
    std::vector<float> heart_vertices;
    float x = -0.95f, y;
    float increment = 0.001f;
    
    while (x < 0.93f)
    {
        x += increment;
        if (x < 0)
            y = glm::pow(-x, 0.4f) + 0.7f * glm::sqrt(0.9f - glm::pow(x, 2)) * glm::sin(a * 3.14159f * x) - 0.4f;
        else    
            y = glm::pow(x, 0.4f) + 0.7f * glm::sqrt(0.9f - glm::pow(x, 2)) * glm::sin(a * 3.14159f * x) - 0.4f;
        heart_vertices.push_back(x);
        heart_vertices.push_back(y);
        heart_vertices.push_back(0.0f);
    }
    return heart_vertices;
}

struct Figure
{
    // to draw
    std::vector<float> vertices;
    GLenum primitive;
    std::vector<unsigned int> indices;
    glm::vec4 color;
    std::vector<float> path_vertices;
    std::vector<unsigned int> path_indices;

    // to animate
    int index_patron = 0;
    std::vector<float> patron;
    glm::vec3 direction_vector;
    glm::vec3 unit_direction_vector;
    glm::vec3 pivot_point;

    Figure() {}
    ~Figure() {}

    void follow_patron()
    {
        if (!between(pivot_point, glm::vec3(patron[index_patron], patron[index_patron + 1], patron[index_patron + 2])))
        {
            norm = glm::sqrt(glm::pow(direction_vector.x, 2) + glm::pow(direction_vector.y, 2));
            glm::vec3 unit_direction_vector = (direction_vector / norm) * speed;
            pivot_point += unit_direction_vector;
            update_vertices(unit_direction_vector);
            path_vertices.push_back(pivot_point.x);
            path_vertices.push_back(pivot_point.y);
            path_vertices.push_back(pivot_point.z);
            path_indices.push_back(path_indices.size());
        }
        else
        {
            index_patron = (index_patron + 3) % patron.size();
            direction_vector = glm::vec3(patron[index_patron], patron[index_patron + 1], patron[index_patron + 2]) - pivot_point;
        }
    }
    void draw_figure()
    {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glDrawElements(primitive, indices.size(), GL_UNSIGNED_INT, 0);
    }
    void draw_path()
    {
        glBufferData(GL_ARRAY_BUFFER, path_vertices.size() * sizeof(float), &path_vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, path_indices.size() * sizeof(unsigned int), &path_indices[0], GL_STATIC_DRAW);
        glDrawElements(GL_POINTS, path_indices.size(), GL_UNSIGNED_INT, 0);
    }
    void update_color() {
        color = glm::vec4(generate_random_float(), generate_random_float(), generate_random_float(), 1.0f);
    }
    void update_patron(std::string p, float aux = 0.5f, int n = 20)
    {
        if (p == "square")
            patron = generate_cicle_vertices(aux, 4);
        else if (p == "triangle")
            patron = generate_cicle_vertices(aux, 3);
        else if (p == "pentagon")
            patron = generate_cicle_vertices(aux, 5);
        else if (p == "circle")
            patron = generate_cicle_vertices();
        else if (p == "spiral")
            patron = generate_spiral_vertices(aux, n);
        patron.erase(patron.begin(), patron.begin() + 3);
        direction_vector = glm::vec3(patron[0], patron[1], patron[2]) - pivot_point;
    }
    void update_vertices(glm::vec3 direction)
    {
        for (int i = 0, s = vertices.size(); i < s; i += 3)
        {
            vertices[i] += direction.x;
            vertices[i + 1] += direction.y;
            vertices[i + 2] += direction.z;
        }
    }
    virtual void update_primitive(GLenum p)
    {
        primitive = p;
        int size = vertices.size() / 3;
        indices.clear();
        if (p == GL_POINTS)
        {
            for (int i = 1; i < size; ++i)
            {
                indices.push_back(i);
            }
        }
        else if (p == GL_LINES)
        {
            int i = 1, s = size - 1;
            for (; i < s; ++i)
            {
                indices.push_back(i);
                indices.push_back(i + 1);
            }
            indices.push_back(i);
            indices.push_back(1);
        }
        else if (p == GL_LINE_LOOP)
        {
            for (int i = 1, s = size - 1; i < s; ++i)
            {
                indices.push_back(i);
                indices.push_back(i + 1);
            }
        }
        else if (p == GL_TRIANGLES)
        {
            for (int i = 1, s = size - 1; i < s; ++i)
            {
                indices.push_back(i);
                indices.push_back(0);
                indices.push_back(i + 1);
            }
            indices.push_back(size - 1);
            indices.push_back(0);
            indices.push_back(1);
        }
    }
};
struct Triangle : Figure
{
    Triangle()
    {
        vertices = generate_cicle_vertices(0.2f, 3);
        update_primitive(GL_TRIANGLES);
    }
    Triangle(GLenum p)
    {
        vertices = generate_cicle_vertices(0.2f, 3);
        update_primitive(p);
    }
};
struct Square : Figure
{
    Square()
    {
        vertices = generate_cicle_vertices(0.2f, 4);
        update_primitive(GL_TRIANGLES);
    }
    Square(GLenum p)
    {
        vertices = generate_cicle_vertices(0.2f, 4);
        update_primitive(p);
    }
};
struct Pentagon : Figure
{
    Pentagon()
    {
        vertices = generate_cicle_vertices(0.2f, 5);
        update_primitive(GL_TRIANGLES);
    }
    Pentagon(GLenum p)
    {
        vertices = generate_cicle_vertices(0.2f, 5);
        update_primitive(p);
    }
};
struct Star : Figure
{
    int corners;
    Star()
    {
        corners = 5;
        vertices = generate_star_vertices(5, 1);
        update_primitive(GL_TRIANGLES);
    }
    Star(int c, GLenum p, float l = 1)
    {
        corners = c;
        vertices = generate_star_vertices(c, l);
        update_primitive(p);
    }
    void update_primitive(GLenum p)
    {
        primitive = p;
        indices.clear();
        if (p == GL_POINTS)
        {
            for (int i = 1, s = vertices.size() / 3; i < s; ++i)
            {
                indices.push_back(i);
            }
        }
        else if (p == GL_LINES)
        {
            int i = 1, s = corners;
            for (; i < s; ++i)
            {
                indices.push_back(i);
                indices.push_back(i + corners);
                indices.push_back(i + corners);
                indices.push_back(i + 1);
            }
            indices.push_back(i);
            indices.push_back(i + corners);
            indices.push_back(i + corners);
            indices.push_back(1);
        }
        else if (p == GL_TRIANGLES)
        {
            for (int i = 1, s = corners; i < s; ++i)
            {
                indices.push_back(i);
                indices.push_back(0);
                indices.push_back(i + 1);
            }
            indices.push_back(corners);
            indices.push_back(0);
            indices.push_back(1);

            int i = 1, s = corners;
            for (; i < s; ++i)
            {
                indices.push_back(i);
                indices.push_back(i + corners);
                indices.push_back(i + 1);
            }
            indices.push_back(i);
            indices.push_back(i + corners);
            indices.push_back(1);
        }
    }
};
struct Heart : Figure
{
    Heart(int a, GLenum p)
    {
        vertices = generate_heart_vertices(a);
        update_primitive(p);
    }
    void update_primitive(GLenum p)
    {
        primitive = p;
        indices.clear();
        if (p == GL_POINTS)
        {
            for (int i = 0, s = vertices.size() / 3; i < s; ++i)
            {
                indices.push_back(i);
            }
        }
        if (p == GL_LINES)
        {
            for (int i = 0, s = vertices.size() / 3 - 1; i < s; ++i)
            {
                indices.push_back(i);
                indices.push_back(i + 1);
            }
        }
    }
};
