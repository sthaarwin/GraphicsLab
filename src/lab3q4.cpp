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
// 2D Scaling Transformation
// ==============================================

Mat3 scaling(float sx, float sy) {
    Mat3 S = identity();
    S.m[0][0] = sx;
    S.m[1][1] = sy;
    return S;
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

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 3 Q4 - 2D Scaling", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Base triangle centered at origin
    std::vector<Vec2> triangle = {
        { -0.2f, -0.2f },
        {  0.2f, -0.2f },
        {  0.0f,  0.2f }
    };

    // Scaling transformations
    Mat3 S1 = scaling(1.5f, 1.5f);       // Uniform scaling (enlarge)
    Mat3 S2 = scaling(0.5f, 0.5f);       // Uniform scaling (shrink)
    Mat3 S3 = scaling(2.0f, 0.5f);       // Non-uniform scaling (wide & short)
    Mat3 S4 = scaling(0.5f, 2.0f);       // Non-uniform scaling (narrow & tall)

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Original triangle
        drawPolygon(triangle, 1.0f, 1.0f, 1.0f);  // White

        // Scaled versions
        auto tri1 = applyTransform(triangle, S1);
        drawPolygon(tri1, 1.0f, 0.0f, 0.0f);  // Red (enlarged)

        auto tri2 = applyTransform(triangle, S2);
        drawPolygon(tri2, 0.0f, 1.0f, 0.0f);  // Green (shrunk)

        auto tri3 = applyTransform(triangle, S3);
        drawPolygon(tri3, 0.0f, 0.0f, 1.0f);  // Blue (wide)

        auto tri4 = applyTransform(triangle, S4);
        drawPolygon(tri4, 1.0f, 1.0f, 0.0f);  // Yellow (tall)

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
