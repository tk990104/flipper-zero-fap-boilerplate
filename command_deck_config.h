#pragma once

#include <furi.h>

#define COMMAND_DECK_CONFIG_SCHEMA_VERSION 1U
#define COMMAND_DECK_COMPANION_HOST_MAX 64U

typedef struct {
    uint32_t schema_version;
    bool transport_enabled;
    bool mock_mode;
    char companion_host[COMMAND_DECK_COMPANION_HOST_MAX];
    uint16_t companion_port;
} CommandDeckConfig;

void command_deck_config_set_defaults(CommandDeckConfig* config);
bool command_deck_config_is_valid(const CommandDeckConfig* config);
const char* command_deck_config_transport_label(const CommandDeckConfig* config);
const char* command_deck_config_mode_label(const CommandDeckConfig* config);
