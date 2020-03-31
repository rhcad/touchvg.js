# WASM更多绘图样例

本例实现在网页画布中使用C++函数绘图，实现更多绘图功能。

- 在 `CanvasCmd` 类实现C++绘图指令的封装，`draw_as_json` 中有使用示例。
- 在 js 层的 `step3_canvas.js` 实现绘图指令的解包，调用画布函数完成绘图。

编译和运行：
```
sh web/steps/step4.sh
emrun --port 8080 web/steps/step4.html
```

## 参考资料

- [Emscripten: Pointers and Pointers](https://kapadia.github.io/emscripten/2013/09/13/emscripten-pointers-and-pointers.html)
- [JavaScript与C交换数据](https://www.cntofu.com/book/150/zh/ch2-c-js/ch2-04-data-exchange.md)
- [CanvasRenderingContext2D](https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D)
