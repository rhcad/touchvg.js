#include "step5.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void Shape::setAttr(const std::string& name, const std::string& value) {
    if (!_attr.Replace(name, value)) {
        _attr.Add(name, value);
    }
}

void Shape::setAttr(const std::string& name, int value) {
    if (!_attr.Replace(name, value)) {
        _attr.Add(name, value);
    }
}

bool Shape::isFilled() const {
    std::string fill;
    _attr.Get("fillStyle", fill);
    return !fill.empty() && fill != "transparent";
}

struct Line : public Shape {
    Point start;
    Point end;

    Line() {}

    Line(const Line &src) : Shape(src), start(src.start), end(src.end) {}

    Line(float x1, float y1, float x2, float y2) : start(x1, y1), end(x2, y2) {}

    virtual void destroy() {
        delete this;
    }

    virtual Shape *clone() const {
        return new Line(*this);
    }

    virtual void render(CJsonObject *items, const CJsonObject* attr=NULL) const {
        if (start != end) {
            CanvasCmd(items).beginPath().moveTo(start.x, start.y).lineTo(end.x, end.y)
                .attr(attr ? attr : _attr).stroke();
        }
    }

    virtual bool valid() const {
        return end != start;
    }

    virtual const char *supportedAttributes() const {
        return "lineCap,lineDashOffset,lineJoin,lineWidth,miterLimit,setLineDash,strokeStyle,alpha";
    }

    virtual float hitTest(const Point& pt, float tol, Point& nearPt) const {
        float t1 = (end.y - start.y) / (end.x - start.x);
        float t2 = -1.f / t1;
        nearPt.x = (pt.y - start.y + start.x * t1 - pt.x * t2) / (t1 - t2);
        nearPt.y = start.y + (nearPt.x - start.x) * t1;
        float dist = pt.distanceTo(nearPt);

        bool between = (start.y <= pt.y && pt.y <= end.y) || (start.y >= pt.y && pt.y >= end.y);
        if (!between) {
            nearPt = fabsf(pt.y - start.y) < fabsf(pt.y - end.y) ? start : end;
            dist = pt.distanceTo(nearPt);
        }

        return dist;
    }
};

struct Circle : public Shape {
    Point center;
    float radius;

    Circle() : radius(0) {}

    Circle(const Circle &src) : Shape(src), center(src.center), radius(src.radius) {}

    Circle(float x, float y, float r) : center(x, y), radius(r) {}

    void setEdgePoint(const Point &pt) {
        radius = pt.distanceTo(center);
    }

    virtual void destroy() {
        delete this;
    }

    virtual Shape *clone() const {
        return new Circle(*this);
    }

    virtual void render(CJsonObject *items, const CJsonObject* attr=NULL) const {
        if (radius > 0) {
            CanvasCmd c(items);
            c.beginPath().arc(center.x, center.y, radius, 0, M_PI * 2, false)
                .attr(attr ? attr : _attr).stroke();
            if (isFilled()) {
                c.fill();
            }
        }
    }

    virtual bool valid() const {
        return radius > 0;
    }

    virtual const char *supportedAttributes() const {
        return "lineCap,lineDashOffset,lineJoin,lineWidth,miterLimit,setLineDash,strokeStyle,fillStyle,alpha";
    }

    virtual float hitTest(const Point& pt, float tol, Point& nearPt) const {
        float dist = pt.distanceTo(center);
        return fabsf(dist - radius);
    }
};

// EventDispacher

EventDispacher::EventDispacher() : _down(false), _needRegen(false), _cmd(NULL) {
}

EventDispacher::~EventDispacher() {
    clear();
    for (std::list<Command *>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
        (*it)->destroy();
    }
}

void EventDispacher::clear() {
    for (std::list<Shape *>::iterator it = _shapes.begin(); it != _shapes.end(); ++it) {
        (*it)->destroy();
    }
    _shapes.clear();
    _needRegen = true;
}

void EventDispacher::addShape(const Shape *shape) {
    Shape *sp = shape->clone();
    _shapes.push_back(sp);
    _needRegen = true;
}

void EventDispacher::applyAttr(Shape *shape) {
    std::string names(shape->supportedAttributes());
    std::string name, value;
    size_t lastPos = names.find_first_not_of(",", 0);
    size_t pos = names.find_first_of(",", lastPos);

    while (names.npos != pos || names.npos != lastPos) {
        name = names.substr(lastPos, pos - lastPos);
        lastPos = names.find_first_not_of(",", pos);
        pos = names.find_first_of(",", lastPos);
        if (attr.Get(name, value)) {
            shape->setAttr(name, value);
        }
    }
}

void EventDispacher::addCommand(Command *cmd) {
    _commands.push_back(cmd);
}

Command *EventDispacher::findCommand(const std::string &name) const {
    for (std::list<Command *>::const_iterator it = _commands.begin(); it != _commands.end(); ++it) {
        if ((*it)->getName() == name) {
            return *it;
        }
    }
    return NULL;
}

bool EventDispacher::setCommand(const std::string &name) {
    Command *cmd = findCommand(name);
    if (cmd != NULL) {
        _cmd = cmd;
    }
    return cmd != NULL;
}

void EventDispacher::regen(CJsonObject *items, int page, int size) {
    int i = 0;
    for (std::list<Shape *>::const_iterator it = _shapes.begin(); it != _shapes.end(); ++it) {
        if (++i > page * size && i <= page * size + size) {
            (*it)->render(items);
        }
    }
    _needRegen = false;
}

void EventDispacher::onDown(const Point &pt) {
    _down = true;
    _downPt = _lastPt = pt;

    if (_cmd != NULL) {
        _cmd->onDown(pt);
    }
}

bool EventDispacher::onMove(const Point &pt) {
    if (_down && _lastPt != pt) {
        if (_cmd != NULL) {
            _cmd->onMove(pt);
        }
        _lastPt = pt;
        return true;
    }
    return false;
}

void EventDispacher::onUp(const Point &pt) {
    if (_down) {
        _down = false;
        if (_cmd != NULL) {
            _cmd->onUp(pt);
        }
    }
}


extern "C" void clear_content(EventDispacher *p) {
    p->clear();
}

extern "C" void *add_rand_shapes(EventDispacher* p, int count) {
    for (int i = 0; i < count; i++) {
        float x = rand() * 2000.0 / RAND_MAX, y = rand() * 1000.0 / RAND_MAX, r = rand() * 500.0 / RAND_MAX;
        Circle c(x, y, r);
        char color[60];
        sprintf(color, "rgb(%d,%d,%d)", rand() % 255, rand() % 255, rand() % 255);
        c.setAttr("strokeStyle", color);
        c.setAttr("lineWidth", rand() % 10 + 1);
        p->addShape(&c);
    }

    return p;
}

extern "C" bool set_command(EventDispacher *p, const char *name) {
    std::string name_(name);
    if (name_ == "clear") {
        p->clear();
    }
    else if (name_ == "rand") {
        add_rand_shapes(p, 100);
    }
    else {
        return p->setCommand(name_);
    }
    return false;
}

extern "C" void *render_all(EventDispacher *p, int page, int size) {
    CJsonObject items;
    p->regen(&items, page, size);
    return to_json(items);
}

extern "C" void *render_dyn(EventDispacher *p) {
    CJsonObject items;
    for (std::list<Command *>::const_iterator it = p->commands().begin(); it != p->commands().end(); ++it) {
        (*it)->render(&items);
    }
    return to_json(items);
}

extern "C" bool is_need_regen(EventDispacher *p) {
    return p->needRegen();
}

extern "C" void mouse_down(EventDispacher *p, float x, float y) {
    p->onDown(Point(x, y));
}

extern "C" bool mouse_move(EventDispacher *p, float x, float y) {
    return p->onMove(Point(x, y));
}

extern "C" void mouse_up(EventDispacher *p, float x, float y) {
    p->onUp(Point(x, y));
}

extern "C" void set_attr(EventDispacher *p, const char *name, const char *value) {
    if (!p->attr.Replace(name, value)) {
        p->attr.Add(name, value);
    }
}

class LineCmd : public Command {
private:
    Line _shape;

public:
    LineCmd(EventDispacher *owner) : Command("line", owner) {}

    virtual void destroy() {
        delete this;
    }

    virtual void render(CJsonObject *items) const {
        _shape.render(items, &owner()->attr);
    }

    virtual void onDown(const Point &pt) {
        _shape.start = _shape.end = pt;
        owner()->applyAttr(&_shape);
    }

    virtual void onMove(const Point &pt) {
        _shape.end = pt;
    }

    virtual void onUp(const Point &pt) {
        _shape.end = pt;
        if (_shape.valid()) {
            owner()->addShape(&_shape);
            _shape.start = _shape.end;
        }
    }
};

class CircleCmd : public Command {
private:
    Circle _shape;

public:
    CircleCmd(EventDispacher *owner) : Command("circle", owner) {}

    virtual void destroy() {
        delete this;
    }

    virtual void render(CJsonObject *items) const {
        _shape.render(items, &owner()->attr);
    }

    virtual void onDown(const Point &pt) {
        _shape.center = pt;
        owner()->applyAttr(&_shape);
    }

    virtual void onMove(const Point &pt) {
        _shape.setEdgePoint(pt);
    }

    virtual void onUp(const Point &pt) {
        _shape.setEdgePoint(pt);
        if (_shape.valid()) {
            owner()->addShape(&_shape);
            _shape.radius = 0;
        }
    }
};

extern "C" void *create_canvas() {
    EventDispacher *p = new EventDispacher();

    p->addCommand(new LineCmd(p));
    p->addCommand(new CircleCmd(p));

    srand((unsigned) time(NULL));
    return p;
}
