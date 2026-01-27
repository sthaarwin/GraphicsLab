#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Scaling parameters
float scaleX = 1.0f;
float scaleY = 1.0f;
float scaleZ = 1.0f;
float uniformScale = 1.0f;

void drawCube() {
    glBegin(GL_QUADS);
    
    // Front face (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    
    // Back face (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Top face (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Bottom face (Yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Right face (Magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    
    // Left face (Cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    
    glEnd();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        const float step = 0.1f;
        switch (key) {
            case GLFW_KEY_X: scaleX += step; break;  // Scale X axis
            case GLFW_KEY_C: scaleX = std::max(0.1f, scaleX - step); break;
            case GLFW_KEY_Y: scaleY += step; break;  // Scale Y axis
            case GLFW_KEY_H: scaleY = std::max(0.1f, scaleY - step); break;
            case GLFW_KEY_Z: scaleZ += step; break;  // Scale Z axis
            case GLFW_KEY_V: scaleZ = std::max(0.1f, scaleZ - step); break;
            case GLFW_KEY_U: // Uniform scale up
                uniformScale += step;
                scaleX = scaleY = scaleZ = uniformScale;
                break;
            case GLFW_KEY_D: // Uniform scale down
                uniformScale = std::max(0.1f, uniformScale - step);
                scaleX = scaleY = scaleZ = uniformScale;
                break;
            case GLFW_KEY_R: // Reset
                scaleX = scaleY = scaleZ = uniformScale = 1.0f;
                std::cout << "Scaling reset" << std::endl;
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
        std::cout << "Scale: X=" << scaleX << " Y=" << scaleY << " Z=" << scaleZ << std::endl;
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 5 Q1d - 3D Scaling", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::cout << "Controls:" << std::endl;
    std::cout << "  X/C - Increase/Decrease Scale on X-axis" << std::endl;
    std::cout << "  Y/H - Increase/Decrease Scale on Y-axis" << std::endl;
    std::cout << "  Z/V - Increase/Decrease Scale on Z-axis" << std::endl;
    std::cout << "  U/D - Uniform scale Up/Down" << std::endl;
    std::cout << "  R - Reset scaling" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspect = 800.0f / 600.0f;
        glFrustum(-aspect, aspect, -1.0, 1.0, 2.0, 20.0);

        // Set up modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Move camera back
        glTranslatef(0.0f, 0.0f, -6.0f);

        // Static rotation for better view
        glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(30.0f, 0.0f, 1.0f, 0.0f);

        // Apply scaling transformation
        glScalef(scaleX, scaleY, scaleZ);

        // Draw the cube
        drawCube();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
