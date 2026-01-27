#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Translation parameters
float translateX = 0.0f;
float translateY = 0.0f;
float translateZ = -5.0f;

void drawCube(float brightness) {
    glBegin(GL_QUADS);
    
    // Front face (Red)
    glColor3f(1.0f * brightness, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    
    // Back face (Green)
    glColor3f(0.0f, 1.0f * brightness, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Top face (Blue)
    glColor3f(0.0f, 0.0f, 1.0f * brightness);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Bottom face (Yellow)
    glColor3f(1.0f * brightness, 1.0f * brightness, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Right face (Magenta)
    glColor3f(1.0f * brightness, 0.0f, 1.0f * brightness);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    
    // Left face (Cyan)
    glColor3f(0.0f, 1.0f * brightness, 1.0f * brightness);
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
            case GLFW_KEY_W: translateY += step; break;  // Move up
            case GLFW_KEY_S: translateY -= step; break;  // Move down
            case GLFW_KEY_A: translateX -= step; break;  // Move left
            case GLFW_KEY_D: translateX += step; break;  // Move right
            case GLFW_KEY_Q: translateZ += step; break;  // Move closer
            case GLFW_KEY_E: translateZ -= step; break;  // Move farther
            case GLFW_KEY_R: // Reset
                translateX = 0.0f;
                translateY = 0.0f;
                translateZ = -5.0f;
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 5 Q1a - 3D Translation", NULL, NULL);
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
    std::cout << "  W/S - Move Up/Down" << std::endl;
    std::cout << "  A/D - Move Left/Right" << std::endl;
    std::cout << "  Q/E - Move Closer/Farther" << std::endl;
    std::cout << "  R - Reset position" << std::endl;
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

        // Draw the original cube at initial position (dimmed)
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -5.0f);
        glRotatef(25.0f, 1.0f, 0.0f, 0.0f);  // Rotate for 3D effect
        glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
        drawCube(0.4f);  // Dimmed to show it's the original
        glPopMatrix();

        // Draw the translated cube
        glPushMatrix();
        glTranslatef(translateX, translateY, translateZ);
        glRotatef(25.0f, 1.0f, 0.0f, 0.0f);  // Rotate for 3D effect
        glRotatef(30.0f, 0.0f, 1.0f, 0.0f);
        drawCube(1.0f);  // Full brightness
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
