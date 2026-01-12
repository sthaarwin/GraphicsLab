#include <GLFW/glfw3.h>
#include <iostream>

// Cohen-Sutherland Line Clipping constants
const int INSIDE = 0; // 0000
const int LEFT   = 1; // 0001
const int RIGHT  = 2; // 0010
const int BOTTOM = 4; // 0100
const int TOP    = 8; // 1000

// Clipping window in NDC coordinates (-1 to 1)
float xMinClip = -0.5f;
float xMaxClip =  0.5f;
float yMinClip = -0.5f;
float yMaxClip =  0.5f;

int computeOutCode(float x, float y) {
    int code = INSIDE;

    if (x < xMinClip)      code |= LEFT;
    else if (x > xMaxClip) code |= RIGHT;

    if (y < yMinClip)      code |= BOTTOM;
    else if (y > yMaxClip) code |= TOP;

    return code;
}

// Cohen-Sutherland line clipping algorithm
bool cohenSutherlandClip(float x0, float y0, float x1, float y1,
                         float &cx0, float &cy0, float &cx1, float &cy1) {
    int outCode0 = computeOutCode(x0, y0);
    int outCode1 = computeOutCode(x1, y1);
    bool accept = false;

    while (true) {
        if (!(outCode0 | outCode1)) {
            // Trivially accept
            accept = true;
            break;
        } else if (outCode0 & outCode1) {
            // Trivially reject
            break;
        } else {
            // At least one endpoint is outside the clipping window
            float x, y;
            int outcodeOut = outCode0 ? outCode0 : outCode1;

            if (outcodeOut & TOP) {
                x = x0 + (x1 - x0) * (yMaxClip - y0) / (y1 - y0);
                y = yMaxClip;
            } else if (outcodeOut & BOTTOM) {
                x = x0 + (x1 - x0) * (yMinClip - y0) / (y1 - y0);
                y = yMinClip;
            } else if (outcodeOut & RIGHT) {
                y = y0 + (y1 - y0) * (xMaxClip - x0) / (x1 - x0);
                x = xMaxClip;
            } else { // LEFT
                y = y0 + (y1 - y0) * (xMinClip - x0) / (x1 - x0);
                x = xMinClip;
            }

            if (outcodeOut == outCode0) {
                x0 = x;
                y0 = y;
                outCode0 = computeOutCode(x0, y0);
            } else {
                x1 = x;
                y1 = y;
                outCode1 = computeOutCode(x1, y1);
            }
        }
    }

    if (accept) {
        cx0 = x0; cy0 = y0;
        cx1 = x1; cy1 = y1;
    }

    return accept;
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

void drawClippedLineCS(float x0, float y0, float x1, float y1) {
    // Draw original (unclipped) line in red
    glColor3f(1.0f, 0.0f, 0.0f);
    drawLineRaw(x0, y0, x1, y1);

    // Clip
    float cx0, cy0, cx1, cy1;
    if (cohenSutherlandClip(x0, y0, x1, y1, cx0, cy0, cx1, cy1)) {
        // Draw clipped segment in green
        glColor3f(0.0f, 1.0f, 0.0f);
        drawLineRaw(cx0, cy0, cx1, cy1);
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 4 Q1 - Cohen-Sutherland Line Clipping", NULL, NULL);
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
        drawClippedLineCS(-0.9f, -0.9f, 0.9f, 0.9f);     // Diagonal through window
        drawClippedLineCS(-0.9f, 0.0f, 0.9f, 0.0f);       // Horizontal
        drawClippedLineCS(0.0f, -0.9f, 0.0f, 0.9f);       // Vertical
        drawClippedLineCS(-0.9f, 0.7f, -0.2f, 0.8f);      // Completely outside
        drawClippedLineCS(-0.9f, -0.2f, -0.2f, 0.8f);     // Partially inside

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
