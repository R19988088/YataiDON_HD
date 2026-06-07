#pragma once

#include "global_data.h"

extern std::atomic<bool> input_thread_running;
extern std::thread input_thread;

extern std::mutex input_mutex;
extern std::vector<int> pressed_keys;
extern std::vector<int> released_keys;

void input_polling_thread();
void poll_keyboard_once();
void poll_touch_once();

// Check if a key was pressed since the last check
// This consumes the key press event
bool check_key_pressed(int key);

// Check if a key was released since the last check
// This consumes the key release event
bool check_key_released(int key);

// Clear all buffered input events
// Useful when changing screens or locking input
void clear_input_buffers();
void shutdown_sdl_joysticks();
void android_set_keyboard_visible(bool visible);

bool is_input_key_pressed(const std::vector<int>& keys, const std::vector<int>& gamepad_buttons);
bool is_l_don_pressed(PlayerNum player_num = PlayerNum::ALL);
bool is_r_don_pressed(PlayerNum player_num = PlayerNum::ALL);
bool is_l_kat_pressed(PlayerNum player_num = PlayerNum::ALL);
bool is_r_kat_pressed(PlayerNum player_num = PlayerNum::ALL);

inline bool operator==(const ray::Color& a, const ray::Color& b)
{
    return a.r == b.r &&
           a.g == b.g &&
           a.b == b.b &&
           a.a == b.a;
}

inline bool operator!=(const ray::Color& a, const ray::Color& b)
{
    return !(a == b);
}
