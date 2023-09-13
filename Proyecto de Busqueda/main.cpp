//lib complemento de opengl
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//lib math opengl
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "figures.h"
#include "Slot.h"
#include "Table.h"

#include <random>
#include <queue> //cola dfs
#include <utility> //first,second - agrupar var en 1
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <stack> 
//definimos el tam del tablero
#define L 100 
using namespace std;

//
typedef pair<int, int> pii;


char g[L][L];
//posibles movimientos 
const int dr[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
const int dc[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

struct Node {
    pii position;
    int costSoFar;
    int estimatedTotalCost;

    bool operator<(const Node& other) const {
        return estimatedTotalCost > other.estimatedTotalCost;
    }
};
int heuristic(const pii& a, const pii& b) {
    // Distancia euclidiana como heurística
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}
//yoo
int manhattanDistance(const pii& p1, const pii& p2) {
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}
//verificar si un mov es válido 0-99
bool isValid(int first, int second)
{
    return (first >= 0 && first < L && second >= 0 && second < L);
}
/*
vector<pii> findShortestPath(const char matrix[L][L], const pii& start, const pii& end) {
    vector<vector<int>> distance(L, vector<int>(L, INT_MAX));
    vector<vector<pii>> shortestPath(L, vector<pii>(L));
    queue<pii> q;
    q.push(start);
    distance[start.first][start.second] = 0;

    while (!q.empty())
    {
        pii current = q.front();
        q.pop();

        if (current.first == end.first && current.second == end.second)
        {
            vector<pii> path;
            pii pos = end;
            while (pos.first != start.first || pos.second != start.second)
            {
                path.push_back(pos);
                pos = shortestPath[pos.first][pos.second];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 8; i++)
        {
            int newRow = current.first + dr[i];
            int newCol = current.second + dc[i];

            if (isValid(newRow, newCol) && matrix[newRow][newCol] != '#' && distance[newRow][newCol] == INT_MAX)
            {
                distance[newRow][newCol] = distance[current.first][current.second] + 1;
                q.push({ newRow, newCol });
                shortestPath[newRow][newCol] = current;
            }
        }
    }

    return vector<pii>();
}
*/

//Algoritmos
vector<pii> findShortestPathDFS(const char matrix[100][100], const pii& start, const pii& end) {
    vector<vector<pii>> parent(L, vector<pii>(L, { -1, -1 }));
    stack<pii> s;
    s.push(start);

    while (!s.empty()) {
        pii current = s.top();
        s.pop();

        if (current.first == end.first && current.second == end.second) {
            vector<pii> path;
            pii pos = end;
            while (pos.first != start.first || pos.second != start.second) {
                path.push_back(pos);
                pos = parent[pos.first][pos.second];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 8; i++) {
            int newRow = current.first + dr[i];
            int newCol = current.second + dc[i];

            if (isValid(newRow, newCol) && matrix[newRow][newCol] != '#' && parent[newRow][newCol] == pii{-1, -1}) {
                s.push({ newRow, newCol });
                parent[newRow][newCol] = current;
            }
        }
    }

    return vector<pii>();
}
vector<pii> findShortestPathBFS(const char matrix[100][100], const pii& start, const pii& end) {
    vector<vector<pii>> parent(L, vector<pii>(L, { -1, -1 }));
    queue<pii> q;
    q.push(start);

    while (!q.empty()) {
        pii current = q.front();
        q.pop();

        if (current.first == end.first && current.second == end.second) {
            vector<pii> path;
            pii pos = end;
            while (pos.first != start.first || pos.second != start.second) {
                path.push_back(pos);
                pos = parent[pos.first][pos.second];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 8; i++) {
            int newRow = current.first + dr[i];
            int newCol = current.second + dc[i];

            if (isValid(newRow, newCol) && matrix[newRow][newCol] != '#' && parent[newRow][newCol] == pii{ -1, -1 }) {
                q.push({ newRow, newCol });
                parent[newRow][newCol] = current;
            }
        }
    }

    return vector<pii>();
}
vector<pii> findShortestPathAStar(const char matrix[100][100], const pii& start, const pii& end) {
    vector<vector<pii>> parent(L, vector<pii>(L, { -1, -1 }));
    vector<vector<int>> costSoFar(L, vector<int>(L, INT_MAX));

    priority_queue<Node> pq;
    pq.push({ start, 0, heuristic(start, end) });
    costSoFar[start.first][start.second] = 0;

    while (!pq.empty()) {
        Node currentNode = pq.top();
        pq.pop();
        pii currentPos = currentNode.position;

        if (currentPos == end) {
            vector<pii> path;
            pii pos = end;
            while (pos != start) {
                path.push_back(pos);
                pos = parent[pos.first][pos.second];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 8; i++) {
            int newRow = currentPos.first + dr[i];
            int newCol = currentPos.second + dc[i];

            if (isValid(newRow, newCol) && matrix[newRow][newCol] != '#') {
                int newCost = costSoFar[currentPos.first][currentPos.second] + 1;

                if (newCost < costSoFar[newRow][newCol]) {
                    costSoFar[newRow][newCol] = newCost;
                    int estimatedTotalCost = newCost + heuristic({ newRow, newCol }, end);
                    pq.push({ {newRow, newCol}, newCost, estimatedTotalCost });
                    parent[newRow][newCol] = currentPos;
                }
            }
        }
    }

    return vector<pii>();
}
vector<pii> findShortestPathHillClimbing(const char matrix[100][100], const pii& start, const pii& end) {
    vector<pii> path;
    pii current = start;

    while (current != end) {
        int minDistance = INT_MAX;
        vector<pii> possibleMoves;

        for (int i = 0; i < 8; i++) {
            int newRow = current.first + dr[i];
            int newCol = current.second + dc[i];

            if (isValid(newRow, newCol) && matrix[newRow][newCol] != '#') {
                int distance = abs(newRow - end.first) + abs(newCol - end.second);
                if (distance < minDistance) {
                    minDistance = distance;
                    possibleMoves.clear();
                    possibleMoves.push_back({ newRow, newCol });
                }
                else if (distance == minDistance) {
                    possibleMoves.push_back({ newRow, newCol });
                }
            }
        }

        if (possibleMoves.empty()) {
            // No hay movimientos válidos disponibles, no se encontró un camino
            return vector<pii>();
        }

        // Selecciona aleatoriamente uno de los movimientos posibles
        int randomIndex = rand() % possibleMoves.size();
        pii next = possibleMoves[randomIndex];

        path.push_back(next);
        current = next;
    }

    return path;
}

int generate_random(int min, int max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(min, max);
    return  distribution(generator);
}
// recorre un vector de pares de enteros y verifica 
//si un par de enteros específico está presente en el
bool inVector(vector<pii> &p , pii a) {
    for (auto& i : p) {
        if (i == a) {
            return true;
        }
    }
    return false;
}
//funcion elimina nodos aleatoriamente
void randomBarrier(Table& x , float p) {
    int amount = L * L * p;// cantidad total de obstaculos, multiplica tamaño
    //de cuadricula por la probabilidad
    vector<pii> position;
    for (int i = 0; i < amount; i++) {
        pii aux;
        do {
            aux = pii(generate_random(0, L-1), generate_random(0, L-1));

        } while (inVector(position,aux));
        position.push_back(aux);
    }
    for (auto& i : position) {
        x.slots[i.first][i.second].update_color(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        g[i.first][i.second] = '#';
    }
}
/*
void barrier(Table& x, int l, int b, int w)
{
    for (int i = b; i < b+l; i++) {
        x.slots[i][w].update_color(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        g[i][w] = '#';
    }
}*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"   FragColor = color;\n"
"}\n\0";

int main()
{
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // set variables
    
    //Slot x(0.0f, 0.0f, 0.0f, 0.2f);
    Table x(-0.95f, 0.95f, 0.0f, 0.019f);

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {

            g[i][j] = ' ';
        }
    }

    //barrier(x, 10, 0, 5);
    //barrier(x, 15, 5, 10);
    //barrier(x, 60, 25, 60);
    randomBarrier(x, 0.05f);
    //explicar pq lo hice con coordenadas : 
    //haz utilizado una libreria?
    //dime donde esta el algo

    //pii begin(70, 0);
    //pii end(70, 70);

    pii begin(70, 0);
    pii end(70, 70);

    vector<pii> shortestPath = findShortestPathHillClimbing(g, begin, end);

    for (auto& s : shortestPath) {
        x.slots[s.first][s.second].update_color(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
    }

    x.slots[begin.first][begin.second].update_color(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
    x.slots[end.first][end.second].update_color(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.7f, 0.69f, 0.69f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPointSize(5);
        glLineWidth(1);
        glUseProgram(shaderProgram);
        unsigned int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

       
        //x.draw();
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                glUniform4f(vertexColorLocation, x.slots[i][j].color[0], x.slots[i][j].color[1], x.slots[i][j].color[2], x.slots[i][j].color[3]);
                x.slots[i][j].draw();
                glUniform4f(vertexColorLocation, x.slots[i][j].colorF[0], x.slots[i][j].colorF[1], x.slots[i][j].colorF[2], x.slots[i][j].colorF[3]);
                x.slots[i][j].drawF();
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
    }
    
}