# Roadmap

This document tracks known issues, planned improvements, and future features for **Mega Man 64: Recompiled**.

Items are grouped by priority (High → Low) and category.

---

## Bug Fixes

| Priority | File | Description |
|----------|------|-------------|
| High | `src/main/main.cpp` | Add a UI error dialog (message box) inside `exit_error()` before exiting, so fatal errors are shown to the user instead of only printing to stderr. The existing `assert(false)` is intentional (debug break) and can remain. |
| High | `src/game/quicksaving.cpp` | Quick-save/load is gated behind `#if 0` — the RDRAM snapshot mechanism needs synchronisation review and re-enabling. |
| Medium | `src/ui/ui_renderer.cpp` | Font atlas is rebuilt from scratch on every UI reload; cache it to reduce hitching. |
| Medium | `src/main/main.cpp` | `queue_samples()` asserts `sample_count > duplicated_input_frames * input_channels`; handle small chunks gracefully. |
| Low | `src/game/controls.cpp` | `get_input_binding()` returns a reference to a `static` dummy field — callers that mutate it may cause unexpected behaviour. Return by value instead. |

---

## Error Handling & Robustness

| Priority | File | Description |
|----------|------|-------------|
| High | `src/main/main.cpp` | Surface RT64 / audio device setup failures to the user via a message-box before exiting. |
| Medium | `src/game/config.cpp` | `load_config()` / `save_config()` now log failures to stderr; surface critical failures through the UI as well. |
| Medium | `src/main/rt64_render_context.cpp` | Callers of `create_render_context()` should check `RT64RenderContext::valid()` and abort gracefully. |
| Low | `src/game/input.cpp` | SDL controller open/close events are handled, but the first-open path does not log errors for `SDL_GameControllerOpen()` failures. |

---

## Code Quality & Clean-up

| Priority | File | Description |
|----------|------|-------------|
| Medium | `src/main/main.cpp` | Remove the block of commented-out `REGISTER_FUNC` calls (lines ~613–625) once the overlay export mechanism is confirmed working or removed. |
| Medium | `src/main/main.cpp` | Move `get_game_thread_name()` out of `main.cpp` into a dedicated support file. |
| Medium | `src/game/input.cpp` | Encapsulate the large `InputState` static struct into a proper class with documented thread-safety guarantees. |
| Low | `src/main/main.cpp` | The `SetImageAsIcon()` helper (Linux only) should move to `src/main/support.cpp` to keep `main.cpp` focused on startup flow. |
| Low | `src/main/register_patches.cpp` | File is effectively empty (patch registration is commented out); either re-enable or document why it is disabled. |

---

## Missing Features & TODOs

| Priority | Feature | Notes |
|----------|---------|-------|
| High | Fullscreen toggle (Linux) | Currently implemented via a `SDL_SetWindowFullscreen` workaround. Remove once RT64 gains native fullscreen support on Linux. |
| High | Mod management UI | All discovered mods are auto-enabled at startup. A proper mod-browser UI is needed. |
| Medium | Mouse look | `// TODO mouse support` in `src/game/input.cpp` — mouse delta is computed but not hooked up to camera controls. |
| Medium | Adjustable deadzone | `// TODO adjustable deadzone threshold` — expose radial deadzone in the controls settings menu. |
| Medium | Preload executable (non-Windows) | `preload_executable()` is a no-op on Linux/macOS. Investigate `mlock()` / `posix_madvise(MADV_WILLNEED)` equivalents. |
| Low | macOS status | Build is supported but less tested. Add macOS to CI once GitHub Actions runners are configured. |

---

## Testing

| Priority | Area | Description |
|----------|------|-------------|
| High | `yaz0_decompress()` | Add unit tests for valid data, truncated input, and malformed back-references to validate the new bounds checks. |
| High | Config serialisation | Add unit tests for round-trip save/load of each config section, including missing keys and corrupt JSON. |
| Medium | Input mapping | Add unit tests for `assign_mapping()` / `get_input_binding()` with edge-case indices. |
| Medium | ROM validation | Add unit tests for `decompress_mm()` with wrong size, wrong header, and correct input. |
| Low | UI smoke tests | Investigate headless RmlUi rendering for automated UI regression testing. |

---

## Performance

| Priority | Area | Description |
|----------|------|-------------|
| Medium | Audio resampling | The `SDL_AudioCVT`-based resampler runs on the audio callback thread. Profile and consider replacing with a higher-quality async resampler. |
| Low | Config path lookup | `get_app_folder_path()` calls `getenv()` on every invocation. Cache the result after first call. |

---

## Documentation

| Priority | Item | Description |
|----------|------|-------------|
| Medium | Architecture overview | Add a section to `README.md` (or a separate `ARCHITECTURE.md`) describing how the recompiler output, patches, overlays, and runtime fit together. |
| Medium | Mod authoring guide | Document the `.rtz` texture pack format and the mod content type API. |
| Low | Code comments | Add function-level doc-comments to all public API headers in `include/`. |
