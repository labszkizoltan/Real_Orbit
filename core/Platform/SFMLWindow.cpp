#include "SFMLWindow.h"

Window* Window::Create(const WindowProps& props)
{
    return new SfmlWindow(props);
}

SfmlWindow::SfmlWindow(const WindowProps& props)
{
	Init(props);
}

SfmlWindow::~SfmlWindow()
{
	Shutdown();
}

void SfmlWindow::OnUpdate()
{
	LOG_INFO("SfmlWindow updated");
}

void SfmlWindow::SetVSync(bool enabled)
{
}

bool SfmlWindow::IsVSync() const
{
	return false;
}

void SfmlWindow::Init(const WindowProps& props)
{
}

void SfmlWindow::Shutdown()
{
}
