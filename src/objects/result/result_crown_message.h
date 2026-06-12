#pragma once

#include "../../libs/animation.h"

class ResultCrownMessage {
private:
    int frame_index;
    bool is_2p;

    TextureResizeAnimation* scale_in;
    TextureResizeAnimation* scale_fix;
    FadeAnimation* fade_in_anim;
    FadeAnimation* fade_out_anim;

public:
    ResultCrownMessage(int frame_index, bool is_2p);

    void update(double current_ms);
    void draw();
};
