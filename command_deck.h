#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/modules/submenu.h>
#include <gui/modules/widget.h>
#include <gui/view_dispatcher.h>

#include "command_deck_api.h"
#include "command_deck_config.h"

#define COMMAND_DECK_VERSION "0.2.0"

typedef enum {
    CommandDeckViewMenu,
    CommandDeckViewStatus,
} CommandDeckView;

typedef enum {
    CommandDeckMenuNetwork,
    CommandDeckMenuComputer,
    CommandDeckMenuIrRemote,
    CommandDeckMenuRaspberryPi,
    CommandDeckMenuUtilities,
    CommandDeckMenuCustomActions,
    CommandDeckMenuSystemStatus,
} CommandDeckMenuItem;

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    Submenu* menu;
    Widget* status;
    CommandDeckConfig config;
} CommandDeckApp;
