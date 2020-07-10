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
    CanvasCmd& arc(float x, float y, float radius, float startAngle, float endAngle, bool ccw=true);
    CanvasCmd& arcTo(float x1, float y1, float x2, float y2, float radius);
    CanvasCmd& quadraticCurveTo(float cpx, float cpy, float x, float y);
    CanvasCmd& bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y);
    CanvasCmd& ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle,
            float endAngle, bool ccw=true);
    CanvasCmd& rect(float x, float y, float w, float h);
    CanvasCmd& text(float x, float y, const char *s);

private:
    CJsonObject* _items;
};

void* canvas_commands_to_json(const CJsonObject& items);

#include "gicanvas.h"

class GiJsonCanvas : public GiCanvas
{
public:
    GiJsonCanvas(CJsonObject* items);
    virtual ~GiJsonCanvas();

    virtual void setPen(int argb, float width, int style, float phase, float originW);
    virtual void setBrush(int argb, int style);
    virtual void clearRect(float x, float y, float w, float h);
    virtual void drawRect(float x, float y, float w, float h, bool stroke, bool fill);
    virtual void drawLine(float x1, float y1, float x2, float y2);
    virtual void drawEllipse(float x, float y, float w, float h, bool stroke, bool fill);

    virtual void beginPath();
    virtual void moveTo(float x, float y);
    virtual void lineTo(float x, float y);
    virtual void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y);
    virtual void quadTo(float cpx, float cpy, float x, float y);
    virtual void closePath();
    virtual void drawPath(bool stroke, bool fill);

    virtual void saveClip();
    virtual void restoreClip();
    virtual void clipRect(float x, float y, float w, float h);
    virtual void clipPath();

    virtual void drawHandle(float x, float y, int type, float angle);
    virtual void drawBitmap(const char* name, float xc, float yc,
                            float w, float h, float angle);
    virtual void drawTextAt(const char* text, float x, float y, float h, int align, float angle);

private:
    CJsonObject *_items;
};
