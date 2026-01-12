#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void plotPixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

// Plot 4-way symmetric points of an ellipse, converting from pixel to NDC
void plotEllipsePoints(int xc, int yc, int x, int y) {
    // Window assumed 800x600, center (400,300)
    plotPixel((xc + x) / 400.0f - 1.0f, (yc + y) / 300.0f - 1.0f);
    plotPixel((xc - x) / 400.0f - 1.0f, (yc + y) / 300.0f - 1.0f);
    plotPixel((xc + x) / 400.0f - 1.0f, (yc - y) / 300.0f - 1.0f);
    plotPixel((xc - x) / 400.0f - 1.0f, (yc - y) / 300.0f - 1.0f);
}

// Midpoint ellipse algorithm
void drawEllipseMidpoint(int xc, int yc, int rx, int ry) {
    int x = 0;
    int y = ry;

    long rx2 = 1L * rx * rx;
    long ry2 = 1L * ry * ry;

    long dx = 2 * ry2 * x;
    long dy = 2 * rx2 * y;

    long d1 = ry2 - rx2 * ry + rx2 / 4;
    plotEllipsePoints(xc, yc, x, y);

    // Region 1
    while (dx < dy) {
        x++;
        dx = dx + 2 * ry2;
        if (d1 < 0) {
            d1 = d1 + dx + ry2;
        } else {
            y--;
            dy = dy - 2 * rx2;
            d1 = d1 + dx - dy + ry2;
        }
        plotEllipsePoints(xc, yc, x, y);
    }

    // Region 2
    long d2 = ry2 * (x + 0.5f) * (x + 0.5f)
            + rx2 * (y - 1) * (y - 1)
            - rx2 * ry2;

    while (y >= 0) {
        y--;
        dy = dy - 2 * rx2;
        if (d2 > 0) {
            d2 = d2 + rx2 - dy;
        } else {
            x++;
            dx = dx + 2 * ry2;
            d2 = d2 + dx + rx2 - dy;
        }
        plotEllipsePoints(xc, yc, x, y);
    }
}

// Convenience wrapper using NDC center and radii in NDC
void drawEllipse(float cxNdc, float cyNdc, float rxNdc, float ryNdc) {
    int xc = static_cast<int>((cxNdc + 1.0f) * 400.0f);
    int yc = static_cast<int>((cyNdc + 1.0f) * 300.0f);
    int rx = static_cast<int>(rxNdc * 400.0f);
    int ry = static_cast<int>(ryNdc * 300.0f);

    drawEllipseMidpoint(xc, yc, rx, ry);
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 3 Q1 - Midpoint Ellipse", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glPointSize(2.0f);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.0f, 1.0f, 0.0f);
        // Single ellipse centered at origin
        drawEllipse(0.0f, 0.0f, 0.5f, 0.25f);

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
