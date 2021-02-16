#include <atomic>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "buffer.h"
#include "context_wrappers.h"
#include "scene.h"
#include "shader.h"
#include "uniform.h"

void readInput(
    std::atomic<glm::vec3>& camera_location_atomic,
    std::atomic<glm::vec3>& camera_look_at_atomic,
    std::atomic<glm::vec3>& light_location_atomic)
{
    while (true)
    {
        std::string type;
        std::cin >> type;

        if (type == "cl")
        {
            glm::vec3 value;
            std::cin >> value.x >> value.y >> value.z;

            camera_location_atomic.store(value, std::memory_order_release);
        }
        else if (type == "la")
        {
            glm::vec3 value;
            std::cin >> value.x >> value.y >> value.z;

            camera_look_at_atomic.store(value, std::memory_order_release);
        }
        else if (type == "ll")
        {
            glm::vec3 value;
            std::cin >> value.x >> value.y >> value.z;

            light_location_atomic.store(value, std::memory_order_release);
        }
    }
}

int main()
{
	try
	{
		GlfwInitContext glfw_init_context;
		if (!glfw_init_context)
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}

		GLFWwindow* window = glfwCreateWindow(1280, 720, "FlameFall", NULL, NULL);
		if (!window)
		{
			throw std::runtime_error("Failed to create window");
		}

		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error("Failed to initialize GLEW");
		}

		ShaderProgramWrapper shader_program;

		{
			ShaderWrapper vertex_transform_shader(GL_VERTEX_SHADER, "StandardShading.vert");
			vertex_transform_shader.compile();

			ShaderWrapper vertex_color_shader(GL_FRAGMENT_SHADER, "StandardShading.frag");
			vertex_color_shader.compile();

			shader_program.consumeShader(std::move(vertex_transform_shader));
			shader_program.consumeShader(std::move(vertex_color_shader));
		}

		shader_program.link();

        auto scene = Scene::import("flamefall.obj");

        BufferWrapper vertex_buffer(GL_ARRAY_BUFFER), normal_buffer(GL_ARRAY_BUFFER), element_buffer(GL_ELEMENT_ARRAY_BUFFER);
        vertex_buffer.setData(scene.getVertices());
        normal_buffer.setData(scene.getNormals());
        element_buffer.setData(scene.getIndices());

        std::atomic<glm::vec3> camera_location_atomic(glm::vec3(-50, 100, 0));
        std::atomic<glm::vec3> camera_look_at_atomic(glm::vec3(0, 70, 0));
        std::atomic<glm::vec3> light_location_atomic(glm::vec3(10.0f, 20.0f, 20.0f));
        std::thread console_reader_thread(std::bind(readInput,
            std::ref(camera_location_atomic),
            std::ref(camera_look_at_atomic),
            std::ref(light_location_atomic)));
        console_reader_thread.detach();

        shader_program.use();
        glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
        glEnable(GL_DEPTH_TEST);

		do
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto camera_location = camera_location_atomic.load(std::memory_order_consume);
            auto camera_look_at = camera_look_at_atomic.load(std::memory_order_consume);
            auto light_location = light_location_atomic.load(std::memory_order_consume);

            // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
            glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 400.0f);
            // Camera matrix
            glm::mat4 View = glm::lookAt(
                camera_location, // Camera is at (4,3,-3), in World Space
                camera_look_at, // and looks at the origin
                glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
            );
            // Model matrix : an identity matrix (model will be at the origin)
            glm::mat4 Model = glm::mat4(1.0f);
            // Our ModelViewProjection : multiplication of our 3 matrices
            glm::mat4 MVP = Projection * View * Model;

            Uniform::setValue(shader_program.getUniformId("M"), Model);
            Uniform::setValue(shader_program.getUniformId("V"), View);
            Uniform::setValue(shader_program.getUniformId("MVP"), MVP);
            Uniform::setValue(shader_program.getUniformId("LightLocation"), light_location);

            VertexAttribArrayLock vertex_arrtib_array_lock(vertex_buffer,
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0);          // array buffer offset
            VertexAttribArrayLock normal_arrtib_array_lock(normal_buffer,
                1,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0);          // array buffer offset

            element_buffer.bind();

            // Draw the triangles !
            glDrawElements(GL_TRIANGLES, scene.getIndices().size(), GL_UNSIGNED_INT, (void*)0);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		while (glfwWindowShouldClose(window) == 0);
	}
	catch (std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
		return -1;
	}

	return 0;
}