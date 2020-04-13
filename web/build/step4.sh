#!/bin/sh
# sh web/build/step4.sh

test -d web/js/gen || mkdir web/js/gen
emcc web/steps/step4*.cpp \
     core/storage/CJsonObject.cpp \
     core/storage/cJSON.c \
     -I core/storage \
     -I web/steps \
     -s EXPORTED_FUNCTIONS="['_render_as_json']" \
     -o web/js/gen/step4.js \
     --post-js web/steps/step3_canvas.js
