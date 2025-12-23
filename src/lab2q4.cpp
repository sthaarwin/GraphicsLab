#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

void plotPixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

// DDA Line Drawing Algorithm
void drawLineDDA(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    
    float x = x1;
    float y = y1;
    
    for (int i = 0; i <= steps; i++) {
        plotPixel(round(x) / 400.0f - 1.0f, round(y) / 300.0f - 1.0f);
        x += xIncrement;
        y += yIncrement;
    }
}

// Bresenham Line Drawing Algorithm for |m| < 1
void bresenhamLow(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int yi = 1;
    
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    
    int D = 2 * dy - dx;
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

// Bresenham Line Drawing Algorithm for |m| >= 1
void bresenhamHigh(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int xi = 1;
    
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    
    int D = 2 * dx - dy;
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

// Bresenham Line Drawing Algorithm
void drawLineBresenham(int x1, int y1, int x2, int y2) {
    if (abs(y2 - y1) < abs(x2 - x1)) {
        if (x1 > x2) {
            bresenhamLow(x2, y2, x1, y1);
        } else {
            bresenhamLow(x1, y1, x2, y2);
        }
    } else {
        if (y1 > y2) {
            bresenhamHigh(x2, y2, x1, y1);
        } else {
            bresenhamHigh(x1, y1, x2, y2);
        }
    }
}

// Draw line using selected algorithm (true = DDA, false = Bresenham)
void drawLine(float x1, float y1, float x2, float y2, bool useDDA = true) {
    int X1 = (x1 + 1.0f) * 400;
    int Y1 = (y1 + 1.0f) * 300;
    int X2 = (x2 + 1.0f) * 400;
    int Y2 = (y2 + 1.0f) * 300;
    
    if (useDDA) {
        drawLineDDA(X1, Y1, X2, Y2);
    } else {
        drawLineBresenham(X1, Y1, X2, Y2);
    }
}

// Structure to hold data points
struct DataPoint {
    float x;
    float y;
};

// Draw axes
void drawAxes() {
    glColor3f(0.5f, 0.5f, 0.5f);
    
    // X-axis
    drawLine(-0.9f, 0.0f, 0.9f, 0.0f, true);
    
    // Y-axis
    drawLine(0.0f, -0.9f, 0.0f, 0.9f, true);
    
    // Grid lines
    glColor3f(0.3f, 0.3f, 0.3f);
    for (float i = -0.8f; i <= 0.8f; i += 0.2f) {
        if (abs(i) > 0.01f) {
            // Vertical grid lines
            drawLine(i, -0.9f, i, 0.9f, true);
            // Horizontal grid lines
            drawLine(-0.9f, i, 0.9f, i, true);
        }
    }
}

// Draw line graph from data points
void drawLineGraph(const std::vector<DataPoint>& data, bool useDDA = true) {
    if (data.size() < 2) return;
    
    // Draw lines connecting consecutive points
    for (size_t i = 0; i < data.size() - 1; i++) {
        drawLine(data[i].x, data[i].y, data[i + 1].x, data[i + 1].y, useDDA);
    }
    
    // Draw data points as larger dots
    glPointSize(6.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (const auto& point : data) {
        plotPixel(point.x, point.y);
    }
    glPointSize(2.0f);
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 2 Q4 - Line Graph with DDA/Bresenham", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glPointSize(2.0f);
    
    // Sample data set 1: Temperature over time
    std::vector<DataPoint> dataset1 = {
        {-0.8f, -0.4f},
        {-0.6f, -0.3f},
        {-0.4f, -0.1f},
        {-0.2f, 0.2f},
        {0.0f, 0.3f},
        {0.2f, 0.5f},
        {0.4f, 0.4f},
        {0.6f, 0.6f},
        {0.8f, 0.7f}
    };
    
    // Sample data set 2: Sine wave approximation
    std::vector<DataPoint> dataset2;
    for (float x = -0.8f; x <= 0.8f; x += 0.1f) {
        dataset2.push_back({x, sin(x * 3.14159f) * 0.5f});
    }
    
    bool useDDA = true;
    int datasetChoice = 1;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw axes and grid
        drawAxes();
        
        // Draw line graph
        glColor3f(0.0f, 1.0f, 0.0f);
        if (datasetChoice == 1) {
            drawLineGraph(dataset1, useDDA);
        } else {
            drawLineGraph(dataset2, useDDA);
        }
        
        // Display algorithm and dataset info
        std::string title = "Lab 2 Q4 - Line Graph | Algorithm: ";
        title += (useDDA ? "DDA" : "Bresenham");
        title += " | Dataset: ";
        title += (datasetChoice == 1 ? "Temperature" : "Sine Wave");
        title += " | Press SPACE to toggle algorithm, D to toggle dataset";
        glfwSetWindowTitle(window, title.c_str());

        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Simple keyboard input handling
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            useDDA = !useDDA;
            glfwWaitEventsTimeout(0.2); // Debounce
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            datasetChoice = (datasetChoice == 1) ? 2 : 1;
            glfwWaitEventsTimeout(0.2); // Debounce
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
