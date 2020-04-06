#include "step4_canvas.h"
#include <list>
#include <string>
#include <math.h>
#include <string.h>

struct Point {
    float x;
    float y;

    Point() : x(0), y(0) {}
    Point(float x_, float y_) : x(x_), y(y_) {}
    Point(const Point& src) : x(src.x), y(src.y) {}

    bool operator==(const Point& src) const {
        return x == src.x && y == src.y;
    }
    bool operator!=(const Point& src) const {
        return !operator==(src);
    }
};

struct Shape {
    Shape() {}
    Shape(const Shape& src) : _attr(src._attr) {}
    virtual ~Shape() {}

    virtual void destroy() = 0;
    virtual Shape* clone() const = 0;
    virtual void draw(CJsonObject* items) const = 0;
    virtual bool valid() const = 0;
    virtual const char* supportedAttributes() const = 0;

    CJsonObject _attr;

    bool isFilled() const {
        std::string fill;
        _attr.Get("fillStyle", fill);
        return !fill.empty() && fill != "transparent";
    }
};

struct Line : public Shape {
    Point start;
    Point end;

    Line() {}
    Line(const Line& src) : Shape(src), start(src.start), end(src.end) {}

    virtual void destroy() {
        delete this;
    }

    virtual Shape* clone() const {
        return new Line(*this);
    }

    virtual void draw(CJsonObject* items) const {
        if (start != end) {
            CanvasCmd(items).beginPath().moveTo(start.x, start.y).lineTo(end.x, end.y).attr(_attr).stroke();
        }
    }

    virtual bool valid() const {
        return end != start;
    }

    virtual const char* supportedAttributes() const {
        return "lineCap,lineDashOffset,lineJoin,lineWidth,miterLimit,setLineDash,strokeStyle";
    }
};

struct Circle : public Shape {
    Point center;
    float radius;

    Circle() : radius(0) {}
    Circle(const Circle& src) : Shape(src), center(src.center), radius(src.radius) {}

    void setEdgePoint(const Point& pt) {
      radius = hypot(pt.x - center.x, pt.y - center.y);
    }

    virtual void destroy() {
        delete this;
    }

    virtual Shape* clone() const {
        return new Circle(*this);
    }

    virtual void draw(CJsonObject* items) const {
        if (radius > 0) {
            CanvasCmd c(items);
            c.beginPath().arc(center.x, center.y, radius, 0, M_PI * 2, false).attr(_attr).stroke();
            if (isFilled()) {
                c.fill();
            }
        }
    }

    virtual bool valid() const {
        return radius > 0;
    }

    virtual const char* supportedAttributes() const {
        return "lineCap,lineDashOffset,lineJoin,lineWidth,miterLimit,setLineDash,strokeStyle,fillStyle";
    }
};

class EventDispacher;

class Command {
public:
    Command(const char* name, EventDispacher* owner) : _name(name), _owner(owner) {}
    virtual ~Command() {}
    virtual void destroy() = 0;

    const std::string& getName() const { return _name; }
    EventDispacher* owner() const { return _owner; }

    virtual void draw(CJsonObject* items) const = 0;
    virtual void onDown(const Point& pt) = 0;
    virtual void onMove(const Point& pt) = 0;
    virtual void onUp(const Point& pt) = 0;

private:
    std::string _name;
    EventDispacher* _owner;
};

class EventDispacher {
public:
    EventDispacher() : _down(false), _needRegen(false), _cmd(NULL) {
    }

    ~EventDispacher() {
        clear();
        for (std::list<Command*>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
            (*it)->destroy();
        }
    }

    bool needRegen() const { return _needRegen; }
    const std::list<Shape*>& shapes() const { return _shapes; }
    const std::list<Command*>& commands() const { return _commands; }

    void clear() {
        for (std::list<Shape*>::iterator it = _shapes.begin(); it != _shapes.end(); ++it) {
            (*it)->destroy();
        }
        _shapes.clear();
        _needRegen = true;
    }

    void addShape(const Shape* shape) {
        Shape* sp = shape->clone();
        _shapes.push_back(sp);
        _needRegen = true;

        std::string names(sp->supportedAttributes());
        std::string name, value;
        size_t lastPos = names.find_first_not_of(",", 0);
        size_t pos = names.find_first_of(",", lastPos);

        while (names.npos != pos || names.npos != lastPos) {
            name = names.substr(lastPos, pos - lastPos);
            lastPos = names.find_first_not_of(",", pos);
            pos = names.find_first_of(",", lastPos);
            if (attr.Get(name, value)) {
                if (!sp->_attr.Replace(name, value)) {
                    sp->_attr.Add(name, value);
                }
            }
        }
    }

    void addCommand(Command* cmd) {
        _commands.push_back(cmd);
    }

    Command* findCommand(const std::string& name) const {
        for (std::list<Command*>::const_iterator it = _commands.begin(); it != _commands.end(); ++it) {
            if ((*it)->getName() == name) {
                return *it;
            }
        }
        return NULL;
    }

    void setCommand(const std::string& name) {
        Command* cmd = findCommand(name);
        if (cmd != NULL) {
            _cmd = cmd;
        }
    }

    void regen(CJsonObject* items) {
      for (std::list<Shape*>::const_iterator it = _shapes.begin(); it != _shapes.end(); ++it) {
          (*it)->draw(items);
      }
      _needRegen = false;
    }

    void onDown(const Point& pt) {
        _down = true;
        _downPt = _lastPt = pt;

        if (_cmd != NULL) {
            _cmd->onDown(pt);
        }
    }

    bool onMove(const Point& pt) {
        if (_down && _lastPt != pt) {
            if (_cmd != NULL) {
                _cmd->onMove(pt);
            }
            _lastPt = pt;
            return true;
        }
        return false;
    }

    void onUp(const Point& pt) {
        if (_down) {
            _down = false;
            if (_cmd != NULL) {
                _cmd->onUp(pt);
            }
        }
    }

public:
    CJsonObject attr;
private:
    bool _down;
    bool _needRegen;
    Point _downPt;
    Point _lastPt;
    std::list<Shape*> _shapes;
    std::list<Command*> _commands;
    Command* _cmd;
};

extern "C" void clear_content(EventDispacher* p) {
    p->clear();
}

extern "C" void set_command(EventDispacher* p, const char* name) {
    std::string name_(name);
    if (name_ == "clear") {
        p->clear();
    }
    else {
        p->setCommand(name_);
    }
}

extern "C" void* draw_all(EventDispacher* p) {
    CJsonObject items;
    p->regen(&items);
    return to_json(items);
}

extern "C" void* draw_dyn(EventDispacher* p) {
    CJsonObject items;
    for (std::list<Command*>::const_iterator it = p->commands().begin(); it != p->commands().end(); ++it) {
        (*it)->draw(&items);
    }
    return to_json(items);
}

extern "C" bool is_need_regen(EventDispacher* p) {
    return p->needRegen();
}

extern "C" void mouse_down(EventDispacher* p, float x, float y) {
    p->onDown(Point(x, y));
}

extern "C" bool mouse_move(EventDispacher* p, float x, float y) {
    return p->onMove(Point(x, y));
}

extern "C" void mouse_up(EventDispacher* p, float x, float y) {
    p->onUp(Point(x, y));
}

extern "C" void set_attr(EventDispacher* p, const char* name, const char* value) {
    if (!p->attr.Replace(name, value)) {
        p->attr.Add(name, value);
    }
}

class LineCmd : public Command {
private:
    Line _shape;

public:
    LineCmd(EventDispacher* owner) : Command("line", owner) {}

    virtual void destroy() {
        delete this;
    }

    virtual void draw(CJsonObject* items) const {
        _shape.draw(items);
    }

    virtual void onDown(const Point& pt) {
        _shape.start = _shape.end = pt;
    }

    virtual void onMove(const Point& pt) {
        _shape.end = pt;
    }

    virtual void onUp(const Point& pt) {
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
    CircleCmd(EventDispacher* owner) : Command("circle", owner) {}

    virtual void destroy() {
        delete this;
    }

    virtual void draw(CJsonObject* items) const {
        _shape.draw(items);
    }

    virtual void onDown(const Point& pt) {
        _shape.center = pt;
    }

    virtual void onMove(const Point& pt) {
        _shape.setEdgePoint(pt);
    }

    virtual void onUp(const Point& pt) {
        _shape.setEdgePoint(pt);
        if (_shape.valid()) {
            owner()->addShape(&_shape);
            _shape.radius = 0;
        }
    }
};

extern "C" void* create_canvas() {
    EventDispacher *p = new EventDispacher();

    p->addCommand(new LineCmd(p));
    p->addCommand(new CircleCmd(p));

    return p;
}
