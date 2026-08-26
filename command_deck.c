#include "command_deck.h"

#include <stdlib.h>

static uint32_t command_deck_menu_back_callback(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

static uint32_t command_deck_status_back_callback(void* context) {
    UNUSED(context);
    return CommandDeckViewMenu;
}

static void command_deck_status_add_lines(
    CommandDeckApp* app,
    const char* title,
    const char* line_one,
    const char* line_two) {
    widget_reset(app->status);
    widget_add_string_element(
        app->status, 64, 14, AlignCenter, AlignCenter, FontPrimary, title);
    widget_add_string_element(
        app->status, 64, 34, AlignCenter, AlignCenter, FontSecondary, line_one);
    widget_add_string_element(
        app->status, 64, 48, AlignCenter, AlignCenter, FontSecondary, line_two);
    view_dispatcher_switch_to_view(app->view_dispatcher, CommandDeckViewStatus);
}

static void command_deck_menu_callback(void* context, uint32_t index) {
    furi_assert(context);
    CommandDeckApp* app = context;

    submenu_set_selected_item(app->menu, index);

    switch(index) {
    case CommandDeckMenuNetwork:
        command_deck_status_add_lines(app, "Network", "Placeholder only", "No requests are sent");
        break;
    case CommandDeckMenuComputer:
        command_deck_status_add_lines(app, "Computer", "Placeholder only", "No commands are run");
        break;
    case CommandDeckMenuIrRemote:
        command_deck_status_add_lines(app, "IR Remote", "Placeholder only", "No signals are sent");
        break;
    case CommandDeckMenuRaspberryPi:
        command_deck_status_add_lines(app, "Raspberry Pi", "Placeholder only", "API not configured");
        break;
    case CommandDeckMenuUtilities:
        command_deck_status_add_lines(app, "Utilities", "Placeholder only", "Nothing runs yet");
        break;
    case CommandDeckMenuCustomActions:
        command_deck_status_add_lines(app, "Custom Actions", "No actions configured", "MVP safe mode");
        break;
    case CommandDeckMenuSystemStatus:
        command_deck_status_add_lines(
            app, "System Status", "Command Deck v" COMMAND_DECK_VERSION, "UI scaffold ready");
        break;
    default:
        command_deck_status_add_lines(app, "Command Deck", "Unknown selection", "Press Back");
        break;
    }
}

static CommandDeckApp* command_deck_alloc(void) {
    CommandDeckApp* app = malloc(sizeof(CommandDeckApp));

    app->gui = furi_record_open(RECORD_GUI);
    app->view_dispatcher = view_dispatcher_alloc();
    app->menu = submenu_alloc();
    app->status = widget_alloc();

    view_dispatcher_attach_to_gui(
        app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    submenu_set_header(app->menu, "Flipper Command Deck");
    submenu_add_item(
        app->menu, "Network", CommandDeckMenuNetwork, command_deck_menu_callback, app);
    submenu_add_item(
        app->menu, "Computer", CommandDeckMenuComputer, command_deck_menu_callback, app);
    submenu_add_item(
        app->menu, "IR Remote", CommandDeckMenuIrRemote, command_deck_menu_callback, app);
    submenu_add_item(
        app->menu, "Raspberry Pi", CommandDeckMenuRaspberryPi, command_deck_menu_callback, app);
    submenu_add_item(
        app->menu, "Utilities", CommandDeckMenuUtilities, command_deck_menu_callback, app);
    submenu_add_item(
        app->menu,
        "Custom Actions",
        CommandDeckMenuCustomActions,
        command_deck_menu_callback,
        app);
    submenu_add_item(
        app->menu,
        "System Status",
        CommandDeckMenuSystemStatus,
        command_deck_menu_callback,
        app);

    view_set_previous_callback(submenu_get_view(app->menu), command_deck_menu_back_callback);
    view_set_previous_callback(widget_get_view(app->status), command_deck_status_back_callback);

    view_dispatcher_add_view(
        app->view_dispatcher, CommandDeckViewMenu, submenu_get_view(app->menu));
    view_dispatcher_add_view(
        app->view_dispatcher, CommandDeckViewStatus, widget_get_view(app->status));

    return app;
}

static void command_deck_free(CommandDeckApp* app) {
    furi_assert(app);

    view_dispatcher_remove_view(app->view_dispatcher, CommandDeckViewStatus);
    view_dispatcher_remove_view(app->view_dispatcher, CommandDeckViewMenu);
    widget_free(app->status);
    submenu_free(app->menu);
    view_dispatcher_free(app->view_dispatcher);

    furi_record_close(RECORD_GUI);
    app->gui = NULL;

    free(app);
}

int32_t command_deck_app(void* context) {
    UNUSED(context);

    CommandDeckApp* app = command_deck_alloc();
    view_dispatcher_switch_to_view(app->view_dispatcher, CommandDeckViewMenu);
    view_dispatcher_run(app->view_dispatcher);
    command_deck_free(app);

    return 0;
}
