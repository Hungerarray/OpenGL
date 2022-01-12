#include "Application.h"

class myApplication : public Application {
public:
	myApplication(std::string_view title)
		: Application{title}
	{}

	void OnStartup() override {
	}

	void OnUpdate() override {
	}
};

int main() {
	myApplication app("window");
	app.Run();
	return EXIT_SUCCESS;
}
