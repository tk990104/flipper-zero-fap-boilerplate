#pragma once

#include "command_deck_config.h"

bool command_deck_storage_load(CommandDeckConfig* config);
bool command_deck_storage_save(const CommandDeckConfig* config);
