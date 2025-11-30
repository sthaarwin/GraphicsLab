#include <GLFW/glfw3.h>
#include <iostream>

void drawLetterA(float x, float y, float scale=1.0f){
    glBegin(GL_LINES);
         // left diagonal
        glVertex2f(x, y);
        glVertex2f(x + 0.05f*scale, y + 0.1f*scale);

        // right diagonal
        glVertex2f(x + 0.05f*scale, y + 0.1f*scale);
        glVertex2f(x + 0.1f*scale, y);

        // crossbar
        glVertex2f(x + 0.02f*scale, y + 0.05f*scale);
        glVertex2f(x + 0.08f*scale, y + 0.05f*scale);
    glEnd();
}

void drawLetterR(float x, float y, float scale=1.0f) {
    glBegin(GL_LINES);
        // Left vertical
        glVertex2f(x, y);
        glVertex2f(x, y + 0.1f*scale);

        // Top horizontal
        glVertex2f(x, y + 0.1f*scale);
        glVertex2f(x + 0.07f*scale, y + 0.1f*scale);

        // Middle diagonal 
        glVertex2f(x + 0.07f*scale, y + 0.1f*scale);
        glVertex2f(x + 0.07f*scale, y + 0.05f*scale);

        // Middle horizontal
        glVertex2f(x, y + 0.05f*scale);
        glVertex2f(x + 0.07f*scale, y + 0.05f*scale);

        // Leg diagonal
        glVertex2f(x + 0.07f*scale, y);
        glVertex2f(x, y + 0.05f*scale);
    glEnd();
}

void drawLetterW(float x, float y, float scale=1.0f) {
    glBegin(GL_LINES);
        // Left vertical down to middle-low
        glVertex2f(x, y + 0.1f*scale);
        glVertex2f(x + 0.025f*scale, y);

        // First middle up
        glVertex2f(x + 0.025f*scale, y);
        glVertex2f(x + 0.05f*scale, y + 0.05f*scale);

        // Second middle down
        glVertex2f(x + 0.05f*scale, y + 0.05f*scale);
        glVertex2f(x + 0.075f*scale, y);

        // Right vertical up
        glVertex2f(x + 0.075f*scale, y);
        glVertex2f(x + 0.1f*scale, y + 0.1f*scale);
    glEnd();
}


void drawLetterI(float x, float y, float scale=1.0f) {
    glBegin(GL_LINES);
        // Top horizontal
        glVertex2f(x, y + 0.1f*scale);
        glVertex2f(x + 0.1f*scale, y + 0.1f*scale);

        // Center vertical
        glVertex2f(x + 0.05f*scale, y);
        glVertex2f(x + 0.05f*scale, y + 0.1f*scale);

        // Bottom horizontal
        glVertex2f(x, y);
        glVertex2f(x + 0.1f*scale, y);
    glEnd();
}

void drawLetterN(float x, float y, float scale=1.0f) {
    glBegin(GL_LINES);
        // Left vertical
        glVertex2f(x, y);
        glVertex2f(x, y + 0.1f*scale);

        // Diagonal middle
        glVertex2f(x, y + 0.1f*scale);
        glVertex2f(x + 0.1f*scale, y);

        // Right vertical
        glVertex2f(x + 0.1f*scale, y);
        glVertex2f(x + 0.1f*scale, y + 0.1f*scale);
    glEnd();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()){
        std::cout <<"glfw init failed"<<std::endl;
        return -1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab Works", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate(); return -1;
    }
    glfwMakeContextCurrent(window);

    float startX = -0.5f;
    float startY = 0.1f;
    float spacing = 0.12f; 
    while(!glfwWindowShouldClose(window)){
       
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawLetterA(startX, startY);
        drawLetterR(startX + spacing, startY);
        drawLetterW(startX + 2*spacing, startY);
        drawLetterI(startX + 3*spacing, startY);
        drawLetterN(startX + 4*spacing, startY);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

   
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
