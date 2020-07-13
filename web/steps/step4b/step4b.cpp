#include "step4b_canvas.h"
#include "gicanvas.h"
#include "gicontxt.h"

struct Point2d {
    float x, y;
    Point2d() : x(0), y(0) {}
    Point2d(float _x, float _y) : x(_x), y(_y) {}
    void set(float _x, float _y) { x = _x; y = _y; }
};

static void _arcToBezier(
    Point2d points[4], const Point2d& center, float rx, float ry,
    float startAngle, float sweepAngle)
{
    float sy = ry / rx;
    ry = rx;
    float B = ry * sinf(sweepAngle / 2);
    float C = rx * cosf(sweepAngle / 2);
    float A = rx - C;

    float X = A * 4 / 3;
    float Y = B - X * (rx-A)/B;

    points[0].set(C,    -B);
    points[1].set(C+X,  -Y);
    points[2].set(C+X,  Y);
    points[3].set(C,    B);

    // rotate to the original angle
    A = startAngle + sweepAngle / 2;
    float s = sinf(A);
    float c = cosf(A);

    for (int i = 0; i < 4; i++)
    {
        points[i].set(center.x + points[i].x * c - points[i].y * s,
            center.y + points[i].x * s * sy + points[i].y * c * sy);
    }
}

static int _arcToBezierPlusSweep(
    Point2d points[16], const Point2d& center, float rx, float ry, 
    float startAngle, float sweepAngle)
{
    const float M = 0.5522847498307933984022516f;
    float dx = rx * M;
    float dy = ry * M;

    int k, n;
    float endAngle;

    // 计算第一段椭圆弧的终止角度
    if (startAngle < M_PI_2) {             // +Y
        endAngle = M_PI_2;
        k = 1;
    }
    else if (startAngle < M_PI) {          // -X
        endAngle = M_PI;
        k = 2;
    }
    else if (startAngle < 3*M_PI_2) {      // -Y
        endAngle = 3*M_PI_2;
        k = 3;
    }
    else {                                  // +X
        endAngle = M_PI * 2;
        k = 0;
    }
    if (endAngle - startAngle > 1e-5)       // 转换第一段椭圆弧
    {
        _arcToBezier(points, center, rx, ry,
            startAngle, endAngle - startAngle);
        n = 4;
    }
    else
        n = 1;                              // 第一点在下边循环内设置
    sweepAngle -= (endAngle - startAngle);
    startAngle = endAngle;
    while (sweepAngle >= M_PI_2)           // 增加整90度弧
    {
        if (k == 0)                         // 第一象限
        {
            points[n-1].set(center.x + rx, center.y);
            points[n  ].set(center.x + rx, center.y + dy);
            points[n+1].set(center.x + dx, center.y + ry);
            points[n+2].set(center.x,      center.y + ry);
        }
        else if (k == 1)                    // 第二象限
        {
            points[n-1].set(center.x,      center.y + ry);
            points[n  ].set(center.x - dx, center.y + ry);
            points[n+1].set(center.x - rx, center.y + dy);
            points[n+2].set(center.x - rx, center.y);
        }
        else if (k == 2)                    // 第三象限
        {
            points[n-1].set(center.x - rx, center.y);
            points[n  ].set(center.x - rx, center.y - dy);
            points[n+1].set(center.x - dx, center.y - ry);
            points[n+2].set(center.x,      center.y - ry);
        }
        else                                // 第四象限
        {
            points[n-1].set(center.x,      center.y - ry);
            points[n  ].set(center.x + dx, center.y - ry);
            points[n+1].set(center.x + rx, center.y - dy);
            points[n+2].set(center.x + rx, center.y);
        }
        k = (k + 1) % 4;
        n += 3;
        sweepAngle -= M_PI_2;
        startAngle += M_PI_2;
    }
    if (sweepAngle > 1e-5)                  // 增加余下的弧
    {
        _arcToBezier(&points[n-1], center, rx, ry, startAngle, sweepAngle);
        n += 3;
    }

    return n;
}

static int arcToBezier(
    Point2d points[16], const Point2d& center, float rx, float ry,
    float startAngle, float sweepAngle)
{
    int n = 0;

    if (fabsf(sweepAngle) < M_PI_2 + 1e-5)
    {
        _arcToBezier(points, center, rx, ry, startAngle, sweepAngle);
        n = 4;
    }
    else if (sweepAngle > 0)
    {
        n = _arcToBezierPlusSweep(
            points, center, rx, ry, startAngle, sweepAngle);
    }

    return n;
}

static void drawArc(GiCanvas &canvas, const Point2d& center, float rx, float ry,
        float startAngle, float sweepAngle) {
    Point2d pxs[16];
    int count = arcToBezier(pxs, center,
        rx, ry, startAngle, sweepAngle);
    if (count > 3) {
        canvas.moveTo(pxs[0].x, pxs[0].y);
        for (int i = 1; i + 2 < count; i += 3) {
            canvas.bezierTo(pxs[i].x, pxs[i].y, pxs[i+1].x, pxs[i+1].y, pxs[i+2].x, pxs[i+2].y);
        }
        canvas.drawPath(true, false);
    }
}

static void test_render(GiCanvas &canvas) {
    // CanvasCmd(&items).arc(220, 70, 50, 0, 1.5 * M_PI, false).stroke();
    drawArc(canvas, Point2d(220, 70), 50, 50, 0, 1.5 * M_PI);

    // rectAttr2.Add("fillStyle", "rgba(0, 0, 255, .6)");
    // rectAttr2.Add("setLineDash", "30, 4");
    // rectAttr2.Add("lineWidth", 3);
    // CanvasCmd(&items).rect(100, 170, 50, 80).attr(rectAttr).stroke().fill();
    canvas.setPen(0, 3, 1, 0, 0);
    canvas.setBrush(0x800000ff, 0);
    canvas.drawRect(100, 170, 50, 80, true, true);

    // arcAttr.Add("strokeStyle", "red");
    // arcAttr.Add("lineJoin", "round");
    // arcAttr.Add("lineWidth", 5);
    // CanvasCmd(&items).beginPath().moveTo(30, 20).lineTo(60, 50).lineTo(150, 12).arcTo(150, 100, 50, 20, 30).attr(arcAttr).stroke();
    canvas.setPen(0xffff0000, 5, 0, 0, 0);
    canvas.beginPath();
    canvas.moveTo(30, 20);
    canvas.lineTo(60, 50);
    canvas.lineTo(150, 12);
    // canvas.arcTo(150, 100, 50, 20, 30);
    canvas.drawPath(true, false);

    // CanvasCmd(&items).text(100, 145, "Hello from C++!");
    canvas.drawTextAt("Hello from C++!", 100, 145, 0, GiCanvas::kAlignLeft, 0);
}

extern "C" void* render_as_json() {
    CJsonObject items;
    GiJsonCanvas canvas(&items);
    test_render(canvas);
    return canvas_commands_to_json(items);
}
