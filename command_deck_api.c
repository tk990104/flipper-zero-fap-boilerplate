#include "command_deck_api.h"

#include <furi.h>

static const CommandDeckApiRoute command_deck_api_routes[CommandDeckApiEndpointCount] = {
    [CommandDeckApiEndpointStatus] =
        {
            .method = CommandDeckApiMethodGet,
            .path = "/api/v1/status",
        },
    [CommandDeckApiEndpointCapabilities] =
        {
            .method = CommandDeckApiMethodGet,
            .path = "/api/v1/capabilities",
        },
};

static const char* const command_deck_api_mock_responses[CommandDeckApiEndpointCount] = {
    [CommandDeckApiEndpointStatus] = "{\"ok\":true,\"mode\":\"mock\"}",
    [CommandDeckApiEndpointCapabilities] = "{\"actions\":[],\"transport\":false}",
};

const CommandDeckApiRoute* command_deck_api_route(CommandDeckApiEndpoint endpoint) {
    furi_check(endpoint < CommandDeckApiEndpointCount);
    return &command_deck_api_routes[endpoint];
}

const char* command_deck_api_mock_response(CommandDeckApiEndpoint endpoint) {
    furi_check(endpoint < CommandDeckApiEndpointCount);
    return command_deck_api_mock_responses[endpoint];
}

CommandDeckApiResult
    command_deck_api_check(const CommandDeckConfig* config, CommandDeckApiEndpoint endpoint) {
    furi_assert(config);
    furi_check(endpoint < CommandDeckApiEndpointCount);

    if(!command_deck_config_is_valid(config)) {
        return CommandDeckApiResultInvalidConfig;
    }

    if(config->mock_mode) {
        return CommandDeckApiResultMockReady;
    }

    if(!config->transport_enabled) {
        return CommandDeckApiResultTransportDisabled;
    }

    return CommandDeckApiResultTransportNotImplemented;
}

const char* command_deck_api_result_label(CommandDeckApiResult result) {
    switch(result) {
    case CommandDeckApiResultMockReady:
        return "Mock response ready";
    case CommandDeckApiResultTransportDisabled:
        return "Transport disabled";
    case CommandDeckApiResultInvalidConfig:
        return "Config invalid";
    case CommandDeckApiResultTransportNotImplemented:
        return "Transport not built";
    default:
        return "Unknown API state";
    }
}
