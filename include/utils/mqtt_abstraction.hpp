// SPDX-License-Identifier: Apache-2.0
// Copyright 2020 - 2023 Pionix GmbH and Contributors to EVerest
#ifndef UTILS_MQTT_ABSTRACTION_HPP
#define UTILS_MQTT_ABSTRACTION_HPP

#include <future>

#include <nlohmann/json.hpp>

#include <utils/types.hpp>

namespace Everest {
using json = nlohmann::json;

class MQTTAbstractionImpl;

///
/// \brief Contains a C++ abstraction for using MQTT in EVerest modules
///
class MQTTAbstraction {

private:
    MQTTAbstraction(const std::string& mqtt_server_address, const std::string& mqtt_server_port,
                    const std::string& mqtt_everest_prefix, const std::string& mqtt_external_prefix);
    MQTTAbstractionImpl& mqtt_abstraction;

public:
    ///
    /// \copydoc MQTTAbstractionImpl::connect()
    bool connect();

    ///
    /// \copydoc MQTTAbstractionImpl::disconnect()
    void disconnect();

    ///
    /// \copydoc MQTTAbstractionImpl::publish(const std::string&, const json&)
    void publish(const std::string& topic, const json& json);

    ///
    /// \copydoc MQTTAbstractionImpl::publish(const std::string&, const json&, QOS)
    void publish(const std::string& topic, const json& json, QOS qos);

    ///
    /// \copydoc MQTTAbstractionImpl::publish(const std::string&, const std::string&)
    void publish(const std::string& topic, const std::string& data);

    ///
    /// \copydoc MQTTAbstractionImpl::publish(const std::string&, const std::string&, QOS)
    void publish(const std::string& topic, const std::string& data, QOS qos);

    ///
    /// \copydoc MQTTAbstractionImpl::subscribe(const std::string&)
    void subscribe(const std::string& topic);

    ///
    /// \copydoc MQTTAbstractionImpl::subscribe(const std::string&, QOS)
    void subscribe(const std::string& topic, QOS qos);

    ///
    /// \copydoc MQTTAbstractionImpl::unsubscribe(const std::string&)
    void unsubscribe(const std::string& topic);

    ///
    /// \copydoc MQTTAbstractionImpl::spawn_main_loop_thread()
    std::future<void> spawn_main_loop_thread();

    ///
    /// \copydoc MQTTAbstractionImpl::register_handler(const std::string&, std::shared_ptr<TypedHandler>, QOS)
    void register_handler(const std::string& topic, std::shared_ptr<TypedHandler> handler, QOS qos);

    ///
    /// \copydoc MQTTAbstractionImpl::unregister_handler(const std::string&, const Token&)
    void unregister_handler(const std::string& topic, const Token& token);

    ///
    /// \returns the instance of the MQTTAbstraction singleton taking a \p mqtt_server_address , \p mqtt_server_port ,
    /// \p mqtt_everest_prefix and \p mqtt_external_prefix as parameters
    static MQTTAbstraction& get_instance(const std::string& mqtt_server_address, const std::string& mqtt_server_port,
                                         const std::string& mqtt_everest_prefix,
                                         const std::string& mqtt_external_prefix) {
        static MQTTAbstraction instance(mqtt_server_address, mqtt_server_port, mqtt_everest_prefix,
                                        mqtt_external_prefix);

        return instance;
    }

    MQTTAbstraction(MQTTAbstraction const&) = delete;
    void operator=(MQTTAbstraction const&) = delete;
};
} // namespace Everest

#endif // UTILS_MQTT_ABSTRACTION_HPP
