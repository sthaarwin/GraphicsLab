#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

struct Vec2 {
    float x, y;
};

// Clipping window in NDC coordinates
float xMinClip = -0.5f;
float xMaxClip =  0.5f;
float yMinClip = -0.5f;
float yMaxClip =  0.5f;

// Helper to draw line loop from vertices
void drawPolygon(const std::vector<Vec2>& poly, float r, float g, float b) {
    if (poly.empty()) return;
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    for (const auto& p : poly) {
        glVertex2f(p.x, p.y);
    }
    glEnd();
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

// Sutherland-Hodgman clipping against a single edge
std::vector<Vec2> clipAgainstLeft(const std::vector<Vec2>& in) {
    std::vector<Vec2> out;
    if (in.empty()) return out;

    auto inside = [](const Vec2& p) { return p.x >= xMinClip; };

    auto intersect = [](const Vec2& p1, const Vec2& p2) {
        Vec2 i;
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float t = (xMinClip - p1.x) / dx;
        i.x = xMinClip;
        i.y = p1.y + t * dy;
        return i;
    };

    Vec2 S = in.back();
    for (const auto& E : in) {
        bool S_in = inside(S);
        bool E_in = inside(E);
        if (S_in && E_in) {
            out.push_back(E);
        } else if (S_in && !E_in) {
            out.push_back(intersect(S, E));
        } else if (!S_in && E_in) {
            out.push_back(intersect(S, E));
            out.push_back(E);
        }
        S = E;
    }
    return out;
}

std::vector<Vec2> clipAgainstRight(const std::vector<Vec2>& in) {
    std::vector<Vec2> out;
    if (in.empty()) return out;

    auto inside = [](const Vec2& p) { return p.x <= xMaxClip; };

    auto intersect = [](const Vec2& p1, const Vec2& p2) {
        Vec2 i;
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float t = (xMaxClip - p1.x) / dx;
        i.x = xMaxClip;
        i.y = p1.y + t * dy;
        return i;
    };

    Vec2 S = in.back();
    for (const auto& E : in) {
        bool S_in = inside(S);
        bool E_in = inside(E);
        if (S_in && E_in) {
            out.push_back(E);
        } else if (S_in && !E_in) {
            out.push_back(intersect(S, E));
        } else if (!S_in && E_in) {
            out.push_back(intersect(S, E));
            out.push_back(E);
        }
        S = E;
    }
    return out;
}

std::vector<Vec2> clipAgainstBottom(const std::vector<Vec2>& in) {
    std::vector<Vec2> out;
    if (in.empty()) return out;

    auto inside = [](const Vec2& p) { return p.y >= yMinClip; };

    auto intersect = [](const Vec2& p1, const Vec2& p2) {
        Vec2 i;
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float t = (yMinClip - p1.y) / dy;
        i.y = yMinClip;
        i.x = p1.x + t * dx;
        return i;
    };

    Vec2 S = in.back();
    for (const auto& E : in) {
        bool S_in = inside(S);
        bool E_in = inside(E);
        if (S_in && E_in) {
            out.push_back(E);
        } else if (S_in && !E_in) {
            out.push_back(intersect(S, E));
        } else if (!S_in && E_in) {
            out.push_back(intersect(S, E));
            out.push_back(E);
        }
        S = E;
    }
    return out;
}

std::vector<Vec2> clipAgainstTop(const std::vector<Vec2>& in) {
    std::vector<Vec2> out;
    if (in.empty()) return out;

    auto inside = [](const Vec2& p) { return p.y <= yMaxClip; };

    auto intersect = [](const Vec2& p1, const Vec2& p2) {
        Vec2 i;
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float t = (yMaxClip - p1.y) / dy;
        i.y = yMaxClip;
        i.x = p1.x + t * dx;
        return i;
    };

    Vec2 S = in.back();
    for (const auto& E : in) {
        bool S_in = inside(S);
        bool E_in = inside(E);
        if (S_in && E_in) {
            out.push_back(E);
        } else if (S_in && !E_in) {
            out.push_back(intersect(S, E));
        } else if (!S_in && E_in) {
            out.push_back(intersect(S, E));
            out.push_back(E);
        }
        S = E;
    }
    return out;
}

std::vector<Vec2> sutherlandHodgmanClip(const std::vector<Vec2>& poly) {
    std::vector<Vec2> out = poly;
    out = clipAgainstLeft(out);
    out = clipAgainstRight(out);
    out = clipAgainstBottom(out);
    out = clipAgainstTop(out);
    return out;
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 4 Q3 - Sutherland-Hodgman Polygon Clipping", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Example polygon (a concave-ish shape overlapping the window)
    std::vector<Vec2> polygon = {
        {-0.8f, -0.2f},
        {-0.2f, -0.8f},
        { 0.6f, -0.4f},
        { 0.8f,  0.3f},
        { 0.2f,  0.8f},
        {-0.6f,  0.6f}
    };

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawClippingWindow();

        // Original polygon in yellow
        drawPolygon(polygon, 1.0f, 1.0f, 0.0f);

        // Clipped polygon in cyan
        std::vector<Vec2> clipped = sutherlandHodgmanClip(polygon);
        drawPolygon(clipped, 0.0f, 1.0f, 1.0f);

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
