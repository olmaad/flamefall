#ifndef CONTEXT_WRAPPERS_H
#define CONTEXT_WRAPPERS_H

class GlfwInitContext
{
public:
	GlfwInitContext();
    GlfwInitContext(const GlfwInitContext&) = delete;
	~GlfwInitContext();

	operator bool()
	{
		return m_init_result;
	}

private:
	int m_init_result;

};

#endif // !CONTEXT_WRAPPERS_H
