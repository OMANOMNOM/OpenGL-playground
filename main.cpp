#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "shader.h"
#include "Renderer.h" 
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "TestClearColor.h"
#include "Test.h"

test::Test* getScene(int selectedScene) {
    std::cout << selectedScene;
    test::Test* selection = nullptr;

    switch (selectedScene) {
    case 1:
        selection = new test::TestClearColor();
        break;
    case 2:
        std::cout << "Not a valid selection";
        return nullptr;
        selection = nullptr;

        break;
    default:
        std::cout << "Not a valid selection";
        selection = nullptr;
    return selection;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

    // This open bracket is to create a new scope. 
    // This makes sure all stack opengl stuff is destroyed before glfw terminate
    // otherwise glfw terminate would generate an error and the applcaiton wouldn't terminate.
    {
        

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::Test* test = nullptr;
        //test::TestClearColor* test = nullptr;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            ImGui_ImplGlfwGL3_NewFrame();

            
            if (test == nullptr)
            {
                ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
                int currentItem;
                ImGui::Combo("Test label", &currentItem, "item1\0item2\0item3\0item4\0item5\0item6\0item7\0item8\0item9\0item10\0", 3);
                if (ImGui::Button("Load"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated
                {
                    //Create that test enviroment
                    test = (test::Test*)getScene(currentItem);
                }
            }
            else
            {
                // Render chosen test
                test->OnUpdate(0.0f);
                test->OnRender();
                test->OnImGuiRender();

            }
            

            

            
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}