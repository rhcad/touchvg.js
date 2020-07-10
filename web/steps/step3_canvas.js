(function(mod) {
    mod.render_with_json = (ctx, c_json) => {
        const json = UTF8ToString(c_json);
        let count = 0;
        _free(c_json);
        if (json) {
            for (const cmd of JSON.parse(json)) {
                try {
                    commands[cmd.cmd](ctx, cmd);
                    count++;
                } catch (e) {
                    console.error(e, cmd);
                }
            }
        }
        return count;
    };
    const commands = {
        beginPath(ctx) {
            ctx.beginPath();
        },
        closePath(ctx) {
            ctx.closePath();
        },
        stroke(ctx) {
            ctx.stroke();
        },
        fill(ctx) {
            ctx.fill();
        },
        restore(ctx) {
            ctx.restore();
        },
        save(ctx) {
            ctx.save();
        },
        clip(ctx) {
            ctx.clip();
        },
        moveTo(ctx, cmd) {
            ctx.moveTo(cmd.x, cmd.y);
        },
        lineTo(ctx, cmd) {
            ctx.lineTo(cmd.x, cmd.y);
        },
        arc(ctx, cmd) {
            ctx.arc(cmd.x, cmd.y, cmd.radius, cmd.startAngle, cmd.endAngle, cmd.ccw);
        },
        arcTo(ctx, cmd) {
            ctx.arcTo(cmd.x1, cmd.y1, cmd.x2, cmd.y2, cmd.radius);
        },
        quadraticCurveTo(ctx, cmd) {
            ctx.quadraticCurveTo(cmd.cpx, cmd.cpy, cmd.x, cmd.y);
        },
        bezierCurveTo(ctx, cmd) {
            ctx.bezierCurveTo(cmd.cp1x, cmd.cp1y, cmd.cp2x, cmd.cp2y, cmd.x, cmd.y);
        },
        ellipse(ctx, cmd) {
            ctx.ellipse(cmd.x, cmd.y, cmd.radiusX, cmd.radiusY, cmd.rotation, cmd.startAngle, cmd.endAngle, cmd.ccw);
        },
        rect(ctx, cmd) {
            ctx.rect(cmd.x, cmd.y, cmd.w, cmd.h);
        },
        clearRect(ctx, cmd) {
            ctx.clearRect(cmd.x, cmd.y, cmd.w, cmd.h);
        },
        fillRect(ctx, cmd) {
            ctx.fillRect(cmd.x, cmd.y, cmd.w, cmd.h);
        },
        strokeRect(ctx, cmd) {
            ctx.strokeRect(cmd.x, cmd.y, cmd.w, cmd.h);
        },
        text(ctx, cmd) {
            ctx.font = '24px Arial';
            ctx.fillText(cmd.text, cmd.x, cmd.y);
        },
        textAlign(ctx, cmd) {
            ctx.textAlign = cmd.textAlign;
        },
        attrNames: ['lineCap', 'lineDashOffset', 'lineJoin', 'lineWidth', 'miterLimit', 'setLineDash',
            'direction', 'font', 'textAlign', 'textBaseline',
            'fillStyle', 'strokeStyle', 'alpha'
        ],
        attr(ctx, cmd) {
            Object.keys(cmd).forEach(name => {
                if (name === 'setLineDash') {
                    if (typeof cmd[name] === 'string') {
                        ctx[name](cmd[name].split(','));
                    }
                    else if (Array.isArray(cmd[name])) {
                        ctx[name](cmd[name]);
                    }
                }
                else if (name === 'alpha') {
                    ctx.globalAlpha = cmd[name];
                }
                else if (this.attrNames.indexOf(name) >= 0) {
                    ctx[name] = cmd[name];
                }
            });
        }
    };
}(window));
