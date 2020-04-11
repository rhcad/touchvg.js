# WASM绘图样例

本例实现在网页画布中使用C++函数绘图。

编译和运行：

```
source ../emsdk/emsdk_env.sh
sh web/build/step3.sh
emrun --no_browser --port 8080 web
```

然后打开 [steps/step3.html](http://0.0.0.0:8080/steps/step3.html)

或者不使用`emrun`，直接在WebStorm或PyCharm中打开 `steps/step3.html` 执行 `Open in Browser`。


要点：

- 在C++中使用JSON封装绘图指令，在JS中解包绘图，是接口相对简单的方式，不需要对每个绘图指令做JS/C++的接口转换。
- 从C++返回字符串到JS，先在C++内分配内存并设置串，然后在JS中使用`UTF8ToString`转为JS串，用完后`_free`。
- 在JS中可编写 `Module.onRuntimeInitialized` 回调函数，WASM 模块加载完后就可自动执行。
- 在编译脚本中，可用 `--post-js` 指定要合并的附加js文件。

## 参考资料

- [Emscripten: Pointers and Pointers](https://kapadia.github.io/emscripten/2013/09/13/emscripten-pointers-and-pointers.html)
- [JavaScript与C交换数据](https://www.cntofu.com/book/150/zh/ch2-c-js/ch2-04-data-exchange.md)
