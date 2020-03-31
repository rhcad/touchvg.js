#include "step4_canvas.h"

extern "C" void* draw_as_json() {
    CJsonObject items;
    CJsonObject arcAttr, rectAttr, rectAttr2, textAttr;

    arcAttr.Add("strokeStyle", "red");
    arcAttr.Add("lineJoin", "round");
    arcAttr.Add("lineWidth", 5);
    rectAttr.Add("fillStyle", "rgba(0, 255, 0, .8)");
    rectAttr2.Add("fillStyle", "rgba(0, 0, 255, .6)");
    rectAttr2.Add("setLineDash", "30, 4");
    rectAttr2.Add("lineWidth", 3);
    textAttr.Add("fillStyle", "#777");

    CanvasCmd(&items).arc(220, 70, 50, 0, 1.5 * M_PI, false).stroke();
    CanvasCmd(&items).rect(100, 170, 50, 80).attr(rectAttr).stroke().fill();
    CanvasCmd(&items).beginPath().rect(130, 160, 50, 80).attr(rectAttr2).stroke().fill();
    CanvasCmd(&items).beginPath().moveTo(30, 20).lineTo(60, 50).lineTo(150, 12).arcTo(150, 100, 50, 20, 30).attr(arcAttr).stroke();
    CanvasCmd(&items).text(100, 145, "Hello from C++!");
    CanvasCmd(&items).attr(textAttr).text(200, 180, "Hello from C++!");

    std::string json(items.ToString());
    void* buf = malloc(1 + json.length());
    memcpy(buf, json.c_str(), 1 + json.length());
    return buf;
}
