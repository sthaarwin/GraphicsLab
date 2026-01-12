#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// ==============================================
// 2D Vector and 3x3 Matrix Structures
// ==============================================

struct Vec2 {
    float x, y;
};

struct Mat3 {
    float m[3][3];
};

// ==============================================
// Matrix Operations
// ==============================================

Mat3 identity() {
    Mat3 I{};
    I.m[0][0] = 1.0f; I.m[0][1] = 0.0f; I.m[0][2] = 0.0f;
    I.m[1][0] = 0.0f; I.m[1][1] = 1.0f; I.m[1][2] = 0.0f;
    I.m[2][0] = 0.0f; I.m[2][1] = 0.0f; I.m[2][2] = 1.0f;
    return I;
}

Vec2 transformPoint(const Mat3& M, const Vec2& p) {
    float x = M.m[0][0] * p.x + M.m[0][1] * p.y + M.m[0][2];
    float y = M.m[1][0] * p.x + M.m[1][1] * p.y + M.m[1][2];
    return { x, y };
}

// ==============================================
// 2D Reflection Transformations
// ==============================================

Mat3 reflectionX() {
    Mat3 M = identity();
    M.m[1][1] = -1.0f;  // Reflect across X-axis (flip Y)
    return M;
}

Mat3 reflectionY() {
    Mat3 M = identity();
    M.m[0][0] = -1.0f;  // Reflect across Y-axis (flip X)
    return M;
}

Mat3 reflectionOrigin() {
    Mat3 M = identity();
    M.m[0][0] = -1.0f;  // Reflect through origin
    M.m[1][1] = -1.0f;
    return M;
}

// ==============================================
// Polygon Transformation & Rendering
// ==============================================

std::vector<Vec2> applyTransform(const std::vector<Vec2>& poly, const Mat3& M) {
    std::vector<Vec2> out;
    out.reserve(poly.size());
    for (const auto& p : poly) {
        out.push_back(transformPoint(M, p));
    }
    return out;
}

void drawPolygon(const std::vector<Vec2>& poly, float r, float g, float b) {
    if (poly.empty()) return;
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    for (const auto& p : poly) {
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void drawAxes() {
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    // X-axis
    glVertex2f(-1.0f, 0.0f);
    glVertex2f( 1.0f, 0.0f);
    // Y-axis
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f,  1.0f);
    glEnd();
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 3 Q5 - 2D Reflection", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Base triangle in first quadrant
    std::vector<Vec2> triangle = {
        {  0.1f,  0.1f },
        {  0.5f,  0.1f },
        {  0.3f,  0.4f }
    };

    // Reflection transformations
    Mat3 RX = reflectionX();         // Reflect across X-axis
    Mat3 RY = reflectionY();         // Reflect across Y-axis
    Mat3 RO = reflectionOrigin();    // Reflect through origin

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw axes for reference
        drawAxes();

        // Original triangle (first quadrant)
        drawPolygon(triangle, 1.0f, 1.0f, 1.0f);  // White

        // Reflect across X-axis (to fourth quadrant)
        auto tri1 = applyTransform(triangle, RX);
        drawPolygon(tri1, 1.0f, 0.0f, 0.0f);  // Red

        // Reflect across Y-axis (to second quadrant)
        auto tri2 = applyTransform(triangle, RY);
        drawPolygon(tri2, 0.0f, 1.0f, 0.0f);  // Green

        // Reflect through origin (to third quadrant)
        auto tri3 = applyTransform(triangle, RO);
        drawPolygon(tri3, 0.0f, 0.0f, 1.0f);  // Blue

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
