#pragma once

#include <GLFW/glfw3.h>

namespace App
{
class Window
{
  public:
	void init(bool &isFramebufferResized);
	void destroy();

	inline GLFWwindow &get()
	{
		return *window;
	}

	inline void setFramebufferResized(bool flag)
	{
		*isFramebufferResized = flag;
	}

  private:
	bool       *isFramebufferResized;
	GLFWwindow *window;
};        // namespace Appclass AppWindow
}        // namespace App