#include <emscripten.h>
#include <CJsonObject.hpp>

// 演示在C++代码里将绘图指令打包为JSON串，在JS里解包调用画布函数绘图，记得要free
extern "C" void* render_as_json() {
    neb::CJsonObject items;
    neb::CJsonObject arc, text;

    // ctx.arc(100, 75, 50, 0, 2 * PI)
    arc.Add("cmd", "arc");
    arc.Add("x", 100);
    arc.Add("y", 75);
    arc.Add("radius", 50);
    arc.Add("startAngle", 0);
    arc.Add("endAngle", 2 * M_PI);
    arc.Add("x", 100);

    text.Add("cmd", "text");
    text.Add("x", 100);
    text.Add("y", 140);
    text.Add("text", "Hello from C++!");

    items.Add(arc);
    items.Add(text);

    std::string json(items.ToString());
    void* buf = malloc(1 + json.length());
    memcpy(buf, json.c_str(), 1 + json.length());
    return buf;
}
