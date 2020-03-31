(function(mod) {
    mod.draw_with_json = (ctx, c_json) => {
        const json = UTF8ToString(c_json);
        _free(c_json);
        for (const cmd of JSON.parse(json)) {
            try {
                commands[cmd.cmd](cmd, ctx);
            } catch (e) {
                console.error(e, cmd);
            }
        }
    };
    const commands = {
        beginPath(cmd, ctx) {
            ctx.beginPath();
        },
        closePath(cmd, ctx) {
            ctx.closePath();
        },
        stroke(cmd, ctx) {
            ctx.stroke();
        },
        fill(cmd, ctx) {
            ctx.fill();
        },
        moveTo(cmd, ctx) {
            ctx.moveTo(cmd.x, cmd.y);
        },
        lineTo(cmd, ctx) {
            ctx.lineTo(cmd.x, cmd.y);
        },
        arc(cmd, ctx) {
            ctx.arc(cmd.x, cmd.y, cmd.radius, cmd.startAngle, cmd.endAngle, cmd.ccw);
        },
        arcTo(cmd, ctx) {
            ctx.arcTo(cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.radius);
        },
        quadraticCurveTo(cmd, ctx) {
            ctx.quadraticCurveTo(cmd.cpx, cmd.cpy, cmd.x, cmd.y);
        },
        bezierCurveTo(cmd, ctx) {
            ctx.bezierCurveTo(cmd.cp1x, cmd.cp1y, cmd.cp2x, cmd.cp2y, cmd.x, cmd.y);
        },
        ellipse(cmd, ctx) {
            ctx.ellipse(cmd.x, cmd.y, cmd.radiusX, cmd.radiusY, cmd.rotation, cmd.startAngle, cmd.endAngle, cmd.ccw);
        },
        rect(cmd, ctx) {
            ctx.rect(cmd.x, cmd.y, cmd.w, cmd.h);
        },
        text(cmd, ctx) {
            ctx.font = '24px Arial';
            ctx.fillText(cmd.text, cmd.x, cmd.y);
        },
        attrNames: ['lineCap', 'lineDashOffset', 'lineJoin', 'lineWidth', 'miterLimit', 'setLineDash',
            'direction', 'font', 'textAlign', 'textBaseline',
            'fillStyle', 'strokeStyle',
        ],
        attr(cmd, ctx) {
            Object.keys(cmd).forEach(name => {
                if (name === 'setLineDash') {
                    if (typeof cmd[name] === 'string') {
                        ctx[name](cmd[name].split(','));
                    }
                    else if (Array.isArray(cmd[name])) {
                        ctx[name](cmd[name]);
                    }
                }
                else if (this.attrNames.indexOf(name) >= 0) {
                    ctx[name] = cmd[name];
                }
            });
        }
    };
}(window));
