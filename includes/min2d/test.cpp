#include "min2d.h"

using namespace Min2D;

int main() {
	Init(800, 600, "windowTitle");

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	Color clr = Min2D::GetPixel({20, 30});
	std::cout << (int)clr.red << " " << (int)clr.green << " " << (int)clr.blue << std::endl;
	while(!glfwWindowShouldClose(Min2D::window)) {
		Clear({255, 255, 255, 255});

		PutPixel({400, 300} , {0, 0, 0, 0}, 100, 100);
		glfwPollEvents();
		Refresh();
	}
	return EXIT_SUCCESS;
}
