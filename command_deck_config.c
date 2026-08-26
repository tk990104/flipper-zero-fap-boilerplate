#include "command_deck_config.h"

#include <string.h>

void command_deck_config_set_defaults(CommandDeckConfig* config) {
    furi_assert(config);

    config->schema_version = COMMAND_DECK_CONFIG_SCHEMA_VERSION;
    config->transport_enabled = false;
    config->mock_mode = true;
    config->companion_host[0] = '\0';
    config->companion_port = 0;
}

bool command_deck_config_is_valid(const CommandDeckConfig* config) {
    furi_assert(config);

    if(config->schema_version != COMMAND_DECK_CONFIG_SCHEMA_VERSION) {
        return false;
    }

    if(!memchr(config->companion_host, '\0', sizeof(config->companion_host))) {
        return false;
    }

    if(config->transport_enabled &&
       ((config->companion_host[0] == '\0') || (config->companion_port == 0))) {
        return false;
    }

    return true;
}

const char* command_deck_config_transport_label(const CommandDeckConfig* config) {
    furi_assert(config);
    return config->transport_enabled ? "Transport: Enabled" : "Transport: Disabled";
}

const char* command_deck_config_mode_label(const CommandDeckConfig* config) {
    furi_assert(config);
    return config->mock_mode ? "Mock mode: On" : "Mock mode: Off";
}
