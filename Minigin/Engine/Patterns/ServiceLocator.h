#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H
#include <memory>
#include "Singleton.h"

namespace dae
{
template <typename ServiceType>
class ServiceLocator final : Singleton<ServiceLocator<ServiceType>>
{
public:
	void RegisterService( std::unique_ptr<ServiceType>&& service )
	{
		m_pService = std::move( service );
	}
	ServiceType& GetService()
	{
		return *m_pService;
	}

private:
	std::unique_ptr<ServiceType> m_pService{};
};
} // namespace dae
#endif
