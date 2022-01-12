#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <string_view>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera.hpp"

// Callback definition
void framebufferCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

class Application {
	public:
		void Run() {
			glfwSetWindowUserPointer(mWindow, (void *)this);
			SetCallbacks();

			OnStartup();

			glEnable(GL_DEPTH_TEST);
			lastFrame = glfwGetTime();

			while(!glfwWindowShouldClose(mWindow)) {
				float currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				OnUpdate();

				glfwSwapBuffers(mWindow);
				glfwPollEvents();
			}
		}

		Application(std::string_view windowName) {
			SetupGlfw(windowName);
			LoadGlad();

			glViewport(0, 0, scrWidth, scrHeight);
		}

		~Application() {
			glfwTerminate();
		}

	protected:
		virtual void OnStartup() = 0;
		virtual void OnUpdate() = 0;

		void SetCallbacks() {
			glfwSetFramebufferSizeCallback(mWindow, framebufferCallback);
			glfwSetKeyCallback(mWindow, keyCallback);
			glfwSetCursorPosCallback(mWindow, mouseCallback);
			glfwSetScrollCallback(mWindow, scrollCallback);
		}

	protected:
		Camera camera;

		float deltaTime;

		int scrWidth;
		int scrHeight;

		friend void framebufferCallback(GLFWwindow* window, int width, int height);
		friend void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
		friend void mouseCallback(GLFWwindow *window, double xpos, double ypos);
		friend void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

	private:

		void SetupGlfw(std::string_view windowName) {
			// Initializing glfw
			if(!glfwInit()) {
				std::cerr << "GLFW::FAILED_TO_INIT" << std::endl;
				exit(EXIT_FAILURE);
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			GLFWmonitor *primary = glfwGetPrimaryMonitor();
			const GLFWvidmode *mode = glfwGetVideoMode(primary);

			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			scrWidth = mode->width;
			scrHeight = mode->height;

			mWindow = glfwCreateWindow(scrWidth, scrHeight, windowName.data(), nullptr, nullptr);
			if(mWindow == nullptr) {
				std::cerr << "GLFW::FAILED_TO_CREATE_WINDOW" << std::endl;
				exit(EXIT_FAILURE);
			}
			glfwMakeContextCurrent(mWindow);

			glfwFocusWindow(mWindow);
			glfwSetInputMode(mWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			//glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		void LoadGlad() {
			if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cerr << "GLAD::FAILED_TO_INIT" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

	private:
		GLFWwindow *mWindow;
		float lastFrame;
};

inline void framebufferCallback(GLFWwindow *window, int width, int height) {
	Application *app = (Application *) glfwGetWindowUserPointer(window);

	app->scrWidth = width;
	app->scrHeight = height;
	glViewport(0, 0, width, height);
}

inline void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if(action == GLFW_RELEASE) {
		return ;
	}

	Application *app = (Application *) glfwGetWindowUserPointer(window);
	Camera &camera = app->camera;
	float deltaTime = app->deltaTime;

	switch(key) {
		case GLFW_KEY_W:
			camera.ProcessKeyboard(FORWARD, deltaTime);
			break;
		case GLFW_KEY_S:
			camera.ProcessKeyboard(BACKWARD, deltaTime);
			break;
		case GLFW_KEY_A:
			camera.ProcessKeyboard(LEFT, deltaTime);
			break;
		case GLFW_KEY_D:
			camera.ProcessKeyboard(RIGHT, deltaTime);
			break;
	}
}

inline void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	static double lastx, lasty;
	static bool firstMouse = true;

	if (firstMouse) {
		lastx = xpos;
		lasty = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastx;
	float yoffset = ypos - lasty;

	lastx = xpos;
	lasty = ypos;

	Application *app = (Application *) glfwGetWindowUserPointer(window);
	app->camera.ProcessMouseMovement(xoffset, yoffset);
}

inline void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
	Application *app = (Application *) glfwGetWindowUserPointer(window);
	app->camera.ProcessMouseScroll(yoffset);
}
#endif
