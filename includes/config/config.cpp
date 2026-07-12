#include "config.h"

#include <array>
#include <climits>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

namespace {

std::filesystem::path normalized_executable_directory(
    const std::filesystem::path& executable
) {
    std::error_code error;
    std::filesystem::path normalized = std::filesystem::weakly_canonical(
        executable,
        error
    );
    if (error) {
        error.clear();
        normalized = std::filesystem::absolute(executable, error);
    }
    if (error) {
        throw std::runtime_error(
            "Unable to resolve the graphing calculator executable path: " +
            error.message()
        );
    }
    return normalized.parent_path();
}

std::filesystem::path executable_directory() {
#if defined(_WIN32)
    std::vector<wchar_t> buffer(32768);
    const DWORD length = GetModuleFileNameW(
        nullptr,
        buffer.data(),
        static_cast<DWORD>(buffer.size())
    );
    if (length == 0 || length == buffer.size()) {
        throw std::runtime_error(
            "Unable to resolve the graphing calculator executable path"
        );
    }
    return normalized_executable_directory(
        std::filesystem::path(std::wstring(buffer.data(), length))
    );
#elif defined(__APPLE__)
    std::uint32_t size = PATH_MAX;
    std::vector<char> buffer(size);
    if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
        buffer.resize(size);
        if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
            throw std::runtime_error(
                "Unable to resolve the graphing calculator executable path"
            );
        }
    }
    return normalized_executable_directory(buffer.data());
#elif defined(__linux__)
    std::array<char, PATH_MAX> buffer{};
    const ssize_t length = readlink(
        "/proc/self/exe",
        buffer.data(),
        buffer.size() - 1
    );
    if (length <= 0) {
        throw std::runtime_error(
            "Unable to resolve the graphing calculator executable path"
        );
    }
    return normalized_executable_directory(
        std::filesystem::path(std::string(buffer.data(), length))
    );
#else
    throw std::runtime_error(
        "Executable-relative asset loading is not supported on this platform"
    );
#endif
}

std::filesystem::path runtime_asset_path(const std::string& filename) {
    return executable_directory() / "assets" / filename;
}

} // namespace

Config::Config() {
    static_cast<void>(this->_ui_font());
}

sf::Font& Config::_ui_font() {
    static sf::Font font = [] {
        const std::filesystem::path path = runtime_asset_path(
            "Poppins-Regular.ttf"
        );
        sf::Font loaded;
        if (!loaded.loadFromFile(path.string())) {
            throw std::runtime_error(
                "Unable to load the packaged UI font at " + path.string() +
                ". Keep the assets directory beside the executable or rebuild "
                "the application."
            );
        }
        return loaded;
    }();
    return font;
}

sf::Font& Config::get_font(int index) {
    if (index != UI_FONT) {
        throw std::out_of_range(
            "Unknown graphing calculator font resource: " +
            std::to_string(index)
        );
    }
    return this->_ui_font();
}
