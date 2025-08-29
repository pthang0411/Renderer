#include <iostream>
using namespace std;
#include <renderer.hpp>
#include <errorReporting.hpp>

bool isKeyDown(GLenum key, Renderer::Window window){
    return glfwGetKey(window.getGLFWwindow(), key) == GLFW_PRESS;
}

int main(){
    Renderer::Window window(800, 800, "title");
    window.initWindow();
    Renderer::Renderer renderer;
    renderer.updateRenderField(window);
    Renderer::Scene scene;
    Renderer::Entity* entity = scene.createEntity(Renderer::TRIANGLE, Vec2(500.0f, 500.0f), Vec2(500.0f, 500.0f), 0.0f, Renderer::Color(1.0f, 1.0f, 1.0f, 1.0f), "C:/Users/JOHNPC/Pictures/Saved Pictures/pic.PNG");
    Renderer::Entity* entity1 = scene.createEntity(Renderer::RECTANGLE, Vec2(200.0f, 200.0f), Vec2(500.0f, 500.0f), 40.0f, Renderer::Color(1.0f,1.0f, 1.0f, 1.0f), "C:/Users/JOHNPC/Pictures/Saved Pictures/freaky jerry.jpg");

    while (!window.closingWindow()){       
        if (isKeyDown(GLFW_KEY_A, window)){
            entity->position.x -= 0.1f;
        }
        if (isKeyDown(GLFW_KEY_D, window)){
            entity->position.x += 0.1f;
        }
        if (isKeyDown(GLFW_KEY_W, window)){
            entity->position.y -= 0.1f;
        }
        if (isKeyDown(GLFW_KEY_S, window)){
            entity->position.y += 0.1f;
        }
        window.beginFrame();
        renderer.shader.use();
        renderer.drawScene(scene, Renderer::YSORT_RENDERING);
        window.endFrame();
    }
    return 0;
}