#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void plotPixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

// Digital Differential Analyzer (DDA) Line Drawing Algorithm
void drawLineDDA(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    
    // Calculate the number of steps required for generating pixels
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    // Calculate increment in x & y for each step
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    
    // Put pixel for each step
    float x = x1;
    float y = y1;
    
    for (int i = 0; i <= steps; i++) {
        plotPixel(round(x) / 400.0f - 1.0f, round(y) / 300.0f - 1.0f);
        x += xIncrement;
        y += yIncrement;
    }
}

// Helper function to convert OpenGL coordinates to screen coordinates
void drawLine(float x1, float y1, float x2, float y2) {
    // Convert OpenGL coords (-1 to 1) to screen pixels
    float X1 = (x1 + 1.0f) * 400;
    float Y1 = (y1 + 1.0f) * 300;
    float X2 = (x2 + 1.0f) * 400;
    float Y2 = (y2 + 1.0f) * 300;
    
    drawLineDDA(X1, Y1, X2, Y2);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()){
        std::cout <<"glfw init failed"<<std::endl;
        return -1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 2 - DDA Line Algorithm", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate(); return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set point size for better visibility
    glPointSize(2.0f);

    while(!glfwWindowShouldClose(window)){
       
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);
        
        // Draw various lines to demonstrate DDA algorithm
        // Horizontal line
        drawLine(-0.8f, 0.5f, 0.8f, 0.5f);
        
        // Vertical line
        drawLine(0.0f, -0.8f, 0.0f, 0.8f);
        
        // Diagonal line (45 degrees)
        drawLine(-0.6f, -0.6f, 0.6f, 0.6f);
        
        // Line with different slope
        drawLine(-0.8f, -0.3f, 0.8f, 0.3f);
        
        // Steep line
        drawLine(-0.3f, -0.8f, 0.3f, 0.8f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

   
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
