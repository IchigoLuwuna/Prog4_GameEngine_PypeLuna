#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H
#include <memory>
#include <stdexcept>
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

	template <typename ServiceLayerType>
		requires requires( std::unique_ptr<ServiceType>&& underlyingService ) {
			ServiceLayerType( std::move( underlyingService ) );
		}
	void AddLayer()
	{
		auto serviceLayer{ std::make_unique<ServiceLayerType>( std::move( m_pService ) ) };
		m_pService = std::move( serviceLayer );
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

	// Base service class also serves as null-service as a byproduct of how the locator works
	std::unique_ptr<ServiceType> m_pService{ std::make_unique<ServiceType>() };
};
} // namespace dae
#endif
