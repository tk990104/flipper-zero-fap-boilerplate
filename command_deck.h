#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/modules/number_input.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>
#include <gui/view_dispatcher.h>

#include "command_deck_api.h"
#include "command_deck_config.h"
#include "command_deck_storage.h"

#define COMMAND_DECK_VERSION "0.3.0"

typedef enum {
    CommandDeckViewMenu,
    CommandDeckViewStatus,
    CommandDeckViewSettings,
    CommandDeckViewHostInput,
    CommandDeckViewPortInput,
} CommandDeckView;

typedef enum {
    CommandDeckMenuNetwork,
    CommandDeckMenuComputer,
    CommandDeckMenuIrRemote,
    CommandDeckMenuRaspberryPi,
    CommandDeckMenuUtilities,
    CommandDeckMenuCustomActions,
    CommandDeckMenuSystemStatus,
    CommandDeckMenuSettings,
} CommandDeckMenuItem;

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    Submenu* menu;
    Widget* status;
    VariableItemList* settings;
    TextInput* host_input;
    NumberInput* port_input;
    CommandDeckConfig config;
    char host_edit[COMMAND_DECK_COMPANION_HOST_MAX];
    char port_text[6];
} CommandDeckApp;
