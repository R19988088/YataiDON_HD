#include "song_select_script.h"
#include "../../libs/script.h"
#include <spdlog/spdlog.h>

SongSelectScript::SongSelectScript() {
    sol::state& lua = *script_manager.lua;

    if (!lua["SongSelect"].valid()) {
        std::string script_path = script_manager.get_lua_script_path("song_select");
        auto result = lua.script_file(script_path);
        if (!result.valid()) {
            sol::error err = result;
            spdlog::error("Error loading song_select.lua: {}", err.what());
            return;
        }
    }

    sol::table song_select_class = lua["SongSelect"];
    sol::protected_function new_func = song_select_class["new"];
    auto call_result = new_func();
    if (!call_result.valid()) {
        sol::error err = call_result;
        spdlog::error("Error calling SongSelect.new: {}", err.what());
        return;
    }

    lua_object           = call_result;
    fn_update            = lua_object["update"];
    fn_restart_text_fade = lua_object["restart_text_fade"];
    fn_draw_footer       = lua_object["draw_footer"];
    fn_draw_overlays     = lua_object["draw_overlays"];
}

void SongSelectScript::update(double current_ms) {
    auto result = fn_update(lua_object, current_ms);
    if (!result.valid()) {
        sol::error err = result;
        spdlog::error("Error calling SongSelect:update: {}", err.what());
    }
}

void SongSelectScript::restart_text_fade() {
    auto result = fn_restart_text_fade(lua_object);
    if (!result.valid()) {
        sol::error err = result;
        spdlog::error("Error calling SongSelect:restart_text_fade: {}", err.what());
    }
}

void SongSelectScript::draw_footer() {
    auto result = fn_draw_footer(lua_object);
    if (!result.valid()) {
        sol::error err = result;
        spdlog::error("Error calling SongSelect:draw_footer: {}", err.what());
    }
}

void SongSelectScript::draw_overlays(int state) {
    auto result = fn_draw_overlays(lua_object, state);
    if (!result.valid()) {
        sol::error err = result;
        spdlog::error("Error calling SongSelect:draw_overlays: {}", err.what());
    }
}
