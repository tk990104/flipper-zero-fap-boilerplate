#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <gui/modules/submenu.h>
#include <gui/modules/widget.h>
#include <gui/view_dispatcher.h>

#define COMMAND_DECK_VERSION "0.1.0"

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
} CommandDeckApp;
