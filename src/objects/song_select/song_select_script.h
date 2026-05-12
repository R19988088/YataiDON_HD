#pragma once
#include <sol/sol.hpp>

class SongSelectScript {
private:
    sol::table lua_object;
    sol::protected_function fn_update;
    sol::protected_function fn_restart_text_fade;
    sol::protected_function fn_draw_footer;
    sol::protected_function fn_draw_overlays;

public:
    SongSelectScript();
    void update(double current_ms);
    void restart_text_fade();
    void draw_footer();
    void draw_overlays(int state);
};
