#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
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
// 2D Rotation Transformation
// ==============================================

Mat3 rotation(float angleDegrees) {
    float rad = angleDegrees * 3.14159265f / 180.0f;
    float c = std::cos(rad);
    float s = std::sin(rad);

    Mat3 R = identity();
    R.m[0][0] =  c; R.m[0][1] = -s;
    R.m[1][0] =  s; R.m[1][1] =  c;
    return R;
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 3 Q3 - 2D Rotation", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Base rectangle centered at origin
    std::vector<Vec2> rectangle = {
        { -0.3f, -0.2f },
        {  0.3f, -0.2f },
        {  0.3f,  0.2f },
        { -0.3f,  0.2f }
    };

    // Rotation transformations around origin
    Mat3 R1 = rotation(30.0f);       // 30 degrees
    Mat3 R2 = rotation(60.0f);       // 60 degrees
    Mat3 R3 = rotation(90.0f);       // 90 degrees

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Original rectangle
        drawPolygon(rectangle, 1.0f, 1.0f, 1.0f);  // White

        // Rotated versions
        auto rect1 = applyTransform(rectangle, R1);
        drawPolygon(rect1, 1.0f, 0.0f, 0.0f);  // Red

        auto rect2 = applyTransform(rectangle, R2);
        drawPolygon(rect2, 0.0f, 1.0f, 0.0f);  // Green

        auto rect3 = applyTransform(rectangle, R3);
        drawPolygon(rect3, 0.0f, 0.0f, 1.0f);  // Blue

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
