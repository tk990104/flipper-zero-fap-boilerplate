# Changelog

## 0.3.0

- Add an on-device Settings view for mock mode, companion host, and companion port.
- Persist settings in the app-scoped SD-card data folder using `APP_DATA_PATH`.
- Force transport off when defaults or saved settings are loaded.
- Add an official uFBT GitHub Actions build, artifact, and lint workflow.

## 0.2.0

- Add validated runtime configuration with mock mode enabled and transport disabled by default.
- Define read-only status and capabilities routes plus deterministic local mock responses.
- Surface configuration and mock API state in the existing menu screens.
- Document the companion API contract and its allow-list-only safety boundary.

## 0.1.0

- Rename the app and entry point to Flipper Command Deck.
- Replace the general-purpose boilerplate demonstrations with a focused seven-item menu.
- Add read-only placeholder/status screens for every MVP section.
- Document the architecture, reference repositories, build path, and safety scope.
