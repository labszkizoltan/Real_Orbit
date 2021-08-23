
#ifndef SFMLWINDOW_H
#define SFMLWINDOW_H

#include <core/Window.h>
#include <SFML/Graphics.hpp>

class SfmlWindow : public Window
{
public:
	SfmlWindow(const WindowProps& props);
	virtual ~SfmlWindow() override;

	void OnUpdate() override;

	inline uint32_t GetWidth() const override { return m_Data.Width; };
	inline uint32_t GetHeight() const override { return m_Data.Height; };

	// Window attributes
	void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

	inline virtual void* GetNativeWindow() const { return m_Window; };

private:
	void Init(const WindowProps& props);
	void Shutdown();

private:

	sf::RenderWindow* m_Window;

	struct WindowData
	{
		std::string Title;
		uint32_t Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;

};







#endif // SFMLWINDOW_H
