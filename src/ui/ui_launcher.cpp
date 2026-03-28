#include "recomp_ui.h"
#include "zelda_config.h"
#include "zelda_support.h"
#include "librecomp/game.hpp"
#include "ultramodern/ultramodern.hpp"
#include "RmlUi/Core.h"
#include "nfd.h"
#include <filesystem>
#include <array>

static std::string version_string;

Rml::DataModelHandle model_handle;
bool mm_rom_valid = false;

extern std::vector<recomp::GameEntry> supported_games;

static bool try_cache_local_rom() {
	const std::array<std::filesystem::path, 4> candidate_paths = {
		std::filesystem::path{"megaman64.us.z64"},
		std::filesystem::path{"Mega Man 64 (USA).z64"},
		std::filesystem::path{"megaman.n64.us.1.0.z64"},
		std::filesystem::path{"Mega Man 64.z64"},
	};

	for (const auto& candidate : candidate_paths) {
		if (!std::filesystem::exists(candidate)) {
			continue;
		}

		if (recomp::select_rom(candidate, supported_games[0].game_id) == recomp::RomValidationError::Good) {
			return true;
		}
	}

	for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
		if (!entry.is_regular_file()) {
			continue;
		}

		const std::filesystem::path extension = entry.path().extension();
		if (extension != ".z64" && extension != ".n64" && extension != ".v64") {
			continue;
		}

		if (recomp::select_rom(entry.path(), supported_games[0].game_id) == recomp::RomValidationError::Good) {
			return true;
		}
	}

	return false;
}

static void queue_autostart_game() {
	if (!mm_rom_valid || ultramodern::is_game_started()) {
		return;
	}

	SDL_Event event{};
	event.type = SDL_USEREVENT;
	event.user.code = recompui::launcher_autostart_event_code;
	recompui::queue_event(event);
}

void recompui::autostart_launcher_game() {
	if (!mm_rom_valid || ultramodern::is_game_started()) {
		return;
	}

	recomp::start_game(supported_games[0].game_id);
	recompui::set_current_menu(recompui::Menu::None);
}

void select_rom() {
	zelda64::open_file_dialog([](bool success, const std::filesystem::path& path) {
		if (success) {
			recomp::RomValidationError rom_error = recomp::select_rom(path, supported_games[0].game_id);
			switch (rom_error) {
				case recomp::RomValidationError::Good:
					mm_rom_valid = true;
					model_handle.DirtyVariable("mm_rom_valid");
					queue_autostart_game();
					break;
				case recomp::RomValidationError::FailedToOpen:
					recompui::message_box("Failed to open ROM file.");
					break;
				case recomp::RomValidationError::NotARom:
					recompui::message_box("This is not a valid ROM file.");
					break;
				case recomp::RomValidationError::IncorrectRom:
					recompui::message_box("This ROM is not the correct game.");
					break;
				case recomp::RomValidationError::NotYet:
					recompui::message_box("This game isn't supported yet.");
					break;
				case recomp::RomValidationError::IncorrectVersion:
					recompui::message_box(
							"This ROM is the correct game, but the wrong version.\nThis project requires the NTSC-U N64 version of the game.");
					break;
				case recomp::RomValidationError::OtherError:
					recompui::message_box("An unknown error has occurred.");
					break;
			}
		}
	});
}

class LauncherMenu : public recompui::MenuController {
public:
    LauncherMenu() {
		mm_rom_valid = recomp::is_rom_valid(supported_games[0].game_id);
		if (!mm_rom_valid) {
			mm_rom_valid = try_cache_local_rom();
		}
		queue_autostart_game();
    }
	~LauncherMenu() override {

	}
	Rml::ElementDocument* load_document(Rml::Context* context) override {
    	const std::filesystem::path asset = zelda64::get_asset_path("launcher.rml");
        return context->LoadDocument(asset.string());
	}
	void register_events(recompui::UiEventListenerInstancer& listener) override {
		recompui::register_event(listener, "select_rom",
			[](const std::string& param, Rml::Event& event) {
				select_rom();
			}
		);
		recompui::register_event(listener, "rom_selected",
			[](const std::string& param, Rml::Event& event) {
				mm_rom_valid = true;
				model_handle.DirtyVariable("mm_rom_valid");
			}
		);
		recompui::register_event(listener, "start_game",
			[](const std::string& param, Rml::Event& event) {
				recomp::start_game(supported_games[0].game_id);
				recompui::set_current_menu(recompui::Menu::None);
			}
		);
        recompui::register_event(listener, "open_controls",
			[](const std::string& param, Rml::Event& event) {
                recompui::set_current_menu(recompui::Menu::Config);
				recompui::set_config_submenu(recompui::ConfigSubmenu::Controls);
			}
		);
        recompui::register_event(listener, "open_settings",
			[](const std::string& param, Rml::Event& event) {
                recompui::set_current_menu(recompui::Menu::Config);
                recompui::set_config_submenu(recompui::ConfigSubmenu::General);
			}
		);
        recompui::register_event(listener, "exit_game",
			[](const std::string& param, Rml::Event& event) {
				ultramodern::quit();
			}
		);
	}
	void make_bindings(Rml::Context* context) override {
		Rml::DataModelConstructor constructor = context->CreateDataModel("launcher_model");

		constructor.Bind("mm_rom_valid", &mm_rom_valid);

		version_string = recomp::get_project_version().to_string();
		constructor.Bind("version_number", &version_string);

		model_handle = constructor.GetModelHandle();
	}
};

std::unique_ptr<recompui::MenuController> recompui::create_launcher_menu() {
    return std::make_unique<LauncherMenu>();
}
