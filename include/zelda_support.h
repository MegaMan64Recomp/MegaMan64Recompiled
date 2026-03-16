#ifndef __ZELDA_SUPPORT_H__
#define __ZELDA_SUPPORT_H__

#include <functional>
#include <filesystem>
#include <optional>
#include <string>

struct OSThread;
struct SDL_Window;

namespace zelda64 {
    std::filesystem::path get_asset_path(const char* asset);
    void open_file_dialog(std::function<void(bool success, const std::filesystem::path& path)> callback);
    void show_error_message_box(const char *title, const char *message);
    std::string get_game_thread_name(const OSThread* t);

#if defined(__linux__)
    bool set_window_icon(SDL_Window* window);
#endif

// Apple specific methods that usually require Objective-C. Implemented in support_apple.mm.
#ifdef __APPLE__
    void dispatch_on_ui_thread(std::function<void()> func);
    std::optional<std::filesystem::path> get_application_support_directory();
    std::filesystem::path get_bundle_resource_directory();
    std::filesystem::path get_bundle_directory();
#endif
}

#endif
