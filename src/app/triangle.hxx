#pragma once

#include "app/vulkan/vulkan.hxx"
#include "app/window/window.hxx"

namespace App
{
class Triangle
{
  public:
	Triangle();
    ~Triangle();

	void run();

  private:
	void mainLoop();

  private:
    bool isFramebufferResized = false;

	Window window;
	Vulkan vulkan;
};
}        // namespace App