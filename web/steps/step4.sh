#!/bin/sh

test -d web/steps/js || ln -s $(pwd)/web/js $(pwd)/web/steps/
test -d web/steps/css || ln -s $(pwd)/web/css $(pwd)/web/steps/

emcc web/steps/step4*.cpp \
     core/storage/CJsonObject.cpp \
     core/storage/cJSON.c \
     -I core/storage \
     -I web/steps \
     -s EXPORTED_FUNCTIONS="['_render_as_json']" \
     -o web/steps/step4.js \
     --post-js web/steps/step3_canvas.js
