#pragma once

#include <GLFW/glfw3.h>

namespace App
{
class Window
{
  public:
	Window();
	~Window();

	inline GLFWwindow &get()
	{
		return *window;
	}

	inline void setIsFramebufferResized(bool flag)
	{
		isFramebufferResized = flag;
	}
	inline bool getIsFramebufferResized()
	{
		return isFramebufferResized;
	}

  private:
	bool        isFramebufferResized = false;
	GLFWwindow *window;
};        // namespace Appclass AppWindow
}        // namespace App