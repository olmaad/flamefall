#pragma once

#include <memory>
#include <xstring>

class IResources
{
public:
	virtual ~IResources() = default;
	
	virtual void mount(const std::string_view mount_point) = 0;
	virtual std::istream&& get(const std::string_view path) = 0;
	
};

class Resource
{
	
};

class Resources : public IResources
{
public:
	void mount(const std::string_view mount_point) override;
	std::istream&& get(const std::string_view path) override;

private:
	std::unique_ptr<IResources> m_mount;
	
};
