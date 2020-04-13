#ifndef _STEP5_H
#define _STEP5_H

#include "step4_canvas.h"
#include <math.h>
#include <string>
#include <list>

struct Point {
    float x;
    float y;

    Point() : x(0), y(0) {}

    Point(float x_, float y_) : x(x_), y(y_) {}

    Point(const Point &src) : x(src.x), y(src.y) {}

    bool operator==(const Point &src) const {
        return x == src.x && y == src.y;
    }

    bool operator!=(const Point &src) const {
        return !operator==(src);
    }

    float distanceTo(const Point& pt) const {
        return hypot(x - pt.x, y - pt.y);
    }
};

struct Shape {
    Shape() {}

    Shape(const Shape &src) : _attr(src._attr) {}
    virtual ~Shape() {}
    virtual void destroy() = 0;

    virtual Shape *clone() const = 0;
    virtual void render(CJsonObject *items, const CJsonObject* attr=NULL) const = 0;

    virtual bool valid() const = 0;
    virtual const char *supportedAttributes() const = 0;
    virtual float hitTest(const Point& pt, float tol, Point& nearPt) const = 0;

    CJsonObject _attr;

    void setAttr(const std::string& name, const std::string& value);
    void setAttr(const std::string& name, int value);
    bool isFilled() const;
};

class EventDispacher;

class Command {
public:
    Command(const char *name, EventDispacher *owner) : _name(name), _owner(owner) {}

    virtual ~Command() {}

    virtual void destroy() = 0;

    const std::string &getName() const { return _name; }

    EventDispacher *owner() const { return _owner; }

    virtual void render(CJsonObject *items) const = 0;

    virtual void onDown(const Point &pt) = 0;
    virtual void onMove(const Point &pt) = 0;
    virtual void onUp(const Point &pt) = 0;

private:
    std::string _name;
    EventDispacher *_owner;
};

class EventDispacher {
public:
    EventDispacher();
    ~EventDispacher();

    bool needRegen() const { return _needRegen; }
    const std::list<Shape *> &shapes() const { return _shapes; }
    const std::list<Command *> &commands() const { return _commands; }

    void clear();
    void addShape(const Shape *shape);
    void applyAttr(Shape *shape);

    void addCommand(Command *cmd);
    Command *findCommand(const std::string &name) const;
    bool setCommand(const std::string &name);

    void regen(CJsonObject *items, int page = 0, int size = 1000);

    void onDown(const Point &pt);
    bool onMove(const Point &pt);
    void onUp(const Point &pt);

public:
    CJsonObject attr;
private:
    bool _down;
    bool _needRegen;
    Point _downPt;
    Point _lastPt;
    std::list<Shape *> _shapes;
    std::list<Command *> _commands;
    Command *_cmd;
};

extern "C" void *create_canvas();

#endif // _STEP5_H
