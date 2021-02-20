#include "resources.h"

#include <fstream>

class FolderResources : public IResources
{
public:
	void mount(const std::string_view mount_point) override;
	std::istream&& get(const std::string_view path) override;
	
};

class ArchiveResources : public IResources
{
public:
	void mount(const std::string_view mount_point) override;
	std::istream&& get(const std::string_view path) override;
	
};

void FolderResources::mount(const std::string_view mount_point)
{
}

std::istream&& FolderResources::get(const std::string_view path)
{
	return std::ifstream();
}

void ArchiveResources::mount(const std::string_view mount_point)
{
}

std::istream&& ArchiveResources::get(const std::string_view path)
{
	return std::ifstream();
}

void Resources::mount(const std::string_view mount_point)
{
	
}

std::istream&& Resources::get(const std::string_view path)
{
	if (!m_mount)
	{
		throw std::runtime_error("Resources not mounted");
	}
	
	return std::ifstream();
}
