#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#define PI 3.14159265359

void plotPixel(float x, float y) {
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

// Midpoint Circle Drawing Algorithm
void drawCircleMidpoint(int xc, int yc, int radius) {
    int x = 0;
    int y = radius;
    int d = 1 - radius;
    
    auto plotCirclePoints = [&](int x, int y) {
        plotPixel((xc + x) / 400.0f - 1.0f, (yc + y) / 300.0f - 1.0f);
        plotPixel((xc - x) / 400.0f - 1.0f, (yc + y) / 300.0f - 1.0f);
        plotPixel((xc + x) / 400.0f - 1.0f, (yc - y) / 300.0f - 1.0f);
        plotPixel((xc - x) / 400.0f - 1.0f, (yc - y) / 300.0f - 1.0f);
        plotPixel((xc + y) / 400.0f - 1.0f, (yc + x) / 300.0f - 1.0f);
        plotPixel((xc - y) / 400.0f - 1.0f, (yc + x) / 300.0f - 1.0f);
        plotPixel((xc + y) / 400.0f - 1.0f, (yc - x) / 300.0f - 1.0f);
        plotPixel((xc - y) / 400.0f - 1.0f, (yc - x) / 300.0f - 1.0f);
    };
    
    plotCirclePoints(x, y);
    
    while (x < y) {
        x++;
        if (d < 0) {
            d = d + 2 * x + 1;
        } else {
            y--;
            d = d + 2 * (x - y) + 1;
        }
        plotCirclePoints(x, y);
    }
}

// Bresenham Line Drawing Algorithm
void bresenhamLow(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int yi = 1;
    
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    
    int D = 2 * dy - dx;
    int y = y1;
    
    for (int x = x1; x <= x2; x++) {
        plotPixel(x / 400.0f - 1.0f, y / 300.0f - 1.0f);
        if (D > 0) {
            y += yi;
            D += 2 * (dy - dx);
        } else {
            D += 2 * dy;
        }
    }
}

void bresenhamHigh(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int xi = 1;
    
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    
    int D = 2 * dx - dy;
    int x = x1;
    
    for (int y = y1; y <= y2; y++) {
        plotPixel(x / 400.0f - 1.0f, y / 300.0f - 1.0f);
        if (D > 0) {
            x += xi;
            D += 2 * (dx - dy);
        } else {
            D += 2 * dx;
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    if (abs(y2 - y1) < abs(x2 - x1)) {
        if (x1 > x2) {
            bresenhamLow(x2, y2, x1, y1);
        } else {
            bresenhamLow(x1, y1, x2, y2);
        }
    } else {
        if (y1 > y2) {
            bresenhamHigh(x2, y2, x1, y1);
        } else {
            bresenhamHigh(x1, y1, x2, y2);
        }
    }
}

// Structure for pie chart data
struct PieSlice {
    float value;
    std::string label;
    float r, g, b;  // Color
};

// Fill a sector of the pie chart
void fillSector(int xc, int yc, int radius, float startAngle, float endAngle, float r, float g, float b) {
    glColor3f(r, g, b);
    
    // Draw filled sector using triangles
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(xc / 400.0f - 1.0f, yc / 300.0f - 1.0f);  // Center
    
    float angleStep = 0.01f;  // Small step for smooth arc
    for (float angle = startAngle; angle <= endAngle; angle += angleStep) {
        float x = xc + radius * cos(angle);
        float y = yc + radius * sin(angle);
        glVertex2f(x / 400.0f - 1.0f, y / 300.0f - 1.0f);
    }
    
    // Last point at exact end angle
    float x = xc + radius * cos(endAngle);
    float y = yc + radius * sin(endAngle);
    glVertex2f(x / 400.0f - 1.0f, y / 300.0f - 1.0f);
    
    glEnd();
}

// Draw outline of a pie slice
void drawSliceOutline(int xc, int yc, int radius, float startAngle, float endAngle) {
    // Draw arc
    float angleStep = 0.01f;
    float prevX = xc + radius * cos(startAngle);
    float prevY = yc + radius * sin(startAngle);
    
    for (float angle = startAngle + angleStep; angle <= endAngle; angle += angleStep) {
        float x = xc + radius * cos(angle);
        float y = yc + radius * sin(angle);
        drawLine((int)prevX, (int)prevY, (int)x, (int)y);
        prevX = x;
        prevY = y;
    }
    
    // Draw radial lines
    int x1 = xc + radius * cos(startAngle);
    int y1 = yc + radius * sin(startAngle);
    int x2 = xc + radius * cos(endAngle);
    int y2 = yc + radius * sin(endAngle);
    
    drawLine(xc, yc, x1, y1);
    drawLine(xc, yc, x2, y2);
}

// Draw complete pie chart
void drawPieChart(int xc, int yc, int radius, const std::vector<PieSlice>& slices) {
    // Calculate total
    float total = 0.0f;
    for (const auto& slice : slices) {
        total += slice.value;
    }
    
    if (total <= 0.0f) return;
    
    // Draw slices
    float currentAngle = -PI / 2.0f;  // Start from top (90 degrees)
    
    for (const auto& slice : slices) {
        float sliceAngle = (slice.value / total) * 2.0f * PI;
        float endAngle = currentAngle + sliceAngle;
        
        // Fill the slice
        fillSector(xc, yc, radius, currentAngle, endAngle, slice.r, slice.g, slice.b);
        
        // Draw outline
        glColor3f(1.0f, 1.0f, 1.0f);
        drawSliceOutline(xc, yc, radius, currentAngle, endAngle);
        
        currentAngle = endAngle;
    }
    
    // Draw outer circle for clean edge
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.0f);
    drawCircleMidpoint(xc, yc, radius);
    glPointSize(2.0f);
}

// Draw legend
void drawLegend(int startX, int startY, const std::vector<PieSlice>& slices) {
    int boxSize = 15;
    int spacing = 25;
    int yOffset = 0;
    
    // Calculate total for percentages
    float total = 0.0f;
    for (const auto& slice : slices) {
        total += slice.value;
    }
    
    for (size_t i = 0; i < slices.size(); i++) {
        int y = startY + yOffset;
        
        // Draw colored box
        glColor3f(slices[i].r, slices[i].g, slices[i].b);
        glBegin(GL_QUADS);
        glVertex2f(startX / 400.0f - 1.0f, y / 300.0f - 1.0f);
        glVertex2f((startX + boxSize) / 400.0f - 1.0f, y / 300.0f - 1.0f);
        glVertex2f((startX + boxSize) / 400.0f - 1.0f, (y + boxSize) / 300.0f - 1.0f);
        glVertex2f(startX / 400.0f - 1.0f, (y + boxSize) / 300.0f - 1.0f);
        glEnd();
        
        // Draw box outline
        glColor3f(1.0f, 1.0f, 1.0f);
        drawLine(startX, y, startX + boxSize, y);
        drawLine(startX + boxSize, y, startX + boxSize, y + boxSize);
        drawLine(startX + boxSize, y + boxSize, startX, y + boxSize);
        drawLine(startX, y + boxSize, startX, y);
        
        yOffset += spacing;
    }
}

int main() {
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 2 Q5 - Pie Chart", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glPointSize(2.0f);
    
    // Sample data: Market share or budget distribution
    std::vector<PieSlice> pieData = {
        {35.0f, "Category A", 1.0f, 0.2f, 0.2f},   // Red
        {25.0f, "Category B", 0.2f, 0.8f, 0.2f},   // Green
        {20.0f, "Category C", 0.2f, 0.4f, 1.0f},   // Blue
        {12.0f, "Category D", 1.0f, 0.8f, 0.2f},   // Yellow
        {8.0f, "Category E", 1.0f, 0.4f, 0.8f}     // Pink
    };
    
    int centerX = 300;
    int centerY = 300;
    int radius = 150;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw pie chart
        drawPieChart(centerX, centerY, radius, pieData);
        
        // Draw legend
        drawLegend(550, 200, pieData);
        
        // Draw title
        glColor3f(1.0f, 1.0f, 1.0f);

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
