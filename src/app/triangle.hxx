#pragma once

#include "app/vulkan/vulkan.hxx"
#include "app/window/window.hxx"

namespace App
{
class Triangle
{
  public:
	Triangle();

	void run();

  private:
	void mainLoop();

  private:
	Window window;
	Vulkan vulkan;
};
}        // namespace App