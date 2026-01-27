#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

// Camera parameters for perspective projection
float fov = 45.0f;      // Field of view (degrees)
float nearPlane = 1.0f;
float farPlane = 50.0f;
float cameraZ = -8.0f;  // Camera position

// Multiple objects at different depths
struct Object {
    float x, y, z;
    float size;
    float colorR, colorG, colorB;
};

Object objects[] = {
    {-3.0f, 0.0f, -5.0f, 1.0f, 1.0f, 0.0f, 0.0f},   // Red cube (near)
    {0.0f, 0.0f, -10.0f, 1.5f, 0.0f, 1.0f, 0.0f},   // Green cube (middle)
    {3.0f, 0.0f, -15.0f, 2.0f, 0.0f, 0.0f, 1.0f},   // Blue cube (far)
    {0.0f, 3.0f, -8.0f, 0.8f, 1.0f, 1.0f, 0.0f},    // Yellow cube (top middle)
    {0.0f, -3.0f, -12.0f, 1.2f, 1.0f, 0.0f, 1.0f}   // Magenta cube (bottom middle)
};
const int numObjects = 5;

void drawCube(float size, float r, float g, float b) {
    glColor3f(r, g, b);
    
    glBegin(GL_QUADS);
    
    // Front face
    glVertex3f(-size, -size,  size);
    glVertex3f( size, -size,  size);
    glVertex3f( size,  size,  size);
    glVertex3f(-size,  size,  size);
    
    // Back face
    glVertex3f(-size, -size, -size);
    glVertex3f(-size,  size, -size);
    glVertex3f( size,  size, -size);
    glVertex3f( size, -size, -size);
    
    // Top face
    glVertex3f(-size,  size, -size);
    glVertex3f(-size,  size,  size);
    glVertex3f( size,  size,  size);
    glVertex3f( size,  size, -size);
    
    // Bottom face
    glVertex3f(-size, -size, -size);
    glVertex3f( size, -size, -size);
    glVertex3f( size, -size,  size);
    glVertex3f(-size, -size,  size);
    
    // Right face
    glVertex3f( size, -size, -size);
    glVertex3f( size,  size, -size);
    glVertex3f( size,  size,  size);
    glVertex3f( size, -size,  size);
    
    // Left face
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size,  size);
    glVertex3f(-size,  size,  size);
    glVertex3f(-size,  size, -size);
    
    glEnd();

    // Draw wireframe
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size, -size,  size);
    glVertex3f( size, -size,  size);
    glVertex3f( size,  size,  size);
    glVertex3f(-size,  size,  size);
    glEnd();
    
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size, -size, -size);
    glVertex3f( size, -size, -size);
    glVertex3f( size,  size, -size);
    glVertex3f(-size,  size, -size);
    glEnd();
    
    glBegin(GL_LINES);
    glVertex3f(-size, -size,  size); glVertex3f(-size, -size, -size);
    glVertex3f( size, -size,  size); glVertex3f( size, -size, -size);
    glVertex3f( size,  size,  size); glVertex3f( size,  size, -size);
    glVertex3f(-size,  size,  size); glVertex3f(-size,  size, -size);
    glEnd();
}

void drawGrid() {
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);
    
    // Horizontal lines
    for (float z = -20.0f; z <= 0.0f; z += 2.0f) {
        glVertex3f(-10.0f, -4.0f, z);
        glVertex3f( 10.0f, -4.0f, z);
    }
    
    // Vertical lines
    for (float x = -10.0f; x <= 10.0f; x += 2.0f) {
        glVertex3f(x, -4.0f, -20.0f);
        glVertex3f(x, -4.0f,   0.0f);
    }
    
    glEnd();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_UP:
                fov = std::max(10.0f, fov - 2.0f);
                std::cout << "FOV: " << fov << " degrees" << std::endl;
                break;
            case GLFW_KEY_DOWN:
                fov = std::min(120.0f, fov + 2.0f);
                std::cout << "FOV: " << fov << " degrees" << std::endl;
                break;
            case GLFW_KEY_W:
                cameraZ += 0.5f;
                std::cout << "Camera Z: " << cameraZ << std::endl;
                break;
            case GLFW_KEY_S:
                cameraZ -= 0.5f;
                std::cout << "Camera Z: " << cameraZ << std::endl;
                break;
            case GLFW_KEY_Q:
                nearPlane = std::max(0.1f, nearPlane - 0.1f);
                std::cout << "Near plane: " << nearPlane << std::endl;
                break;
            case GLFW_KEY_A:
                nearPlane += 0.1f;
                std::cout << "Near plane: " << nearPlane << std::endl;
                break;
            case GLFW_KEY_E:
                farPlane -= 1.0f;
                std::cout << "Far plane: " << farPlane << std::endl;
                break;
            case GLFW_KEY_D:
                farPlane += 1.0f;
                std::cout << "Far plane: " << farPlane << std::endl;
                break;
            case GLFW_KEY_R: // Reset
                fov = 45.0f;
                nearPlane = 1.0f;
                farPlane = 50.0f;
                cameraZ = -8.0f;
                std::cout << "Perspective parameters reset" << std::endl;
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }
}

void setPerspective(float fovDegrees, float aspectRatio, float zNear, float zFar) {
    // Manual perspective projection matrix setup
    float fovRadians = fovDegrees * 3.14159265f / 180.0f;
    float f = 1.0f / tan(fovRadians / 2.0f);
    
    GLfloat perspMatrix[16] = {
        f / aspectRatio, 0.0f, 0.0f, 0.0f,
        0.0f, f, 0.0f, 0.0f,
        0.0f, 0.0f, (zFar + zNear) / (zNear - zFar), -1.0f,
        0.0f, 0.0f, (2.0f * zFar * zNear) / (zNear - zFar), 0.0f
    };
    
    glLoadMatrixf(perspMatrix);
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 5 Q2 - Perspective Projection", NULL, NULL);
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

    std::cout << "========================================" << std::endl;
    std::cout << "Lab 5 Q2 - Perspective Projection Demo" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  UP/DOWN - Decrease/Increase FOV" << std::endl;
    std::cout << "  W/S - Move camera forward/backward" << std::endl;
    std::cout << "  Q/A - Decrease/Increase near plane" << std::endl;
    std::cout << "  E/D - Decrease/Increase far plane" << std::endl;
    std::cout << "  R - Reset perspective parameters" << std::endl;
    std::cout << "  ESC - Exit" << std::endl;
    std::cout << "========================================" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up projection matrix with custom perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        float aspect = 800.0f / 600.0f;
        
        // Use custom perspective or glFrustum
        // setPerspective(fov, aspect, nearPlane, farPlane);
        
        // Alternative: Using glFrustum for perspective
        float tanHalfFov = tan(fov * 3.14159265f / 360.0f);
        float top = nearPlane * tanHalfFov;
        float bottom = -top;
        float right = top * aspect;
        float left = -right;
        glFrustum(left, right, bottom, top, nearPlane, farPlane);

        // Set up modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Position camera
        glTranslatef(0.0f, 0.0f, cameraZ);

        // Draw grid for depth reference
        drawGrid();

        // Draw all objects at different depths
        for (int i = 0; i < numObjects; i++) {
            glPushMatrix();
            glTranslatef(objects[i].x, objects[i].y, objects[i].z);
            drawCube(objects[i].size, objects[i].colorR, objects[i].colorG, objects[i].colorB);
            glPopMatrix();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
