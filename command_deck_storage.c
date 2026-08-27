#include "command_deck_storage.h"

#include <flipper_format/flipper_format.h>
#include <storage/storage.h>
#include <string.h>

#define COMMAND_DECK_CONFIG_PATH          APP_DATA_PATH("config.txt")
#define COMMAND_DECK_CONFIG_HEADER        "Flipper Command Deck Config"
#define COMMAND_DECK_CONFIG_FILE_VERSION  1U
#define COMMAND_DECK_CONFIG_KEY_MOCK_MODE "MockMode"
#define COMMAND_DECK_CONFIG_KEY_HOST      "CompanionHost"
#define COMMAND_DECK_CONFIG_KEY_PORT      "CompanionPort"

bool command_deck_storage_load(CommandDeckConfig* config) {
    furi_assert(config);

    bool loaded = false;
    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* file = flipper_format_file_alloc(storage);
    FuriString* file_type = furi_string_alloc();
    FuriString* host = furi_string_alloc();

    do {
        if(!flipper_format_file_open_existing(file, COMMAND_DECK_CONFIG_PATH)) {
            break;
        }

        uint32_t version = 0;
        if(!flipper_format_read_header(file, file_type, &version) ||
           !furi_string_equal_str(file_type, COMMAND_DECK_CONFIG_HEADER) ||
           (version != COMMAND_DECK_CONFIG_FILE_VERSION)) {
            break;
        }

        CommandDeckConfig candidate;
        command_deck_config_set_defaults(&candidate);

        uint32_t mock_mode = 1;
        if(flipper_format_read_uint32(file, COMMAND_DECK_CONFIG_KEY_MOCK_MODE, &mock_mode, 1)) {
            candidate.mock_mode = (mock_mode != 0);
        }

        uint32_t port = 0;
        if(flipper_format_read_uint32(file, COMMAND_DECK_CONFIG_KEY_PORT, &port, 1) &&
           (port <= UINT16_MAX)) {
            candidate.companion_port = (uint16_t)port;
        }

        if(flipper_format_read_string(file, COMMAND_DECK_CONFIG_KEY_HOST, host) &&
           (furi_string_size(host) < sizeof(candidate.companion_host))) {
            memcpy(
                candidate.companion_host, furi_string_get_cstr(host), furi_string_size(host) + 1);
        }

        candidate.transport_enabled = false;
        if(!command_deck_config_is_valid(&candidate)) {
            break;
        }

        *config = candidate;
        loaded = true;
    } while(false);

    flipper_format_free(file);
    furi_string_free(host);
    furi_string_free(file_type);
    furi_record_close(RECORD_STORAGE);
    return loaded;
}

bool command_deck_storage_save(const CommandDeckConfig* config) {
    furi_assert(config);

    if(!command_deck_config_is_valid(config)) {
        return false;
    }

    bool saved = false;
    Storage* storage = furi_record_open(RECORD_STORAGE);
    FlipperFormat* file = flipper_format_file_alloc(storage);

    do {
        if(!flipper_format_file_open_always(file, COMMAND_DECK_CONFIG_PATH)) {
            break;
        }

        const uint32_t mock_mode = config->mock_mode ? 1U : 0U;
        const uint32_t port = config->companion_port;
        if(!flipper_format_write_header_cstr(
               file, COMMAND_DECK_CONFIG_HEADER, COMMAND_DECK_CONFIG_FILE_VERSION) ||
           !flipper_format_write_uint32(file, COMMAND_DECK_CONFIG_KEY_MOCK_MODE, &mock_mode, 1) ||
           !flipper_format_write_string_cstr(
               file, COMMAND_DECK_CONFIG_KEY_HOST, config->companion_host) ||
           !flipper_format_write_uint32(file, COMMAND_DECK_CONFIG_KEY_PORT, &port, 1)) {
            break;
        }

        saved = true;
    } while(false);

    flipper_format_free(file);
    furi_record_close(RECORD_STORAGE);
    return saved;
}
