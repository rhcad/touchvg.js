#include <emscripten.h>

EM_JS(void, call_js2, (const char* msg), {
    alert(UTF8ToString(msg));
});

extern "C" void call_js(void) {
    EM_ASM_ARGS({
        var msg = UTF8ToString($0);
        alert(msg);
    }, "Hello from C++ via UTF8ToString!");
    call_js2("Hello from C++!");
}

int main() {
    EM_ASM({ alert('Hello World in main()!'); });
}
