#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Rotation parameters
float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
bool autoRotate = false;

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
        const float step = 5.0f;
        switch (key) {
            case GLFW_KEY_X: angleX += step; break;  // Rotate around X-axis
            case GLFW_KEY_Y: angleY += step; break;  // Rotate around Y-axis
            case GLFW_KEY_Z: angleZ += step; break;  // Rotate around Z-axis
            case GLFW_KEY_SPACE:
                autoRotate = !autoRotate;
                std::cout << "Auto-rotation " << (autoRotate ? "ON" : "OFF") << std::endl;
                break;
            case GLFW_KEY_R: // Reset
                angleX = 0.0f;
                angleY = 0.0f;
                angleZ = 0.0f;
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 5 Q1b - 3D Rotation", NULL, NULL);
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
    std::cout << "  X - Rotate around X-axis" << std::endl;
    std::cout << "  Y - Rotate around Y-axis" << std::endl;
    std::cout << "  Z - Rotate around Z-axis" << std::endl;
    std::cout << "  SPACE - Toggle auto-rotation" << std::endl;
    std::cout << "  R - Reset rotation" << std::endl;
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

        // Apply rotations
        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        glRotatef(angleZ, 0.0f, 0.0f, 1.0f);

        // Auto-rotation
        if (autoRotate) {
            angleY += 0.5f;
            angleX += 0.3f;
            if (angleY >= 360.0f) angleY -= 360.0f;
            if (angleX >= 360.0f) angleX -= 360.0f;
        }

        // Draw the cube
        drawCube();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
