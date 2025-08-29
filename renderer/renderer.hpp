#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <Shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <algorithm>
#include <vector>

struct Vec2{
    float x;
    float y;
    Vec2():x(0), y(0){}
    Vec2(float x, float y):x(x), y(y){}
    Vec2 operator+(Vec2 v){
        return Vec2(x+v.x, y+v.y);
    };
    Vec2 operator-(Vec2 v){
        return Vec2(x-v.x, y-v.y);
    };
    template <typename T>
    Vec2 operator*(T v){
        return Vec2(x*v, y*v);
    };
    template <typename T>
    Vec2 operator/(T v){
        return Vec2(x/v, y/v);
    };
};
struct Vec3{
    float x;
    float y;
    float z;
    Vec3(float x, float y, float z): x(x), y(y), z(z){}
};

namespace Renderer{
    const char* vertexShader = R"(
    #version 460 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec2 texPosition;
    uniform mat4 model;
    uniform mat4 proj;
    uniform vec4 color;
    out vec2 FtexPos;
    out vec4 Fcolor;
    void main(){
        gl_Position = proj * model * vec4(position, 1.0f);
        Fcolor = color;
        FtexPos = texPosition;
    }
    )";
    const char* fragmentShader = R"(
    #version 460 core
    out vec4 FragColor;
    in vec2 FtexPos;
    in vec4 Fcolor;
    uniform sampler2D tex;
    uniform bool TexAvail;
    void main(){
        if (TexAvail){
            FragColor = texture(tex, FtexPos) * Fcolor;
        }
        else{
            FragColor = Fcolor;
        }
    }
    )";

    unsigned int TRIANGLE = 1;
    unsigned int RECTANGLE = 2;
    unsigned int NORMAL_RENDERING = 1;
    unsigned int YSORT_RENDERING = 2;
    struct Color{
        float red;
        float green;
        float blue;
        float alpha;
        Color(float red, float green, float blue, float alpha){
            this->red = red;
            this->blue = blue;
            this->green = green;
            this->alpha = alpha;
        }
    };
    struct ObjectData{
        const float* vertices;
        const size_t size;
        const float* indices = nullptr;
    };
    struct TriangleData{
        const float vertices[15] = {
            0.0f,  -0.5f, 0.0f, 0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f
        };
        const size_t size = sizeof(float)*15;
    };
    struct QuadData{
        const float vertices[20] = {
            -0.5f,  -0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f
        };
        const size_t size = sizeof(float)*20;
        const unsigned int indices[6] = {    
            0, 1, 2,   
            2, 1, 3
        };
        const size_t indicesSize = sizeof(unsigned int)*6;
    };
    
    class Entity{
        private:
        inline static unsigned int id = 0;  
        public:
        bool textureAvailable;
        glm::mat4 model = glm::mat4(1.0f);  
        unsigned int vboId;
        unsigned int eboId;
        unsigned int vaoId;
        unsigned int texId;
        GLuint texUnit = 0;
        inline static GLuint newTexUnit = 0;
        unsigned int mesh;
        Vec2 position;
        Vec2 scale;
        float rotation;
        Color color;
        unsigned int ID;
        Entity(unsigned int mesh, Vec2 position = {0.0f, 0.0f}, Vec2 scale = {1.0f, 1.0f}, float rotation = 0.0f, Color color = {1.0f, 0.0f, 0.0f, 1.0f}, const char* texPath = ""):color(color),mesh(mesh), position(position), scale(scale), rotation(rotation){
            id ++;
            ID = id;
            textureAvailable = false;
            if (mesh == RECTANGLE){
                QuadData quadData;
                glGenVertexArrays(1, &vaoId);
                glBindVertexArray(vaoId);

                glGenBuffers(1, &vboId);
                glBindBuffer(GL_ARRAY_BUFFER, vboId);
                glBufferData(GL_ARRAY_BUFFER, quadData.size, quadData.vertices, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));
                glEnableVertexAttribArray(1);

                glGenBuffers(1, &eboId);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadData.indicesSize, quadData.indices, GL_STATIC_DRAW);
            }
            if (mesh == TRIANGLE){
                TriangleData triangleData;
                glGenVertexArrays(1, &vaoId);
                glBindVertexArray(vaoId);

                glGenBuffers(1, &vboId);
                glBindBuffer(GL_ARRAY_BUFFER, vboId);
                glBufferData(GL_ARRAY_BUFFER, triangleData.size, triangleData.vertices, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)0);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));
                glEnableVertexAttribArray(1);
            }
            if (texPath!=""){
                textureAvailable = true;
                int width, height, nrchannels;
                unsigned char* data = stbi_load(texPath, &width, &height, &nrchannels, 0);

                glGenTextures(1, &texId);
                glBindTexture(GL_TEXTURE_2D, texId);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                if (nrchannels == 3){
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                }
                else if (nrchannels == 4){
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                }
                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(data);
                texUnit = newTexUnit;
                newTexUnit ++;
            }
        }
        void draw(){
            glBindVertexArray(vaoId);
            if (mesh == TRIANGLE){
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }
            if (mesh == RECTANGLE){
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
        ~Entity(){
            glDeleteVertexArrays(1, &vaoId);
            glDeleteBuffers(1, &vboId);
            glDeleteBuffers(1, &eboId);
        };
    };
    class Scene{
            
        public:
            std::vector <Entity*> renderedEntities;
            std::vector <Entity*> ysortedEntities;
            Entity* createEntity(unsigned int mesh,  Vec2 position = {0.0f, 0.0f}, Vec2 scale = {1.0f, 1.0f}, float rotation = 0.0f, Color color = {1.0f, 0.0f, 0.0f, 1.0f}, const char* texPath=""){
                Entity* entity = new Entity(mesh, position, scale, rotation, color, texPath);
                renderedEntities.push_back(entity);
                return entity;
            };
            void deleteEntity(unsigned int id) {
                renderedEntities.erase(
                    std::remove_if(renderedEntities.begin(), renderedEntities.end(),
                        [id](Entity* entity) {
                            if (entity->ID == id) {
                                delete entity;     // only delete the matching one
                                return true;       // mark for removal
                            }
                            return false;          // keep others
                        }),
                    renderedEntities.end()
                );
            }

    };
    
    class Window{
        private:
            GLFWwindow* window;
        public:
            Color color;
            const char* title;
            int screenWidth, screenHeight;
            Window(int screenWidth, int screenHeight, const char* title, Color color = {1.0f, 1.0f, 1.0f, 1.0f}):color(color), screenWidth(screenWidth), screenHeight(screenHeight),title(title){}
            int initWindow(){
                if (!glfwInit()){
                    return -1;
                }
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
                this->window = window;
                if (!this->window){
                    glfwTerminate();
                    return -1;
                }
                glfwMakeContextCurrent(window);
                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
                    glfwTerminate();
                    return -1;
                }
                glViewport(0, 0 ,screenWidth, screenHeight);
                return 0;
            }
            GLFWwindow* getGLFWwindow(){
                return this->window;
            }
            bool closingWindow(){
                return glfwWindowShouldClose(window);
            }
            void destroyWindow(){
                glfwDestroyWindow(window);
                glfwTerminate();
            }
            void beginFrame(){
                glClearColor(color.red, color.green, color.blue, color.alpha);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            void endFrame(){
                glfwPollEvents();
                glfwSwapBuffers(window);
            }
            
    };

    class Renderer{
        protected:
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection;
            
        public:
        ShaderFromText shader = ShaderFromText(vertexShader, fragmentShader);
        void drawScene(Scene scene, unsigned int drawingOption = NORMAL_RENDERING){
            
            if (drawingOption == NORMAL_RENDERING){
                for (Entity* entity:scene.renderedEntities){
                    shader.use();
                    entity->model = glm::mat4(1.0f);
                    entity->model = glm::translate(entity->model, glm::vec3(entity->position.x, entity->position.y, 0.0f));
                    entity->model = glm::rotate(entity->model,entity->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
                    entity->model = glm::scale(entity->model, glm::vec3(entity->scale.x, entity->scale.y, 1.0f));
                    if (entity->textureAvailable){
                        glActiveTexture(GL_TEXTURE0 + entity->texUnit);
                        glBindTexture(GL_TEXTURE_2D, entity->texId);
                        shader.setInt("tex", entity->texUnit);
                    }
                    shader.setBool("TexAvail", entity->textureAvailable);
                    shader.setVec4f("color", entity->color.red, entity->color.green, entity->color.blue, entity->color.alpha);
                    shader.setMat4fv("model", entity->model);
                    entity->draw();
                }
            }
            else if (drawingOption == YSORT_RENDERING){
                scene.ysortedEntities = scene.renderedEntities;
                std::sort(scene.ysortedEntities.begin(), scene.ysortedEntities.end(),
                [](Entity* a, Entity* b) {
                    return a->position.y < b->position.y;
                });
                for (Entity* entity:scene.ysortedEntities){
                    shader.use();
                    entity->model = glm::mat4(1.0f);
                    entity->model = glm::translate(entity->model, glm::vec3(entity->position.x, entity->position.y, 0.0f));
                    entity->model = glm::rotate(entity->model,entity->rotation, glm::vec3(0.0f, 0.0f, 1.0f));
                    entity->model = glm::scale(entity->model, glm::vec3(entity->scale.x, entity->scale.y, 1.0f));
                    if (entity->textureAvailable){
                        glActiveTexture(GL_TEXTURE0 + entity->texUnit);
                        glBindTexture(GL_TEXTURE_2D, entity->texId);
                        shader.setInt("tex", entity->texUnit);
                    }
                    shader.setBool("TexAvail", entity->textureAvailable);
                    shader.setVec4f("color", entity->color.red, entity->color.green, entity->color.blue, entity->color.alpha);
                    shader.setMat4fv("model", entity->model);
                    entity->draw();
                }
            }
            
        };

        void updateRenderField(Window window){
            projection = glm::ortho(0.0f, (float)window.screenWidth, (float)window.screenHeight, 0.0f, -10.0f, 10.0f);
            shader.use();
            shader.setMat4fv("proj", projection);
        }
        
    };

};

namespace Box{
    class Box2D{
    private:
    Vec2 distanceFromParent;
    public:
    Vec2 position;
    float rotation;
    Vec2 scale;
    Renderer::Entity* parent;
        Box2D(Renderer::Entity* parent = 0, Vec2 position = {0,0}, Vec2 scale = {1,1}, float rotation = 0.0f):position(position), scale(scale), rotation(rotation), parent(parent){
            if (parent != 0){
                distanceFromParent = parent->position - position;
            }
        }
        void FollowParent(){

        }
    };
}