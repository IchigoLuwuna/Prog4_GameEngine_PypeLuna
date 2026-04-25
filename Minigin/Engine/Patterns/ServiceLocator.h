#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H
#include <memory>

namespace dae
{
template <typename ServiceType>
class ServiceLocator final
{
public:
	static void RegisterService( std::unique_ptr<ServiceType>&& service )
	{
		m_pService = std::move( service );
	}
	static ServiceType& GetService()
	{
		return *m_pService;
	}

private:
	static std::unique_ptr<ServiceType> m_pService{};
};
} // namespace dae
#endif
