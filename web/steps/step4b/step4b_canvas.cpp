#include "step4b_canvas.h"
#include <stdlib.h>
#include <sstream>

CanvasCmd::CanvasCmd(CJsonObject* items) : _items(items) {
}

CanvasCmd::CanvasCmd(const CanvasCmd& src) : _items(src._items) {
}

CanvasCmd& CanvasCmd::beginPath() {
    CJsonObject obj;
    obj.Add("cmd", "beginPath");
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::closePath() {
    CJsonObject obj;
    obj.Add("cmd", "closePath");
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::stroke() {
    CJsonObject obj;
    obj.Add("cmd", "stroke");
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::fill() {
    CJsonObject obj;
    obj.Add("cmd", "fill");
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::attr(const CJsonObject& a) {
    CJsonObject obj(a);
    obj.Add("cmd", "attr");
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::moveTo(float x, float y) {
    CJsonObject obj;
    obj.Add("cmd", "moveTo");
    obj.Add("x", x);
    obj.Add("y", y);
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::lineTo(float x, float y) {
    CJsonObject obj;
    obj.Add("cmd", "lineTo");
    obj.Add("x", x);
    obj.Add("y", y);
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::arc(float x, float y, float radius, float startAngle, float endAngle, bool ccw) {
    CJsonObject obj;
    obj.Add("cmd", "arc");
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("radius", radius);
    obj.Add("startAngle", startAngle);
    obj.Add("endAngle", endAngle);
    obj.Add("ccw", ccw);
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::arcTo(float x1, float y1, float x2, float y2, float radius) {
    CJsonObject obj;
    obj.Add("cmd", "arcTo");
    obj.Add("x1", x1);
    obj.Add("y1", y1);
    obj.Add("x2", x2);
    obj.Add("y2", y2);
    obj.Add("radius", radius);
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::quadraticCurveTo(float cpx, float cpy, float x, float y) {
    CJsonObject obj;
    obj.Add("cmd", "quadraticCurveTo");
    obj.Add("cpx", cpx);
    obj.Add("cpy", cpy);
    obj.Add("x", x);
    obj.Add("y", y);
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
    CJsonObject obj;
    obj.Add("cmd", "bezierCurveTo");
    obj.Add("cp1x", cp1x);
    obj.Add("cp1y", cp1y);
    obj.Add("cp2x", cp2x);
    obj.Add("cp2y", cp2y);
    obj.Add("x", x);
    obj.Add("y", y);
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::ellipse(float x, float y, float radiusX, float radiusY, float rotation,
        float startAngle, float endAngle, bool ccw) {
    CJsonObject obj;
    obj.Add("cmd", "ellipse");
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("radiusX", radiusX);
    obj.Add("radiusY", radiusY);
    obj.Add("rotation", rotation);
    obj.Add("startAngle", startAngle);
    obj.Add("endAngle", endAngle);
    obj.Add("ccw", ccw);
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::rect(float x, float y, float w, float h) {
    CJsonObject obj;
    obj.Add("cmd", "rect");
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("w", w);
    obj.Add("h", h);
    _items->Add(obj);
    return *this;
}

CanvasCmd& CanvasCmd::text(float x, float y, const char *s) {
    CJsonObject obj;
    obj.Add("cmd", "text");
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("text", s);
    _items->Add(obj);
    return *this;
}

void* canvas_commands_to_json(const CJsonObject& items) {
    std::string json(items.ToString());
    void* buf = malloc(1 + json.length());
    memcpy(buf, json.c_str(), 1 + json.length());
    return buf;
}


GiJsonCanvas::GiJsonCanvas(CJsonObject* items) : _items(items) {
}

GiJsonCanvas::~GiJsonCanvas() {
}

static const float patDash[]      = { 4, 2, 0 };
static const float patDot[]       = { 1, 2, 0 };
static const float patDashDot[]   = { 10, 2, 2, 2, 0 };
static const float dashDotDot[]   = { 20, 2, 2, 2, 2, 2, 0 };
const float* const LINE_DASH[] = { NULL, patDash, patDot, patDashDot, dashDotDot };

void GiJsonCanvas::setPen(int argb, float width, int style, float phase, float) {
    CJsonObject attr;
    char color[30];
    int i;

    if (style > 0 && (style & kLineCapMask)) {
        attr.Add("lineCap", (style & kLineCapSquare) ? "square" : (style & kLineCapRound) ? "round" : "butt");
    }
    style = style > 0 ? (style & kLineCapMask) : style;
    if (style == 5) {   // null line
        attr.Add("strokeStyle", "transparent");
    }
    else if (argb != 0) {   // 0x AA RR GG BB, ignored if equals to zero
        sprintf(color, "rgba(%d,%d,%d,%.1f)", (argb >> 16) & 0xFF, argb >> 8, (argb & 0xFF) & 0xFF,
                ((argb >> 24) & 0xFF) / 255.f);
        attr.Add("strokeStyle", color);
    }

    if (width > 0) {    // Line width in point unit, ignored if <= 0
        attr.Add("lineWidth", width);
    }
    if (style >= 0 && style < 5) { // 0:solid, 1:dash, 2:...., 3:-.-.-, 4:-..-..
        std::stringstream pattern;
        for (i = 0; LINE_DASH[style] && LINE_DASH[style][i] > 0.1f; i++) {
            if (i > 0) pattern << ",";
            pattern << LINE_DASH[style][i] * (width < 1.f ? 1.f : width);
        }
        attr.Add("setLINE_DASH", pattern.str());
    }

    attr.Add("cmd", "attr");
    _items->Add(attr);
}

void GiJsonCanvas::setBrush(int argb, int) {
    CJsonObject attr;
    char color[30];
    sprintf(color, "rgba(%d,%d,%d,%.1f)", (argb >> 16) & 0xFF, argb >> 8, (argb & 0xFF) & 0xFF,
            ((argb >> 24) & 0xFF) / 255.f);
    attr.Add("fillStyle", color);
    attr.Add("cmd", "attr");
    _items->Add(attr);
}

void GiJsonCanvas::clearRect(float x, float y, float w, float h) {
    CJsonObject obj;
    obj.Add("cmd", "clearRect");
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("w", w);
    obj.Add("h", h);
    _items->Add(obj);
}

void GiJsonCanvas::drawRect(float x, float y, float w, float h, bool stroke, bool fill) {
    CJsonObject obj;
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("w", w);
    obj.Add("h", h);
    _items->Add(obj);

    if (stroke) {
        CJsonObject obj1(obj);
        obj1.Add("cmd", "strokeRect");
        _items->Add(obj1);
    }
    if (fill) {
        CJsonObject obj2(obj);
        obj2.Add("cmd", "fillRect");
        _items->Add(obj2);
    }
}

void GiJsonCanvas::drawLine(float x1, float y1, float x2, float y2) {
    moveTo(x1, y1);
    lineTo(x2, y2);
}

void GiJsonCanvas::drawEllipse(float x, float y, float w, float h, bool stroke, bool fill) {
    CJsonObject obj;
    obj.Add("cmd", "ellipse");
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("w", w);
    obj.Add("h", h);
    _items->Add(obj);
    drawPath(stroke, fill);
}

void GiJsonCanvas::beginPath() {
    CJsonObject obj;
    obj.Add("cmd", "beginPath");
    _items->Add(obj);
}

void GiJsonCanvas::moveTo(float x, float y) {
    CJsonObject obj;
    obj.Add("cmd", "moveTo");
    obj.Add("x", x);
    obj.Add("y", y);
    _items->Add(obj);
}

void GiJsonCanvas::lineTo(float x, float y) {
    CJsonObject obj;
    obj.Add("cmd", "lineTo");
    obj.Add("x", x);
    obj.Add("y", y);
    _items->Add(obj);
}

void GiJsonCanvas::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    CJsonObject obj;
    obj.Add("cmd", "bezierCurveTo");
    obj.Add("cp1x", c1x);
    obj.Add("cp1y", c1y);
    obj.Add("cp2x", c2x);
    obj.Add("cp2y", c2y);
    obj.Add("x", x);
    obj.Add("y", y);
    _items->Add(obj);
}

void GiJsonCanvas::quadTo(float cpx, float cpy, float x, float y) {
    CJsonObject obj;
    obj.Add("cmd", "quadraticCurveTo");
    obj.Add("cpx", cpx);
    obj.Add("cpy", cpy);
    obj.Add("x", x);
    obj.Add("y", y);
    _items->Add(obj);
}

void GiJsonCanvas::closePath() {
    CJsonObject obj;
    obj.Add("cmd", "closePath");
    _items->Add(obj);
}

void GiJsonCanvas::drawPath(bool stroke, bool fill) {
    if (stroke) {
        CJsonObject obj1;
        obj1.Add("cmd", "stroke");
        _items->Add(obj1);
    }
    if (fill) {
        CJsonObject obj2;
        obj2.Add("cmd", "fill");
        _items->Add(obj2);
    }
}

void GiJsonCanvas::saveClip() {
    CJsonObject obj;
    obj.Add("cmd", "save");
    _items->Add(obj);
}

void GiJsonCanvas::restoreClip() {
    CJsonObject obj;
    obj.Add("cmd", "restore");
    _items->Add(obj);
}

void GiJsonCanvas::clipRect(float x, float y, float w, float h) {
    CJsonObject obj;
    obj.Add("cmd", "rect");
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("w", w);
    obj.Add("h", h);
    _items->Add(obj);
    clipPath();
}

void GiJsonCanvas::clipPath() {
    CJsonObject obj;
    obj.Add("cmd", "clip");
    _items->Add(obj);
}

void GiJsonCanvas::drawHandle(float x, float y, int type, float angle) {
}

void GiJsonCanvas::drawBitmap(const char* name, float xc, float yc, float w, float h, float angle) {
}

void GiJsonCanvas::drawTextAt(const char* text, float x, float y, float h, int align, float angle) {
    CJsonObject obj;
    obj.Add("cmd", "text");
    obj.Add("x", x);
    obj.Add("y", y);
    obj.Add("text", text);
    obj.Add("align", align);
    obj.Add("angle", angle);
    _items->Add(obj);
}
