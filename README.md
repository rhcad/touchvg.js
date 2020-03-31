# touchvg-js

交互式矢量图形框架 TouchVG 的 Web 版，基于 WebAssembly 编译 C++ 内核。

## 编译

1. 下载 WebAssembly，使用国内的镜像库编译（含 Binaryen 等编译器后端工具）

```
git clone https://github.com/juj/emsdk.git ../emsdk
cp misc/emsdk_manifest.json ../emsdk
cd ../emsdk
./emsdk install sdk-upstream-master-64bit --shallow
./emsdk activate sdk-upstream-master-64bit
source ./emsdk_env.sh
```

2. 编译 TouchVG
```
source ../emsdk/emsdk_env.sh
sh build.sh
```

3. 在浏览器运行
```
emrun --no_browser --port 8080 web
```

## 更多

- [探索过程](doc/steps.md)
