#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Shearing parameters
float shearXY = 0.0f;  // Shear X based on Y
float shearXZ = 0.0f;  // Shear X based on Z
float shearYX = 0.0f;  // Shear Y based on X
float shearYZ = 0.0f;  // Shear Y based on Z
float shearZX = 0.0f;  // Shear Z based on X
float shearZY = 0.0f;  // Shear Z based on Y

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

void applyShearMatrix() {
    // Apply shearing transformation using glMultMatrix
    // Shear matrix structure:
    // [ 1      shearXY  shearXZ  0 ]
    // [ shearYX  1      shearYZ  0 ]
    // [ shearZX  shearZY  1      0 ]
    // [ 0        0        0      1 ]
    
    GLfloat shearMatrix[16] = {
        1.0f, shearYX, shearZX, 0.0f,
        shearXY, 1.0f, shearZY, 0.0f,
        shearXZ, shearYZ, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    
    glMultMatrixf(shearMatrix);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        const float step = 0.1f;
        switch (key) {
            case GLFW_KEY_1: shearXY += step; break;  // Shear X by Y
            case GLFW_KEY_2: shearXY -= step; break;
            case GLFW_KEY_3: shearXZ += step; break;  // Shear X by Z
            case GLFW_KEY_4: shearXZ -= step; break;
            case GLFW_KEY_5: shearYX += step; break;  // Shear Y by X
            case GLFW_KEY_6: shearYX -= step; break;
            case GLFW_KEY_7: shearYZ += step; break;  // Shear Y by Z
            case GLFW_KEY_8: shearYZ -= step; break;
            case GLFW_KEY_9: shearZX += step; break;  // Shear Z by X
            case GLFW_KEY_0: shearZX -= step; break;
            case GLFW_KEY_Q: shearZY += step; break;  // Shear Z by Y
            case GLFW_KEY_W: shearZY -= step; break;
            case GLFW_KEY_R: // Reset
                shearXY = shearXZ = shearYX = shearYZ = shearZX = shearZY = 0.0f;
                std::cout << "Shearing reset" << std::endl;
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 5 Q1c - 3D Shearing", NULL, NULL);
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
    std::cout << "  1/2 - Increase/Decrease Shear X by Y" << std::endl;
    std::cout << "  3/4 - Increase/Decrease Shear X by Z" << std::endl;
    std::cout << "  5/6 - Increase/Decrease Shear Y by X" << std::endl;
    std::cout << "  7/8 - Increase/Decrease Shear Y by Z" << std::endl;
    std::cout << "  9/0 - Increase/Decrease Shear Z by X" << std::endl;
    std::cout << "  Q/W - Increase/Decrease Shear Z by Y" << std::endl;
    std::cout << "  R - Reset shearing" << std::endl;
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

        // Apply shearing transformation
        applyShearMatrix();

        // Draw the cube
        drawCube();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
