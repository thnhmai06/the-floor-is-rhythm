#pragma once

namespace Core::Manager
{
	namespace System
	{
		void init();
		void quit();
	}
	namespace Config
	{
		void init();
		void quit();
	}
	namespace Window
	{
		void init();
		void quit();
	}
	namespace Renderer
	{
		void init();
		void quit();
	}
	namespace Mixer
	{
		void init();
		void quit();
	}
	namespace Resources
	{
		void init();
		void quit();
	}

	void init();
	void quit();
}