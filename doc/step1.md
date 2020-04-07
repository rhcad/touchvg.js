# 第一个样例

本例实现在网页调用简单的C++函数。

1. 在C++代码里实现接口函数 `extern "C" int test_func()`，见 `step1.cpp`。

2. 在编译脚本中用 `EXPORTED_FUNCTIONS` 指定要导出的函数，将C++代码编译为js和wasm文件：

```
source ../emsdk/emsdk_env.sh
emcc web/steps/step1.cpp -s EXPORTED_FUNCTIONS="['_test_func']" -o web/steps/step1.js
```
注：
    - 基于 LLVM 的 Emscripten 编译器工具，编译C/C++模块
    - 默认已定义 `BINARYEN` 变量，使用 Binaryen (将WebAssembly作为输出格式定位的编译器的后端工具)。
    - emcc 在 1.37 以上版本支持直接生成 wasm 文件，不必再定义`-s WASM=1`

3. 在网页中引用 `simple.js`，调用接口函数 `_test_func`，见 `step1.html`。 

4. 在浏览器运行

```
emrun --port 8080 web/steps/step1.html
```

## 参考资料

- [asm.js 和 Emscripten 入门教程](http://www.ruanyifeng.com/blog/2017/09/asmjs_emscripten.html)
