#include <cocaine/framework/application.hpp>

#include <cocaine/format.hpp>

using namespace cocaine::framework;

application_t::application_t(const std::string& name,
                             std::shared_ptr<service_manager_t> service_manager) :
    m_name(name),
    m_service_manager(service_manager)
{
    // pass
}

std::shared_ptr<base_handler_t>
application_t::invoke(const std::string& event,
                      std::shared_ptr<cocaine::api::stream_t> response)
{
    auto it = m_handlers.find(event);

    if (it != m_handlers.end()) {
        std::shared_ptr<base_handler_t> new_handler = it->second->make_handler();
        new_handler->invoke(event, response);
        return new_handler;
    } else if (m_default_handler) {
        std::shared_ptr<base_handler_t> new_handler = m_default_handler->make_handler();
        new_handler->invoke(event, response);
        return new_handler;
    } else {
        throw std::runtime_error(
            cocaine::format("Unrecognized event %s has been accepted.", event)
        );
    }
}

void
application_t::on(const std::string& event,
                  std::shared_ptr<base_factory_t> factory)
{
    m_handlers[event] = factory;
}

void
application_t::on_unregistered(std::shared_ptr<base_factory_t> factory) {
    m_default_handler = factory;
}