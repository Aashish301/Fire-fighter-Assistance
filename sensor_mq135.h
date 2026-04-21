#pragma once

void mq135Init();
int  mq135Read();
bool mq135IsHigh(int value, int threshold = 400);
