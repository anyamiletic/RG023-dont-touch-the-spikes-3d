#pragma once

void draw_new_spike(const char *side, float height);
void draw_spike_wall(const char *side, int difficulty_level, bool change);
void init_heights();
bool ball_spike_collision(float pos_x, float pos_y, float spike_height, char *side);
bool spike_collision(float height1, float height2);