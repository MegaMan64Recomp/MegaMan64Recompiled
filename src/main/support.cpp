#include "zelda_support.h"
#include <SDL.h>
#include "nfd.h"
#include "RmlUi/Core.h"
#include "ultramodern/ultra64.h"

#if defined(__linux__)
#include "icon_bytes.h"
#include "../../lib/rt64/src/contrib/stb/stb_image.h"
#endif

namespace zelda64 {
    // MARK: - Internal Helpers
    void perform_file_dialog_operation(const std::function<void(bool, const std::filesystem::path&)>& callback) {
        nfdnchar_t* native_path = nullptr;
        nfdresult_t result = NFD_OpenDialogN(&native_path, nullptr, 0, nullptr);

        bool success = (result == NFD_OKAY);
        std::filesystem::path path;

        if (success) {
            path = std::filesystem::path{native_path};
            NFD_FreePathN(native_path);
        }

        callback(success, path);
    }

    // MARK: - Public API

    std::filesystem::path get_asset_path(const char* asset) {
        std::filesystem::path base_path = "";
#if defined(__APPLE__)
        base_path = get_bundle_resource_directory();
#endif

        return base_path / "assets" / asset;
    }

    void open_file_dialog(std::function<void(bool success, const std::filesystem::path& path)> callback) {
#ifdef __APPLE__
        dispatch_on_ui_thread([callback]() {
            perform_file_dialog_operation(callback);
        });
#else
        perform_file_dialog_operation(callback);
#endif
    }

    void show_error_message_box(const char *title, const char *message) {
#ifdef __APPLE__
    std::string title_copy(title);
    std::string message_copy(message);

    dispatch_on_ui_thread([title_copy, message_copy] {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title_copy.c_str(), message_copy.c_str(), nullptr);
    });
#else
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, nullptr);
#endif
    }

    std::string get_game_thread_name(const OSThread* t) {
        std::string name = "[Game] ";

        switch (t->id) {
            case 0:
                switch (t->priority) {
                    case 150:
                        name += "PIMGR";
                        break;

                    case 254:
                        name += "VIMGR";
                        break;

                    default:
                        name += std::to_string(t->id);
                        break;
                }
                break;

            case 1:
                name += "IDLE";
                break;

            case 2:
                switch (t->priority) {
                    case 5:
                        name += "SLOWLY";
                        break;

                    case 127:
                        name += "FAULT";
                        break;

                    default:
                        name += std::to_string(t->id);
                        break;
                }
                break;

            case 3:
                name += "MAIN";
                break;

            case 4:
                name += "GRAPH";
                break;

            case 5:
                name += "SCHED";
                break;

            case 7:
                name += "PADMGR";
                break;

            case 10:
                name += "AUDIOMGR";
                break;

            case 13:
                name += "FLASHROM";
                break;

            case 18:
                name += "DMAMGR";
                break;

            case 19:
                name += "IRQMGR";
                break;

            default:
                name += std::to_string(t->id);
                break;
        }

        return name;
    }

#if defined(__linux__)
    bool set_window_icon(SDL_Window* window) {
        // Read icon image data from the embedded icon_bytes array.
        int width, height, bytesPerPixel;
        void* data = stbi_load_from_memory(reinterpret_cast<const uint8_t*>(icon_bytes), sizeof(icon_bytes), &width, &height, &bytesPerPixel, 4);

        // Calculate pitch.
        int pitch = (width * 4 + 3) & ~3;

        // Set up channel masks based on byte order.
        int Rmask, Gmask, Bmask, Amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
        Rmask = 0x000000FF;
        Gmask = 0x0000FF00;
        Bmask = 0x00FF0000;
        Amask = 0xFF000000;
#else
        Rmask = 0xFF000000;
        Gmask = 0x00FF0000;
        Bmask = 0x0000FF00;
        Amask = 0x000000FF;
#endif

        SDL_Surface* surface = nullptr;
        if (data != nullptr) {
            surface = SDL_CreateRGBSurfaceFrom(data, width, height, 32, pitch, Rmask, Gmask, Bmask, Amask);
        }

        if (surface == nullptr) {
            if (data != nullptr) {
                stbi_image_free(data);
            }
            return false;
        } else {
            SDL_SetWindowIcon(window, surface);
            SDL_FreeSurface(surface);
            stbi_image_free(data);
            return true;
        }
    }
#endif
}
