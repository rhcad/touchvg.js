#!/bin/sh

test -d web/js/gen || mkdir web/js/gen
emcc web/steps/step3.cpp \
     core/storage/CJsonObject.cpp \
     core/storage/cJSON.c \
     -I core/storage \
     -s EXPORTED_FUNCTIONS="['_render_as_json']" \
     -o web/js/gen/step3.js \
     --post-js web/steps/step3_canvas.js
