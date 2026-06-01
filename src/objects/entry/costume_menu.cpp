#include "costume_menu.h"
#include "../../libs/texture.h"
#include "../../libs/input.h"
#include "../../libs/audio.h"

CostumeMenu::CostumeMenu(PlayerNum player_num) : player_num(player_num), is_2p(player_num == PlayerNum::P2) {
    auto& info = tex.skin_config[SC::ENTRY_COSTUME_TEXT];
    std::string lang = global_data.config->general.language;
    title_text = std::make_unique<OutlinedText>(
        info.text[lang], static_cast<int>(info.font_size),
        ray::WHITE, ray::BLACK, false);

    blue_arrow_fade = (FadeAnimation*)tex.get_animation(13, true);
    blue_arrow_move = (MoveAnimation*)tex.get_animation(14, true);
    blue_arrow_fade->start();
    blue_arrow_move->start();
}

CostumeMenu::~CostumeMenu() {
    for (auto& icon : costume_icons)
        ray::UnloadTexture(icon);
}

void CostumeMenu::load_costume_icons() {
    if (icons_loaded) return;
    icons_loaded = true;

    fs::path dir = fs::path("Skins") / global_data.config->paths.skin / "Models/costume_icon";
    if (!fs::exists(dir)) return;

    std::vector<std::pair<int, fs::path>> entries;
    for (auto& e : fs::directory_iterator(dir)) {
        if (e.path().extension() == ".png") {
            try { entries.push_back({std::stoi(e.path().stem().string()), e.path()}); }
            catch (...) {}
        }
    }
    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    for (auto& [id, path] : entries) {
        costume_ids.push_back(id);
        costume_icons.push_back(ray::LoadTexture(path.string().c_str()));
    }
}

void CostumeMenu::update(double current_time_ms) {
    blue_arrow_fade->update(current_time_ms);
    blue_arrow_move->update(current_time_ms);
}

std::optional<int> CostumeMenu::get_index() {
    if (!costume_select_mode) return std::nullopt;
    return costume_icon_index;
}

std::string CostumeMenu::get_costume_name() const {
    if (costume_ids.empty()) return "";
    return std::to_string(costume_ids[costume_icon_index]);
}

void CostumeMenu::handle_input() {
    if (costume_select_mode) {
        if (!costume_icons.empty()) {
            int n = (int)costume_icons.size();
            if (is_l_kat_pressed(player_num)) {
                costume_icon_index = (costume_icon_index - 1 + n) % n;
                audio.play_sound("kat", VolumePreset::SOUND);
            }
            if (is_r_kat_pressed(player_num)) {
                costume_icon_index = (costume_icon_index + 1) % n;
                audio.play_sound("kat", VolumePreset::SOUND);
            }
            if (is_l_don_pressed(player_num) || is_r_don_pressed(player_num)) {
                confirmed = true;
                audio.play_sound("don", VolumePreset::SOUND);
            }
        }
    } else {
        if (is_l_kat_pressed(player_num)) {
            selected_index = (selected_index - 1 + NUM_ITEMS) % NUM_ITEMS;
            audio.play_sound("kat", VolumePreset::SOUND);
        }
        if (is_r_kat_pressed(player_num)) {
            selected_index = (selected_index + 1) % NUM_ITEMS;
            audio.play_sound("kat", VolumePreset::SOUND);
        }

        if ((is_l_don_pressed(player_num) || is_r_don_pressed(player_num)) && ITEMS[selected_index] == COSTUME_SELECT::COSTUME) {
            costume_select_mode = true;
            load_costume_icons();
            audio.play_sound("don", VolumePreset::SOUND);
        }
    }
}

void CostumeMenu::draw(float x, float y) {
    if (is_2p) {
        tex.draw_texture(COSTUME_SELECT::BOX_TOP_LEFT_2P,    {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_TOP_RIGHT_2P,   {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_BOTTOM_LEFT_2P, {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_BOTTOM_RIGHT_2P,{.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_TOP_2P,         {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_BOTTOM_2P,      {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_LEFT_2P,        {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_RIGHT_2P,       {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_CENTER_2P,      {.x = x, .y = y});
    } else {
        tex.draw_texture(COSTUME_SELECT::BOX_TOP_LEFT_1P,    {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_TOP_RIGHT_1P,   {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_BOTTOM_LEFT_1P, {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_BOTTOM_RIGHT_1P,{.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_TOP_1P,         {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_BOTTOM_1P,      {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_LEFT_1P,        {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_RIGHT_1P,       {.x = x, .y = y});
        tex.draw_texture(COSTUME_SELECT::BOX_CENTER_1P,      {.x = x, .y = y});
    }

    auto item_box = is_2p ? COSTUME_SELECT::ITEM_BOX_2P : COSTUME_SELECT::ITEM_BOX_1P;
    constexpr float ITEM_W = 80.0f;
    tex.draw_texture(item_box,                             {.x = x + ITEM_W * 0, .y = y});
    tex.draw_texture(item_box,                             {.x = x + ITEM_W * 1, .y = y});
    tex.draw_texture(COSTUME_SELECT::ITEM_BOX_HIGHLIGHTED, {.x = x + ITEM_W * 2, .y = y});
    tex.draw_texture(item_box,                             {.x = x + ITEM_W * 3, .y = y});
    tex.draw_texture(item_box,                             {.x = x + ITEM_W * 4, .y = y});

    auto& ib    = tex.textures[COSTUME_SELECT::ITEM_BOX_1P];
    float base_x = ib->x[0] + x;
    float base_y = ib->y[0] + y;

    if (costume_select_mode && !costume_icons.empty()) {
        int n = (int)costume_icons.size();
        for (int i = 0; i < 5; i++) {
            int idx = ((costume_icon_index - 2 + i) % n + n) % n;
            auto& icon = costume_icons[idx];
            float scale = std::min(ITEM_W / icon.width, ITEM_W / icon.height);
            float dw = icon.width * scale, dh = icon.height * scale;
            float ix = tex.draw_offset_x + base_x + i * ITEM_W + (ITEM_W - dw) / 2.0f;
            float iy = tex.draw_offset_y + base_y + (ITEM_W - dh) / 2.0f;
            float offset = (icon.width - ib->width) / 2.0f;
            ray::DrawTexturePro(icon,
                {0, 0, (float)icon.width, (float)icon.height},
                {ix + offset, iy + offset, dw, dh}, {0, 0}, 0, ray::WHITE);
        }
    } else {
        for (int i = 0; i < 5; i++) {
            int idx = ((selected_index - 2 + i) % NUM_ITEMS + NUM_ITEMS) % NUM_ITEMS;
            auto it = tex.textures.find(ITEMS[idx]);
            if (it == tex.textures.end()) continue;
            auto* item = it->second.get();
            float item_x = base_x + i * ITEM_W + (ITEM_W - item->width)  / 2.0f;
            float item_y = base_y +              (ITEM_W - item->height) / 2.0f;
            tex.draw_texture(ITEMS[idx], {.x = item_x, .y = item_y});
        }
    }

    auto& hl  = tex.textures[COSTUME_SELECT::ITEM_BOX_HIGHLIGHTED];
    auto& arr = tex.textures[COSTUME_SELECT::BLUE_ARROW];
    float hl_left  = hl->x[0] + x + ITEM_W * 2;
    float hl_right = hl_left + hl->width;
    float arr_y    = hl->y[0] + y + (hl->height - arr->height) / 2.0f;
    float move_val = (float)blue_arrow_move->attribute;
    float fade_val = blue_arrow_fade->attribute;
    tex.draw_texture(COSTUME_SELECT::BLUE_ARROW, {.mirror = "horizontal", .x = hl_right + move_val,                   .y = arr_y, .fade = fade_val});
    tex.draw_texture(COSTUME_SELECT::BLUE_ARROW, {.x = hl_left - (float)arr->width - move_val, .y = arr_y, .fade = fade_val});

    auto th_id = is_2p ? COSTUME_SELECT::TEXT_HIGHLIGHT_2P : COSTUME_SELECT::TEXT_HIGHLIGHT_1P;
    auto th_it = tex.textures.find(th_id);
    if (th_it != tex.textures.end()) {
        auto* th = th_it->second.get();
        tex.draw_texture(th_id, {.x = x - th->width / 2.0f, .y = y - th->height / 2.0f});
    }

    auto& box = tex.textures[is_2p ? COSTUME_SELECT::BOX_CENTER_2P : COSTUME_SELECT::BOX_CENTER_1P];
    float cx = box->x[0] + x + box->x2[0] / 2.0f - title_text->width / 2.0f;
    title_text->draw({.x = cx + tex.skin_config[SC::ENTRY_COSTUME_TEXT].x, .y = y + tex.skin_config[SC::ENTRY_COSTUME_TEXT].y});
}
