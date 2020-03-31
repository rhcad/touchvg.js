#include <CJsonObject.hpp>

using neb::CJsonObject;

class CanvasCmd {
public:
    CanvasCmd(CJsonObject* items);
    CanvasCmd(const CanvasCmd& src);

    CanvasCmd& beginPath();
    CanvasCmd& closePath();
    CanvasCmd& stroke();
    CanvasCmd& fill();
    CanvasCmd& attr(const CJsonObject& a);

    CanvasCmd& moveTo(float x, float y);
    CanvasCmd& lineTo(float x, float y);
    CanvasCmd& arc(float x, float y, float radius, float startAngle, float endAngle, bool ccw = true);
    CanvasCmd& arcTo(float x1, float y1, float x2, float y2, float radius);
    CanvasCmd& quadraticCurveTo(float cpx, float cpy, float x, float y);
    CanvasCmd& bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);
    CanvasCmd& ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle,
            float endAngle, bool ccw = true);
    CanvasCmd& rect(float x, float y, float w, float h);
    CanvasCmd& text(float x, float y, const char *s);

private:
    CJsonObject _obj;
    CJsonObject* _items;

    CanvasCmd& push();
};
