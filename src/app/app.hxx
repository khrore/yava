#pragma once

#include "app/vulkan/vulkan.hxx"
#include "app/window/window.hxx"

namespace App
{
class App
{
  public:
	App();

	void run();

  private:
	void mainLoop();

  private:
	Window window;
	Vulkan vulkan;
};
}        // namespace App