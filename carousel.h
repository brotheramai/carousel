#pragma once

#include "quantum.h"


typedef struct {
    uint16_t keycode;
    char * options;
} Carousel;
const Carousel PROGMEM carousels[CAROUSEL_MAX];