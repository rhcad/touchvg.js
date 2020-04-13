#include "step5.h"

class SelectCmd : public Command {
private:
    const Shape* _sp;

public:
    SelectCmd(EventDispacher *owner) : Command("select", owner), _sp(NULL) {}

    virtual void destroy() {
        delete this;
    }

    virtual void render(CJsonObject *items) const {
        if (_sp) {
            CJsonObject attr(_sp->_attr);
            if (!attr.Replace("strokeStyle", "#00f")) {
                attr.Add("strokeStyle", "#00f");
            }
            if (!attr.Replace("lineWidth", 5)) {
                attr.Add("lineWidth", 5);
            }
            _sp->render(items, &attr);
        }
    }

    virtual void onDown(const Point &pt) {
        _sp = hitTest(pt);
    }

    virtual void onMove(const Point &pt) {
        _sp = hitTest(pt);
    }

    virtual void onUp(const Point &pt) {
        _sp = NULL;
    }

    const Shape* hitTest(const Point &pt) {
        Point nearPt;
        float tol = 10;
        float d, minDist = 1e5;
        const Shape* found = NULL;

        for (std::list<Shape *>::const_iterator it = owner()->shapes().begin(); it != owner()->shapes().end(); ++it) {
            d = (*it)->hitTest(pt, tol, nearPt);
            if (minDist > d) {
                minDist = d;
                found = *it;
            }
        }
        return minDist < tol ? found : NULL;
    }
};

extern "C" void *create_canvas2() {
    EventDispacher *p = (EventDispacher *)create_canvas();

    p->addCommand(new SelectCmd(p));
    return p;
}
