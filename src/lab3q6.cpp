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
// 2D Shearing Transformations
// ==============================================

Mat3 shearX(float shx) {
    Mat3 M = identity();
    M.m[0][1] = shx;  // x' = x + shx * y
    return M;
}

Mat3 shearY(float shy) {
    Mat3 M = identity();
    M.m[1][0] = shy;  // y' = y + shy * x
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

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 3 Q6 - 2D Shearing", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Base square centered at origin
    std::vector<Vec2> square = {
        { -0.3f, -0.3f },
        {  0.3f, -0.3f },
        {  0.3f,  0.3f },
        { -0.3f,  0.3f }
    };

    // Shearing transformations
    Mat3 SX1 = shearX(0.5f);         // Shear in X direction
    Mat3 SX2 = shearX(-0.5f);        // Shear in X direction (negative)
    Mat3 SY1 = shearY(0.5f);         // Shear in Y direction
    Mat3 SY2 = shearY(-0.5f);        // Shear in Y direction (negative)

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Original square
        drawPolygon(square, 1.0f, 1.0f, 1.0f);  // White

        // X-shear (positive)
        auto sq1 = applyTransform(square, SX1);
        drawPolygon(sq1, 1.0f, 0.0f, 0.0f);  // Red

        // X-shear (negative)
        auto sq2 = applyTransform(square, SX2);
        drawPolygon(sq2, 0.0f, 1.0f, 0.0f);  // Green

        // Y-shear (positive)
        auto sq3 = applyTransform(square, SY1);
        drawPolygon(sq3, 0.0f, 0.0f, 1.0f);  // Blue

        // Y-shear (negative)
        auto sq4 = applyTransform(square, SY2);
        drawPolygon(sq4, 1.0f, 1.0f, 0.0f);  // Yellow

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
