#!/bin/sh

test -d web/js/gen || mkdir web/js/gen
test -f web/js/gen/step3.js || sh web/build/step3.sh
test -f web/js/gen/step4.js || sh web/build/step4.sh
test -f web/js/gen/step5.js || sh web/build/step5.sh
test -f web/js/gen/step6.js || sh web/build/step6.sh
