#!/bin/sh

test -d web/steps/js || ln -s $(pwd)/web/js $(pwd)/web/steps/
test -d web/steps/css || ln -s $(pwd)/web/css $(pwd)/web/steps/

emcc web/steps/step5.cpp \
     web/steps/step4_canvas.cpp \
     core/storage/CJsonObject.cpp \
     core/storage/cJSON.c \
     -I core/storage \
     -I web/steps \
     -s EXPORTED_FUNCTIONS="['_create_canvas', '_clear_content', '_draw_all', '_draw_dyn', '_is_need_regen', '_mouse_down', '_mouse_move', '_mouse_up', '_set_attr', '_set_command']" \
     -o web/steps/step5.js \
     --post-js web/steps/step3_canvas.js
