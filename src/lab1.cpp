#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void plotPixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2) {
    // Convert OpenGL coords (-1 to 1) to screen pixels
    int X1 = (x1 + 1.0f) * 400;
    int Y1 = (y1 + 1.0f) * 300;
    int X2 = (x2 + 1.0f) * 400;
    int Y2 = (y2 + 1.0f) * 300;

    int dx = abs(X2 - X1);
    int dy = abs(Y2 - Y1);

    int sx = (X1 < X2) ? 1 : -1;
    int sy = (Y1 < Y2) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        // Convert back to OpenGL coordinates
        float px = (X1 / 400.0f) - 1.0f;
        float py = (Y1 / 300.0f) - 1.0f;
        plotPixel(px, py);

        if (X1 == X2 && Y1 == Y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; X1 += sx; }
        if (e2 < dx)  { err += dx; Y1 += sy; }
    }
}

void drawLetterA(float x, float y, float s = 1.0f) {
    drawLine(x, y, x + 0.05f*s, y + 0.1f*s);
    drawLine(x + 0.05f*s, y + 0.1f*s, x + 0.1f*s, y);
    drawLine(x + 0.02f*s, y + 0.05f*s, x + 0.08f*s, y + 0.05f*s);
}

void drawLetterR(float x, float y, float s = 1.0f) {
    drawLine(x, y, x, y + 0.1f*s);
    drawLine(x, y + 0.1f*s, x + 0.07f*s, y + 0.1f*s);
    drawLine(x + 0.07f*s, y + 0.1f*s, x + 0.07f*s, y + 0.05f*s);
    drawLine(x, y + 0.05f*s, x + 0.07f*s, y + 0.05f*s);
    drawLine(x + 0.07f*s, y, x, y + 0.05f*s);
}

void drawLetterW(float x, float y, float s = 1.0f) {
    drawLine(x, y + 0.1f*s, x + 0.025f*s, y);
    drawLine(x + 0.025f*s, y, x + 0.05f*s, y + 0.05f*s);
    drawLine(x + 0.05f*s, y + 0.05f*s, x + 0.075f*s, y);
    drawLine(x + 0.075f*s, y, x + 0.1f*s, y + 0.1f*s);
}

void drawLetterI(float x, float y, float s = 1.0f) {
    drawLine(x, y + 0.1f*s, x + 0.1f*s, y + 0.1f*s);
    drawLine(x + 0.05f*s, y, x + 0.05f*s, y + 0.1f*s);
    drawLine(x, y, x + 0.1f*s, y);
}

void drawLetterN(float x, float y, float s = 1.0f) {
    drawLine(x, y, x, y + 0.1f*s);
    drawLine(x, y + 0.1f*s, x + 0.1f*s, y);
    drawLine(x + 0.1f*s, y, x + 0.1f*s, y + 0.1f*s);
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 1", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glPointSize(3.0f);

    float startX = -0.5f;
    float startY = 0.0f;
    float spacing = 0.12f;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);

        drawLetterA(startX, startY);
        drawLetterR(startX + spacing, startY);
        drawLetterW(startX + 2 * spacing, startY);
        drawLetterI(startX + 3 * spacing, startY);
        drawLetterN(startX + 4 * spacing, startY);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
