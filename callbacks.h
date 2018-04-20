#pragma once

void on_display(void);
void on_keyboard(unsigned char key, int x, int y);
void on_reshape(int width, int height);
void on_mouse(int button, int state, int x, int y);
void on_timer(int value);