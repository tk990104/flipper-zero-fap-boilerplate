#include "command_deck.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CommandDeckSettingMockMode,
    CommandDeckSettingHost,
    CommandDeckSettingPort,
    CommandDeckSettingReset,
} CommandDeckSetting;

static const char* const command_deck_toggle_labels[] = {
    "Off",
    "On",
};

static uint32_t command_deck_menu_back_callback(void* context) {
    UNUSED(context);
    return VIEW_NONE;
}

static uint32_t command_deck_status_back_callback(void* context) {
    UNUSED(context);
    return CommandDeckViewMenu;
}

static uint32_t command_deck_settings_back_callback(void* context) {
    UNUSED(context);
    return CommandDeckViewMenu;
}

static uint32_t command_deck_input_back_callback(void* context) {
    UNUSED(context);
    return CommandDeckViewSettings;
}

static void command_deck_save_config(CommandDeckApp* app) {
    if(!command_deck_storage_save(&app->config)) {
        FURI_LOG_E("CommandDeck", "Failed to save configuration");
    }
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

static void command_deck_settings_populate(CommandDeckApp* app);

static void command_deck_mock_mode_changed(VariableItem* item) {
    CommandDeckApp* app = variable_item_get_context(item);
    const uint8_t index = variable_item_get_current_value_index(item);

    app->config.mock_mode = (index == 1);
    variable_item_set_current_value_text(item, command_deck_toggle_labels[index]);
    command_deck_save_config(app);
}

static bool command_deck_host_validator(const char* text, FuriString* error, void* context) {
    UNUSED(context);

    for(const char* character = text; *character != '\0'; ++character) {
        const bool allowed =
            ((*character >= 'a') && (*character <= 'z')) ||
            ((*character >= 'A') && (*character <= 'Z')) ||
            ((*character >= '0') && (*character <= '9')) || (*character == '.') ||
            (*character == '-') || (*character == ':') || (*character == '[') ||
            (*character == ']');
        if(!allowed) {
            furi_string_set_str(error, "Use a hostname or IP");
            return false;
        }
    }

    return true;
}

static void command_deck_host_input_result(void* context) {
    CommandDeckApp* app = context;

    memcpy(app->config.companion_host, app->host_edit, strlen(app->host_edit) + 1);
    app->config.transport_enabled = false;
    command_deck_save_config(app);
    command_deck_settings_populate(app);
    view_dispatcher_switch_to_view(app->view_dispatcher, CommandDeckViewSettings);
}

static void command_deck_port_input_result(void* context, int32_t number) {
    CommandDeckApp* app = context;

    app->config.companion_port = (uint16_t)number;
    app->config.transport_enabled = false;
    command_deck_save_config(app);
    command_deck_settings_populate(app);
    view_dispatcher_switch_to_view(app->view_dispatcher, CommandDeckViewSettings);
}

static void command_deck_settings_enter_callback(void* context, uint32_t index) {
    CommandDeckApp* app = context;

    switch(index) {
    case CommandDeckSettingHost:
        memcpy(
            app->host_edit,
            app->config.companion_host,
            strlen(app->config.companion_host) + 1);
        text_input_reset(app->host_input);
        text_input_set_header_text(app->host_input, "Companion host or IP");
        text_input_set_minimum_length(app->host_input, 0);
        text_input_set_validator(app->host_input, command_deck_host_validator, app);
        text_input_set_result_callback(
            app->host_input,
            command_deck_host_input_result,
            app,
            app->host_edit,
            sizeof(app->host_edit),
            false);
        view_dispatcher_switch_to_view(app->view_dispatcher, CommandDeckViewHostInput);
        break;
    case CommandDeckSettingPort:
        number_input_set_header_text(app->port_input, "Companion port");
        number_input_set_result_callback(
            app->port_input,
            command_deck_port_input_result,
            app,
            app->config.companion_port ? app->config.companion_port : 80,
            1,
            UINT16_MAX);
        view_dispatcher_switch_to_view(app->view_dispatcher, CommandDeckViewPortInput);
        break;
    case CommandDeckSettingReset:
        command_deck_config_set_defaults(&app->config);
        command_deck_save_config(app);
        command_deck_settings_populate(app);
        break;
    case CommandDeckSettingMockMode:
    default:
        break;
    }
}

static void command_deck_settings_populate(CommandDeckApp* app) {
    variable_item_list_reset(app->settings);

    VariableItem* item = variable_item_list_add(
        app->settings, "Mock mode", 2, command_deck_mock_mode_changed, app);
    const uint8_t mock_mode_index = app->config.mock_mode ? 1 : 0;
    variable_item_set_current_value_index(item, mock_mode_index);
    variable_item_set_current_value_text(item, command_deck_toggle_labels[mock_mode_index]);

    item = variable_item_list_add(app->settings, "Companion host", 1, NULL, app);
    variable_item_set_current_value_text(
        item, app->config.companion_host[0] ? "Set" : "Not set");

    item = variable_item_list_add(app->settings, "Companion port", 1, NULL, app);
    if(app->config.companion_port) {
        snprintf(
            app->port_text, sizeof(app->port_text), "%u", app->config.companion_port);
        variable_item_set_current_value_text(item, app->port_text);
    } else {
        variable_item_set_current_value_text(item, "Not set");
    }

    item = variable_item_list_add(app->settings, "Reset defaults", 1, NULL, app);
    variable_item_set_current_value_text(item, "OK");
}

static void command_deck_menu_callback(void* context, uint32_t index) {
    furi_assert(context);
    CommandDeckApp* app = context;

    submenu_set_selected_item(app->menu, index);

    switch(index) {
    case CommandDeckMenuNetwork:
        command_deck_status_add_lines(
            app,
            "Network",
            command_deck_config_transport_label(&app->config),
            command_deck_config_mode_label(&app->config));
        break;
    case CommandDeckMenuComputer:
        command_deck_status_add_lines(app, "Computer", "Placeholder only", "No commands are run");
        break;
    case CommandDeckMenuIrRemote:
        command_deck_status_add_lines(app, "IR Remote", "Placeholder only", "No signals are sent");
        break;
    case CommandDeckMenuRaspberryPi:
        command_deck_status_add_lines(
            app,
            "Raspberry Pi",
            command_deck_api_route(CommandDeckApiEndpointStatus)->path,
            command_deck_api_result_label(
                command_deck_api_check(&app->config, CommandDeckApiEndpointStatus)));
        break;
    case CommandDeckMenuUtilities:
        command_deck_status_add_lines(
            app, "Utilities", "API contract: v1", "Safe routes: 2");
        break;
    case CommandDeckMenuCustomActions:
        command_deck_status_add_lines(
            app, "Custom Actions", "Allow-list empty", "Execution disabled");
        break;
    case CommandDeckMenuSystemStatus:
        command_deck_status_add_lines(
            app, "System Status", "Command Deck v" COMMAND_DECK_VERSION, "Config persisted");
        break;
    case CommandDeckMenuSettings:
        command_deck_settings_populate(app);
        view_dispatcher_switch_to_view(app->view_dispatcher, CommandDeckViewSettings);
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
    app->settings = variable_item_list_alloc();
    app->host_input = text_input_alloc();
    app->port_input = number_input_alloc();

    command_deck_config_set_defaults(&app->config);
    command_deck_storage_load(&app->config);
    app->config.transport_enabled = false;

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
    submenu_add_item(
        app->menu, "Settings", CommandDeckMenuSettings, command_deck_menu_callback, app);

    variable_item_list_set_enter_callback(
        app->settings, command_deck_settings_enter_callback, app);

    view_set_previous_callback(submenu_get_view(app->menu), command_deck_menu_back_callback);
    view_set_previous_callback(widget_get_view(app->status), command_deck_status_back_callback);
    view_set_previous_callback(
        variable_item_list_get_view(app->settings), command_deck_settings_back_callback);
    view_set_previous_callback(
        text_input_get_view(app->host_input), command_deck_input_back_callback);
    view_set_previous_callback(
        number_input_get_view(app->port_input), command_deck_input_back_callback);

    view_dispatcher_add_view(
        app->view_dispatcher, CommandDeckViewMenu, submenu_get_view(app->menu));
    view_dispatcher_add_view(
        app->view_dispatcher, CommandDeckViewStatus, widget_get_view(app->status));
    view_dispatcher_add_view(
        app->view_dispatcher,
        CommandDeckViewSettings,
        variable_item_list_get_view(app->settings));
    view_dispatcher_add_view(
        app->view_dispatcher,
        CommandDeckViewHostInput,
        text_input_get_view(app->host_input));
    view_dispatcher_add_view(
        app->view_dispatcher,
        CommandDeckViewPortInput,
        number_input_get_view(app->port_input));

    return app;
}

static void command_deck_free(CommandDeckApp* app) {
    furi_assert(app);

    command_deck_save_config(app);

    view_dispatcher_remove_view(app->view_dispatcher, CommandDeckViewPortInput);
    view_dispatcher_remove_view(app->view_dispatcher, CommandDeckViewHostInput);
    view_dispatcher_remove_view(app->view_dispatcher, CommandDeckViewSettings);
    view_dispatcher_remove_view(app->view_dispatcher, CommandDeckViewStatus);
    view_dispatcher_remove_view(app->view_dispatcher, CommandDeckViewMenu);

    number_input_free(app->port_input);
    text_input_free(app->host_input);
    variable_item_list_free(app->settings);
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
