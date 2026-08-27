# Companion API contract

This document defines the future boundary between Flipper Command Deck and a trusted companion service, such as one running on a Raspberry Pi. Version 0.3.0 implements this contract as local mock data only; it contains no HTTP client and sends no requests.

## Base path and media type

- Base path: `/api/v1`
- Response media type: `application/json`
- Transport: not implemented
- Authentication: required before transport can be enabled in a future release

## Read-only routes

### `GET /api/v1/status`

Returns service readiness and mode.

```json
{
  "ok": true,
  "mode": "mock"
}
```

### `GET /api/v1/capabilities`

Returns only capabilities explicitly exposed by the companion. The MVP mock response has no actions.

```json
{
  "actions": [],
  "transport": false
}
```

## Future action boundary

Future actions must use stable identifiers returned by the capabilities route and matched against a local allow-list. The contract must never accept shell commands, executable paths, scripts, or arbitrary URLs from the Flipper.

An action transport is intentionally absent from version 0.3.0. Adding one requires:

1. explicit user configuration;
2. authenticated requests;
3. an allow-listed action identifier;
4. a confirmation policy for consequential actions;
5. bounded timeouts and response sizes; and
6. clear failure states without automatic retries for consequential actions.

## Configuration invariants

- Mock mode defaults to enabled.
- Transport defaults to disabled.
- Companion host defaults to empty.
- Companion port defaults to zero.
- Enabling transport without a host and non-zero port is invalid.
- The Settings UI cannot enable transport.
- Loading saved settings always forces transport off.
