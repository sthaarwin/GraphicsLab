#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    // Initialize GLFW
    if (!glfwInit()){
        std::cout <<"glfw init failed"<<std::endl;
        return -1;
    }

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Lab 2", NULL, NULL);
    if (!window) {
        std::cout << "glfwCreateWindow FAILED" << std::endl;
        glfwTerminate(); return -1;
    }
    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window)){
       
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //your code here

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

   
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
