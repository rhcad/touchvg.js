# 第二个样例

本例实现在网页中js与C++函数互操作。

Emscripten 提供以下宏用以调用嵌入的 js 代码:

- EM_ASM：调用 JS 代码，没有参数，也没有返回值。
- EM_ASM_ARGS：调用 JS 代码，可以有任意个参数，但是没有返回值。
- EM_ASM_INT：调用 JS 代码，可以有任意个参数，返回一个整数。
- EM_ASM_DOUBLE：调用 JS 代码，可以有任意个参数，返回一个双精度浮点数。
- EM_ASM_INT_V：调用 JS 代码，没有参数，返回一个整数。
- EM_ASM_DOUBLE_V：调用 JS 代码，没有参数，返回一个双精度浮点数。

其中，JS 代码里的$0表示第一个参数，$1表示第二个参数。用 UTF8ToString() 方法将C字符数组转成 JS 的字符串。

1. 在编译脚本中用 `EXPORTED_FUNCTIONS` 指定要导出的函数，将C++代码编译为js和wasm文件：
```
emcc web/steps/step2.cpp -s EXPORTED_FUNCTIONS="['_main', '_call_js']" -o web/steps/step2.js
```

2. 在浏览器运行
```
emrun --port 8080 web/steps/step2.html
emrun --no_browser --port 8080 web/steps
```

## 参考资料

- [asm.js 和 Emscripten 入门教程](http://www.ruanyifeng.com/blog/2017/09/asmjs_emscripten.html)
- [emscripten.h](https://emscripten.org/docs/api_reference/emscripten.h.html)
