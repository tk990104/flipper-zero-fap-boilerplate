# Flipper Command Deck

Flipper Command Deck is a safe, menu-first Flipper Zero application scaffold. The `0.1.0` MVP establishes the navigation and application boundaries needed for future integrations without transmitting IR, making network requests, or running remote commands.

## MVP

The app opens directly to a top-level menu with seven sections:

- Network
- Computer
- IR Remote
- Raspberry Pi
- Utilities
- Custom Actions
- System Status

Each selection opens a read-only status screen. The placeholders deliberately perform no device, network, infrared, GPIO, or remote-computer action.

## Architecture

The MVP has a deliberately small surface:

1. `application.fam` declares the external application as `command_deck` and exposes the `command_deck_app` entry point.
2. `command_deck.c` owns the Flipper GUI records, a `ViewDispatcher`, the top-level `Submenu`, and one reusable status `Widget`.
3. `command_deck.h` contains the app state plus stable menu and view identifiers.
4. Selecting a menu item only replaces the text in the reusable status widget. Pressing Back returns to the menu; pressing Back from the menu exits.

Future integrations should sit behind explicit interfaces and configuration. Network and IR transports should remain separate from menu/navigation code, and custom actions should be allow-listed rather than accepting arbitrary commands.

## Reference repositories

These forks are references for later phases; none is linked into the MVP yet.

| Repository | Intended role |
| --- | --- |
| [`tk990104/FlipperHTTP`](https://github.com/tk990104/FlipperHTTP) | Future HTTP transport through a supported Wi-Fi board or companion device |
| [`tk990104/flipperzero-good-faps`](https://github.com/tk990104/flipperzero-good-faps) | Flipper app UI, lifecycle, storage, and API examples |
| [`tk990104/Flipper`](https://github.com/tk990104/Flipper) | Broader reference collection for patterns and utilities |
| [`tk990104/Flipper-IRDB`](https://github.com/tk990104/Flipper-IRDB) | Future user-selected IR data, with transmission kept out of the MVP |
| [`tk990104/flipper-zero-backpacks`](https://github.com/tk990104/flipper-zero-backpacks) | Future GPIO/ESP32 backpack and enclosure research |

## Build

Copy this directory into `applications_user/command_deck` in a compatible Flipper Zero firmware checkout, then run:

```text
./fbt launch_app APPSRC=applications_user/command_deck
```

The source also supports the standard uFBT workflow:

```text
ufbt
```

## Safety scope

This branch is intentionally non-operational. It does not include BadUSB payloads, Sub-GHz transmission, credential handling, arbitrary command execution, network requests, or IR transmission.

## Lineage

The `command-deck` branch was created from this fork's `main` branch so the original boilerplate history remains intact.
