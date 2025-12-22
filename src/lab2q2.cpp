#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void plotPixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

// Bresenham Line Drawing Algorithm for |m| < 1 (gentle slope)
void bresenhamLow(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int yi = 1;
    
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    
    int D = 2 * dy - dx;  // Decision parameter
    int y = y1;
    
    for (int x = x1; x <= x2; x++) {
        plotPixel(x / 400.0f - 1.0f, y / 300.0f - 1.0f);
        
        if (D > 0) {
            y += yi;
            D += 2 * (dy - dx);
        } else {
            D += 2 * dy;
        }
    }
}

// Bresenham Line Drawing Algorithm for |m| >= 1 (steep slope)
void bresenhamHigh(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int xi = 1;
    
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    
    int D = 2 * dx - dy;  // Decision parameter
    int x = x1;
    
    for (int y = y1; y <= y2; y++) {
        plotPixel(x / 400.0f - 1.0f, y / 300.0f - 1.0f);
        
        if (D > 0) {
            x += xi;
            D += 2 * (dx - dy);
        } else {
            D += 2 * dx;
        }
    }
}

// Bresenham Line Drawing Algorithm - Main function
void drawLineBresenham(int x1, int y1, int x2, int y2) {
    // Handle all octants by choosing the appropriate algorithm
    if (abs(y2 - y1) < abs(x2 - x1)) {
        // Gentle slope: |m| < 1
        if (x1 > x2) {
            bresenhamLow(x2, y2, x1, y1);
        } else {
            bresenhamLow(x1, y1, x2, y2);
        }
    } else {
        // Steep slope: |m| >= 1
        if (y1 > y2) {
            bresenhamHigh(x2, y2, x1, y1);
        } else {
            bresenhamHigh(x1, y1, x2, y2);
        }
    }
}

// Helper function to convert OpenGL coordinates to screen coordinates
void drawLine(float x1, float y1, float x2, float y2) {
    // Convert OpenGL coords (-1 to 1) to screen pixels
    int X1 = (x1 + 1.0f) * 400;
    int Y1 = (y1 + 1.0f) * 300;
    int X2 = (x2 + 1.0f) * 400;
    int Y2 = (y2 + 1.0f) * 300;
    
    drawLineBresenham(X1, Y1, X2, Y2);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()){
        std::cout <<"glfw init failed"<<std::endl;
        return -1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 2 Q2 - Bresenham Line Algorithm", NULL, NULL);
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
        
        // Draw various lines to demonstrate Bresenham algorithm
        // Lines with |m| < 1 (gentle slope)
        drawLine(-0.8f, 0.5f, 0.8f, 0.5f);      // Horizontal line (m = 0)
        drawLine(-0.8f, -0.3f, 0.8f, 0.3f);     // Gentle positive slope
        drawLine(-0.8f, 0.3f, 0.8f, -0.3f);     // Gentle negative slope
        
        // Lines with |m| >= 1 (steep slope)
        drawLine(0.0f, -0.8f, 0.0f, 0.8f);      // Vertical line (m = infinity)
        drawLine(-0.3f, -0.8f, 0.3f, 0.8f);     // Steep positive slope
        drawLine(0.3f, -0.8f, -0.3f, 0.8f);     // Steep negative slope
        
        // Diagonal line (m = 1, boundary case)
        drawLine(-0.6f, -0.6f, 0.6f, 0.6f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

   
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
