#include <GLFW/glfw3.h>
#include <iostream>

// Liang-Barsky Line Clipping
// Clipping window in NDC coordinates (-1 to 1)
float xMinClip = -0.5f;
float xMaxClip =  0.5f;
float yMinClip = -0.5f;
float yMaxClip =  0.5f;

bool liangBarskyClip(float x0, float y0, float x1, float y1,
                     float &cx0, float &cy0, float &cx1, float &cy1) {
    float dx = x1 - x0;
    float dy = y1 - y0;

    float p[4] = { -dx,  dx, -dy,  dy };
    float q[4] = { x0 - xMinClip,
                   xMaxClip - x0,
                   y0 - yMinClip,
                   yMaxClip - y0 };

    float u1 = 0.0f; // entering
    float u2 = 1.0f; // leaving

    for (int i = 0; i < 4; ++i) {
        if (p[i] == 0.0f) {
            if (q[i] < 0.0f) {
                return false; // Parallel and outside
            }
        } else {
            float r = q[i] / p[i];
            if (p[i] < 0.0f) {
                if (r > u2) return false;
                if (r > u1) u1 = r;
            } else {
                if (r < u1) return false;
                if (r < u2) u2 = r;
            }
        }
    }

    cx0 = x0 + u1 * dx;
    cy0 = y0 + u1 * dy;
    cx1 = x0 + u2 * dx;
    cy1 = y0 + u2 * dy;
    return true;
}

void drawClippingWindow() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(xMinClip, yMinClip);
        glVertex2f(xMaxClip, yMinClip);
        glVertex2f(xMaxClip, yMaxClip);
        glVertex2f(xMinClip, yMaxClip);
    glEnd();
}

void drawLineRaw(float x0, float y0, float x1, float y1) {
    glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
    glEnd();
}

void drawClippedLineLB(float x0, float y0, float x1, float y1) {
    // Draw original (unclipped) line in red
    glColor3f(1.0f, 0.0f, 0.0f);
    drawLineRaw(x0, y0, x1, y1);

    float cx0, cy0, cx1, cy1;
    if (liangBarskyClip(x0, y0, x1, y1, cx0, cy0, cx1, cy1)) {
        glColor3f(0.0f, 1.0f, 0.0f);
        drawLineRaw(cx0, cy0, cx1, cy1);
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 4 Q2 - Liang-Barsky Line Clipping", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawClippingWindow();

        // Example lines
        drawClippedLineLB(-0.9f, -0.9f, 0.9f, 0.9f);     // Diagonal through window
        drawClippedLineLB(-0.9f, 0.0f, 0.9f, 0.0f);       // Horizontal
        drawClippedLineLB(0.0f, -0.9f, 0.0f, 0.9f);       // Vertical
        drawClippedLineLB(-0.9f, 0.7f, -0.2f, 0.8f);      // Completely outside
        drawClippedLineLB(-0.9f, -0.2f, -0.2f, 0.8f);     // Partially inside

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
