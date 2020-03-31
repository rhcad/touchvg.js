#!/bin/sh

emcc web/steps/step3.cpp \
     core/storage/CJsonObject.cpp \
     core/storage/cJSON.c \
     -I core/storage \
     -s EXPORTED_FUNCTIONS="['_draw_as_json']" \
     -o web/steps/step3.js \
     --post-js web/steps/step3_canvas.js
