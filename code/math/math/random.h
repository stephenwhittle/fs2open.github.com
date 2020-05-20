#pragma once

int myrand();
int rand32(); // returns a random number between 0 and 0x7fffffff

const int RAND_MAX_2	= (RAND_MAX/2);
const float RAND_MAX_1f	= (1.0f / RAND_MAX);
