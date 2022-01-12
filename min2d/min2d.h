#pragma once

#ifndef MIN_2D_H
#define MIN_2D_H
#include <iostream>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Min2D {
	GLFWwindow *window {nullptr};

	uint32_t wWidth;
	uint32_t wHeight;

	uint32_t shaderProgram;

	struct Color {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};

	struct Point {
		uint32_t x;
		uint32_t y;
	};

	const char *vssource = R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;

		void main() {
			gl_Position = vec4(aPos, 1.0);
		}
	)";

	const char *fgsource = R"(
		#version 330 core

		out vec4 FragColor;
		uniform vec4 inColor;

		void main() {
			FragColor = inColor;
		}
	)";

	inline void framebufferCallback(GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
		wWidth = width;
		wHeight = height;
	}

	inline void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}

	inline void Init(uint32_t width, uint32_t height, std::string_view title, bool fullscreen = false) {
		// 1. Getting window on screen with glfw
		if(!glfwInit()) {
			std::cerr << "ERROR::glfw failed to init" << std::endl;
			exit(EXIT_FAILURE);
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		if(fullscreen) {
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode *md = glfwGetVideoMode(monitor);
		 // unknown if the error I thought occurs ..

			window = glfwCreateWindow(width, height, title.data(), glfwGetPrimaryMonitor(), nullptr);
		} else {
			window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		}
		if(!window) {
			std::cerr << "ERROR::glfw failed to create window" << std::endl;
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
		wWidth = width;
		wHeight = height;

		// 2. Loading GLAD for gl functions
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "ERROR::glad failed to load" << std::endl;
			exit(EXIT_FAILURE);
		}

		// 3. extra utilities
		glViewport(0, 0, width, height);
		glfwSetFramebufferSizeCallback(window, framebufferCallback);
		glfwSetKeyCallback(window, keyCallback);


		// 4. setting up for 2d manipulation
		float vertices[] = {
			 -1.0f,  -1.0f,  0.0f,
  			1.0f,  -1.0f,  0.0f,
  			1.0f,   1.0f,  0.0f,
			 -1.0f,  -1.0f,  0.0f,
  			1.0f,   1.0f,  0.0f,
			 -1.0f,   1.0f,  0.0f
		};

		uint32_t vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		uint32_t vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		uint32_t vsShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vsShader, 1, &vssource, nullptr);
		glCompileShader(vsShader);

		uint32_t fgShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fgShader, 1, &fgsource, nullptr);
		glCompileShader(fgShader);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vsShader);
		glAttachShader(shaderProgram, fgShader);
		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);

		glDeleteShader(vsShader);
		glDeleteShader(fgShader);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	inline void Clear(const Color &clr) {
		glClearColor(clr.red / 255.0f, clr.green / 255.0f, clr.blue / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	inline void Shutdown() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	inline Color GetPixel(const Point &pt) {
		Color clr;
		glReadPixels(pt.x, pt.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &clr);

		return clr;
	}

	void PutPixel(const Point &pt, const Color &clr, uint32_t width = 1, uint32_t height = 1) {
		int32_t loc = glGetUniformLocation(shaderProgram, "inColor");
		glUniform4f(loc, clr.red / 255.0f, clr.green / 255.0f, clr.blue / 255.0f, 1.0f);

		glEnable(GL_SCISSOR_TEST);
		glScissor(pt.x, pt.y, width, height);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_SCISSOR_TEST);
	}

	inline void Refresh() {
		glfwSwapBuffers(window);
	}
}
#endif
