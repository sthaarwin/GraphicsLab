#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

void plotPixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

c
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
