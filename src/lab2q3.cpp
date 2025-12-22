#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void plotPixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

// Plot 8 symmetric points of a circle
void plotCirclePoints(int xc, int yc, int x, int y) {
    // Convert screen coordinates to OpenGL coordinates and plot
    plotPixel((xc + x) / 400.0f - 1.0f, (yc + y) / 300.0f - 1.0f);
    plotPixel((xc - x) / 400.0f - 1.0f, (yc + y) / 300.0f - 1.0f);
    plotPixel((xc + x) / 400.0f - 1.0f, (yc - y) / 300.0f - 1.0f);
    plotPixel((xc - x) / 400.0f - 1.0f, (yc - y) / 300.0f - 1.0f);
    plotPixel((xc + y) / 400.0f - 1.0f, (yc + x) / 300.0f - 1.0f);
    plotPixel((xc - y) / 400.0f - 1.0f, (yc + x) / 300.0f - 1.0f);
    plotPixel((xc + y) / 400.0f - 1.0f, (yc - x) / 300.0f - 1.0f);
    plotPixel((xc - y) / 400.0f - 1.0f, (yc - x) / 300.0f - 1.0f);
}

// Midpoint Circle Drawing Algorithm
void drawCircleMidpoint(int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    
    // Initial decision parameter
    int d = 1 - radius;
    
    // Plot initial point
    plotCirclePoints(xc, yc, x, y);
    
    // Generate points for one octant
    while (x < y) {
        x++;
        
        // Mid-point is inside or on the perimeter
        if (d < 0) {
            d = d + 2 * x + 1;
        }
        // Mid-point is outside the perimeter
        else {
            y--;
            d = d + 2 * (x - y) + 1;
        }
        
        // Plot 8 symmetric points
        plotCirclePoints(xc, yc, x, y);
    }
}

// Helper function to convert OpenGL coordinates to screen coordinates
void drawCircle(float xc, float yc, float r) {
    // Convert OpenGL coords (-1 to 1) to screen pixels
    int X = (xc + 1.0f) * 400;
    int Y = (yc + 1.0f) * 300;
    int R = r * 200;  // Scale radius
    
    drawCircleMidpoint(X, Y, R);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 2 Q3 - Midpoint Circle Algorithm", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Set point size for better visibility
    glPointSize(2.0f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);
        
        // Draw circles of different sizes to demonstrate the algorithm
        drawCircle(0.0f, 0.0f, 0.6f);      // Large circle at center
        drawCircle(-0.5f, 0.4f, 0.2f);     // Small circle top-left
        drawCircle(0.5f, 0.4f, 0.2f);      // Small circle top-right
        drawCircle(0.0f, -0.5f, 0.3f);     // Medium circle bottom

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
