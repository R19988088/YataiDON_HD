#include "result_crown_message.h"
#include "../../libs/texture.h"
#include <algorithm>

ResultCrownMessage::ResultCrownMessage(int frame_index, bool is_2p)
    : frame_index(frame_index), is_2p(is_2p) {
    scale_in      = (TextureResizeAnimation*)tex.get_animation(21);
    scale_fix     = (TextureResizeAnimation*)tex.get_animation(22);
    fade_in_anim  = (FadeAnimation*)tex.get_animation(23);
    fade_out_anim = (FadeAnimation*)tex.get_animation(24);
    scale_in->start();
    scale_fix->start();
    fade_in_anim->start();
    fade_out_anim->start();
}

void ResultCrownMessage::update(double current_ms) {
    scale_in->update(current_ms);
    scale_fix->update(current_ms);
    fade_in_anim->update(current_ms);
    fade_out_anim->update(current_ms);
}

void ResultCrownMessage::draw() {
    float scale = scale_in->is_finished
        ? (float)scale_fix->attribute
        : (float)scale_in->attribute;
    double fade = std::min(fade_in_anim->attribute, fade_out_anim->attribute);
    if (fade <= 0.0) return;
    tex.draw_texture(tex.get_enum("crown/message"),
        {.frame=frame_index, .scale=scale, .center=true, .fade=fade, .index=is_2p});
}
