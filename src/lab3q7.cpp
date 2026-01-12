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

Mat3 multiply(const Mat3& A, const Mat3& B) {
    Mat3 R{};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            R.m[i][j] = A.m[i][0] * B.m[0][j]
                       + A.m[i][1] * B.m[1][j]
                       + A.m[i][2] * B.m[2][j];
        }
    }
    return R;
}

Vec2 transformPoint(const Mat3& M, const Vec2& p) {
    float x = M.m[0][0] * p.x + M.m[0][1] * p.y + M.m[0][2];
    float y = M.m[1][0] * p.x + M.m[1][1] * p.y + M.m[1][2];
    return { x, y };
}

// ==============================================
// 2D Transformation Matrices
// ==============================================

Mat3 translation(float tx, float ty) {
    Mat3 T = identity();
    T.m[0][2] = tx;
    T.m[1][2] = ty;
    return T;
}

Mat3 scaling(float sx, float sy) {
    Mat3 S = identity();
    S.m[0][0] = sx;
    S.m[1][1] = sy;
    return S;
}

Mat3 rotation(float angleDegrees) {
    float rad = angleDegrees * 3.14159265f / 180.0f;
    float c = std::cos(rad);
    float s = std::sin(rad);

    Mat3 R = identity();
    R.m[0][0] =  c; R.m[0][1] = -s;
    R.m[1][0] =  s; R.m[1][1] =  c;
    return R;
}

Mat3 shearX(float shx) {
    Mat3 M = identity();
    M.m[0][1] = shx;
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 3 Q7 - Composite Transformations", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Base triangle
    std::vector<Vec2> triangle = {
        { -0.6f, -0.3f },
        { -0.3f,  0.2f },
        { -0.1f, -0.3f }
    };

    // ==============================================
    // Composite Transformation 1: Scale -> Rotate -> Translate
    // ==============================================
    Mat3 comp1_step1 = scaling(1.5f, 1.5f);
    Mat3 comp1_step2 = multiply(rotation(45.0f), comp1_step1);
    Mat3 composite1  = multiply(translation(0.5f, 0.5f), comp1_step2);

    // ==============================================
    // Composite Transformation 2: Translate -> Rotate -> Scale
    // ==============================================
    Mat3 comp2_step1 = translation(0.3f, -0.4f);
    Mat3 comp2_step2 = multiply(rotation(-30.0f), comp2_step1);
    Mat3 composite2  = multiply(scaling(0.8f, 1.2f), comp2_step2);

    // ==============================================
    // Composite Transformation 3: Shear -> Scale -> Rotate -> Translate
    // (At least 4 operations)
    // ==============================================
    Mat3 comp3_step1 = shearX(0.3f);
    Mat3 comp3_step2 = multiply(scaling(1.2f, 0.8f), comp3_step1);
    Mat3 comp3_step3 = multiply(rotation(60.0f), comp3_step2);
    Mat3 composite3  = multiply(translation(-0.4f, 0.3f), comp3_step3);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Original triangle
        drawPolygon(triangle, 1.0f, 1.0f, 1.0f);  // White

        // Composite 1: Scale -> Rotate -> Translate
        auto tri1 = applyTransform(triangle, composite1);
        drawPolygon(tri1, 1.0f, 0.0f, 0.0f);  // Red

        // Composite 2: Translate -> Rotate -> Scale
        auto tri2 = applyTransform(triangle, composite2);
        drawPolygon(tri2, 0.0f, 1.0f, 0.0f);  // Green

        // Composite 3: Shear -> Scale -> Rotate -> Translate (4 ops)
        auto tri3 = applyTransform(triangle, composite3);
        drawPolygon(tri3, 0.0f, 0.5f, 1.0f);  // Light Blue

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
