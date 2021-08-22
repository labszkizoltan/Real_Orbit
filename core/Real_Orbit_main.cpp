
#include <iostream>

#include <core/Core.h>
#include <core/Application.h>


int main()
{
	Application* app = CreateApplication();

	app->Run();

	delete app;

    return 0;
}




#ifdef DY_PLATFORM_WINDOWS

extern Dyson::Application* Dyson::CreateApplication();

int main(int argc, char** argv)
{
	Dyson::Log::Init();

	DY_PROFILE_BEGIN_SESSION("Startup", "profiling_results/DysonProfile-Startup.json");
	Dyson::Application* app = Dyson::CreateApplication();
	DY_PROFILE_END_SESSION();

	DY_PROFILE_BEGIN_SESSION("Runtime", "profiling_results/DysonProfile-Runtime.json");
	app->Run();
	DY_PROFILE_END_SESSION();

	DY_PROFILE_BEGIN_SESSION("Shutdown", "profiling_results/DysonProfile-Shutdown.json");
	delete app;
	DY_PROFILE_END_SESSION();

}

#endif









