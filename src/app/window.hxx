#pragma once

#include "GLFW/glfw3.h"

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

  private:
	GLFWwindow *window;
};        // namespace Appclass AppWindow
}        // namespace App