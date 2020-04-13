#!/bin/sh
# sh web/build/step6.sh

functions=`python web/build/extract_api.py web/steps/step5.cpp web/steps/step6.cpp`

test -d web/js/gen || mkdir web/js/gen
emcc web/steps/step6.cpp \
     web/steps/step5.cpp \
     web/steps/step4_canvas.cpp \
     core/storage/CJsonObject.cpp \
     core/storage/cJSON.c \
     -I core/storage \
     -I web/steps \
     -s EXPORTED_FUNCTIONS="$functions" \
     -o web/js/gen/step6.js \
     --post-js web/steps/step3_canvas.js
