#!/bin/sh

emcc web/steps/step4*.cpp \
     core/storage/CJsonObject.cpp \
     core/storage/cJSON.c \
     -I core/storage \
     -I web/steps \
     -s EXPORTED_FUNCTIONS="['_draw_as_json']" \
     -o web/steps/step4.js \
     --post-js web/steps/step3_canvas.js
