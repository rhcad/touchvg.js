#!/bin/sh
# sh web/build/step5.sh

emcc web/steps/step5.cpp \
     web/steps/step4_canvas.cpp \
     core/storage/CJsonObject.cpp \
     core/storage/cJSON.c \
     -I core/storage \
     -I web/steps \
     -s EXPORTED_FUNCTIONS="['_create_canvas', '_clear_content', '_render_all', '_render_dyn', '_is_need_regen', '_mouse_down', '_mouse_move', '_mouse_up', '_set_attr', '_set_command', '_add_rand_shapes']" \
     -o web/js/gen/step5.js \
     --post-js web/steps/step3_canvas.js
