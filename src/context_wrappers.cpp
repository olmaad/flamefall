#include "context_wrappers.h"

#include <GLFW/glfw3.h>

GlfwInitContext::GlfwInitContext()
{
	m_init_result = glfwInit();
}

GlfwInitContext::~GlfwInitContext()
{
	if (m_init_result)
	{
		glfwTerminate();
	}
}

