#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>

#include "shaderList.h"
#include "grapher.h"
#include "game.h"


const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "uniform vec2 offset;\n"
                                 "uniform float scale;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = vec4(aPos.x * scale + offset.x, aPos.y * scale + offset.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderRED = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.8554f, 0.1015f, 0.1015f, 1.0f);\n"
                                   "}\0";
								   
const char *fragmentShaderGREEN = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
                                   "}\0";
								   
								   
const char *fragmentShaderBLUE = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.0859f, 0.1171f, 0.3281f, 1.0f);\n"
                                   "}\0";								   


const char *fragmentShaderORANGE = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.9414f, 0.4257f, 0.2031f, 1.0f);\n"
                                   "}\0";

const char *fragmentShaderYELLOW = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                   "}\0";

const char *fragmentShaderBACKGROUND = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.5468f, 0.7773f, 0.7686f, 1.0f);\n"
                                   "}\0";


void frame_buffer_size_call_back(GLFWwindow* in_window, int in_w, int in_h)
{
    glViewport(0, 0, in_w, in_h);
}

void key_call_back(GLFWwindow* in_window, int key, int scan_code, int action, int mods)
{
    if (key ==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(in_window, true);
}

void game_loop(game &in_game)
{
    while(in_game.is_playing)
        in_game.next_move();
}

int main()
{
    const int width = 900;
    const int height = 900;

    // Initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "N EN RAYA", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create a windows\n";
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window); // 2
    
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_call_back);
    glfwSetKeyCallback(window, key_call_back);

    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // SHADERS
    ShaderList shaders;
    shaders.create_vertex_shader(vertexShaderSource);
    shaders.add_fragment_shader("RED", fragmentShaderRED);
    shaders.add_fragment_shader("GREEN", fragmentShaderGREEN);
    shaders.add_fragment_shader("BLUE", fragmentShaderBLUE);
    shaders.add_fragment_shader("ORANGE", fragmentShaderORANGE);
    shaders.add_fragment_shader("YELLOW", fragmentShaderYELLOW);
    shaders.add_fragment_shader("BACKGROUND", fragmentShaderBACKGROUND);

    shaders.delete_shaders();
    
    // GAME SET - UP
    const int n = 4;
    game jueguin(n);
    jueguin.setup_game();

    // Thread to_play
    std::thread t_game(game_loop, std::ref(jueguin));
    
    // GRAPHER
    Grapher drawer(n);
    drawer.set_table();
    drawer.set_circle();
    drawer.set_cross();

    float &step = drawer.step;
    float &init_x = drawer.init_x;
    float &init_y = drawer.init_y;
    float mid = step / 2.0f;

    glLineWidth(5.0f);
    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.5468f, 0.7773f, 0.7686f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		

        shaders.use_shader("ORANGE");
        shaders.set_float("scale", 1.0f);
        shaders.set_vec2("offset", 0.0f, 0.0f);
        drawer.draw_table();


        auto table = jueguin.get_table();

        for (int y = 0; y < n; y++)
        {
            for (int x = 0; x < n; x++)
            {
                float new_x = (init_x + mid) + (x * step);
                float new_y = (init_y - mid) - (y * step);
                
                if (table[y][x] == CPU_PIECE)
                {
                    shaders.use_shader("BLUE");
                    shaders.set_float("scale", (step/2.0f) * 0.8);
                    shaders.set_vec2("offset", new_x, new_y);
                    drawer.draw_circle();


                    shaders.use_shader("BACKGROUND");
                    shaders.set_float("scale", (step/2.0f) * 0.65);
                    shaders.set_vec2("offset", new_x, new_y);
                    drawer.draw_circle();
                }
                else if (table[y][x] == PLAYER_PIECE)
                {
                    shaders.use_shader("RED");
                    shaders.set_float("scale", (step/2.0f) * 0.7);
                    shaders.set_vec2("offset", new_x, new_y);
                    drawer.draw_cross();
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    t_game.join();
    shaders.delete_programs();
    glfwTerminate();
}