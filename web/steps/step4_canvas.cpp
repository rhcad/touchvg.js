#include "step4_canvas.h"

CanvasCmd::CanvasCmd(CJsonObject* items) : _items(items) {
}

CanvasCmd::CanvasCmd(const CanvasCmd& src) : _obj(src._obj), _items(src._items) {
}

CanvasCmd& CanvasCmd::push() {
    if (!_obj.IsEmpty()) {
        _items->Add(CJsonObject(_obj));
    }
    _obj.Clear();
    return *this;
}

CanvasCmd& CanvasCmd::beginPath() {
    push()._obj.Add("cmd", "beginPath");
    return push();
}

CanvasCmd& CanvasCmd::closePath() {
    push()._obj.Add("cmd", "closePath");
    return push();
}

CanvasCmd& CanvasCmd::stroke() {
    push()._obj.Add("cmd", "stroke");
    return push();
}

CanvasCmd& CanvasCmd::fill() {
    push()._obj.Add("cmd", "fill");
    return push();
}

CanvasCmd& CanvasCmd::attr(const CJsonObject& a) {
    push()._obj = a;
    _obj.Add("cmd", "attr");
    return push();
}

CanvasCmd& CanvasCmd::moveTo(float x, float y) {
    push()._obj.Add("cmd", "moveTo");
    _obj.Add("x", x);
    _obj.Add("y", y);
    return push();
}

CanvasCmd& CanvasCmd::lineTo(float x, float y) {
    push()._obj.Add("cmd", "lineTo");
    _obj.Add("x", x);
    _obj.Add("y", y);
    return push();
}

CanvasCmd& CanvasCmd::arc(float x, float y, float radius, float startAngle, float endAngle, bool ccw) {
    push()._obj.Add("cmd", "arc");
    _obj.Add("x", x);
    _obj.Add("y", y);
    _obj.Add("radius", radius);
    _obj.Add("startAngle", startAngle);
    _obj.Add("endAngle", endAngle);
    _obj.Add("ccw", ccw);
    return push();
}

CanvasCmd& CanvasCmd::arcTo(float x1, float y1, float x2, float y2, float radius) {
    push()._obj.Add("cmd", "arcTo");
    _obj.Add("x1", x1);
    _obj.Add("y1", y1);
    _obj.Add("x2", x2);
    _obj.Add("y2", y2);
    _obj.Add("radius", radius);
    return push();
}

CanvasCmd& CanvasCmd::quadraticCurveTo(float cpx, float cpy, float x, float y) {
    push()._obj.Add("cmd", "quadraticCurveTo");
    _obj.Add("cpx", cpx);
    _obj.Add("cpy", cpy);
    _obj.Add("x", x);
    _obj.Add("y", y);
    return push();
}

CanvasCmd& CanvasCmd::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    push()._obj.Add("cmd", "bezierCurveTo");
    _obj.Add("cp1x", cp1x);
    _obj.Add("cp1y", cp1y);
    _obj.Add("cp2x", cp2x);
    _obj.Add("cp2y", cp2y);
    _obj.Add("x", x);
    _obj.Add("y", y);
    return push();
}

CanvasCmd& CanvasCmd::ellipse(float x, float y, float radiusX, float radiusY, float rotation,
        float startAngle, float endAngle, bool ccw) {
    push()._obj.Add("cmd", "ellipse");
    _obj.Add("x", x);
    _obj.Add("y", y);
    _obj.Add("radiusX", radiusX);
    _obj.Add("radiusY", radiusY);
    _obj.Add("rotation", rotation);
    _obj.Add("startAngle", startAngle);
    _obj.Add("endAngle", endAngle);
    _obj.Add("ccw", ccw);
    return push();
}

CanvasCmd& CanvasCmd::rect(float x, float y, float w, float h) {
    push()._obj.Add("cmd", "rect");
    _obj.Add("x", x);
    _obj.Add("y", y);
    _obj.Add("w", w);
    _obj.Add("h", h);
    return push();
}

CanvasCmd& CanvasCmd::text(float x, float y, const char *s) {
    push()._obj.Add("cmd", "text");
    _obj.Add("x", x);
    _obj.Add("y", y);
    _obj.Add("text", s);
    return push();
}

void* to_json(CJsonObject& items) {
    std::string json(items.ToString());
    void* buf = malloc(1 + json.length());
    memcpy(buf, json.c_str(), 1 + json.length());
    return buf;
}
