#pragma once

void draw_new_spike(const char *side, float height);
void draw_spike_wall(const char *side, int difficulty_level, bool change);
void init_heights(); //for spikes
bool ball_spike_collision(float pos_x, float pos_y, float spike_height, char *side);
bool spike_collision(float height1, float height2);

void draw_token(float token_radius, float height, float width);
bool ball_token_collision(const char *side, float pos_x, float pos_y, float token_height, float token_width);
void draw_rand_token(float token_radius, float height, float width, bool collision);