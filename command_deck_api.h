#pragma once

#include "command_deck_config.h"

typedef enum {
    CommandDeckApiMethodGet,
} CommandDeckApiMethod;

typedef enum {
    CommandDeckApiEndpointStatus,
    CommandDeckApiEndpointCapabilities,
    CommandDeckApiEndpointCount,
} CommandDeckApiEndpoint;

typedef enum {
    CommandDeckApiResultMockReady,
    CommandDeckApiResultTransportDisabled,
    CommandDeckApiResultInvalidConfig,
    CommandDeckApiResultTransportNotImplemented,
} CommandDeckApiResult;

typedef struct {
    CommandDeckApiMethod method;
    const char* path;
} CommandDeckApiRoute;

const CommandDeckApiRoute* command_deck_api_route(CommandDeckApiEndpoint endpoint);
const char* command_deck_api_mock_response(CommandDeckApiEndpoint endpoint);
CommandDeckApiResult
    command_deck_api_check(const CommandDeckConfig* config, CommandDeckApiEndpoint endpoint);
const char* command_deck_api_result_label(CommandDeckApiResult result);
