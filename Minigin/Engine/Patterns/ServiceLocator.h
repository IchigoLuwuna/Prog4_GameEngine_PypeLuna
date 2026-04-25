#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H
#include <memory>
#include "Singleton.h"

namespace dae
{
template <typename ServiceType>
class ServiceLocator final : public Singleton<ServiceLocator<ServiceType>>
{
public:
	void RegisterService( std::unique_ptr<ServiceType>&& service )
	{
		m_pService = std::move( service );
	}
	ServiceType& GetService()
	{
#ifndef NDEBUG
		if ( !m_pService )
		{
			throw std::runtime_error( "Attempted to access unregistered service" );
		}
#endif

		return *m_pService;
	}

private:
	friend class Singleton<ServiceLocator<ServiceType>>;

	std::unique_ptr<ServiceType> m_pService{};
};
} // namespace dae
#endif
