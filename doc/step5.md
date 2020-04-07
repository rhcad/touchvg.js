# 使用鼠标交互式绘图

本例实现在网页画布中使用C++函数绘图，实现鼠标交互和绘图命令。

- 从网页传递画布的鼠标消息到C++
- 实现绘图命令和图形实体类
- 双缓存绘图，区分静态图形和动态拖曳图形
- 绘图属性的改变控制
- 画布缩放

编译和运行：

```
source ../emsdk/emsdk_env.sh
sh web/steps/step5.sh
emrun --no_browser --port 8080 web
```

然后打开 [steps/step5.html](http://0.0.0.0:8080/steps/step5.html)

或者不使用`emrun`，直接在WebStorm或PyCharm中打开 `steps/step5.html` 执行 `Open in Browser`，刷新网页即可查看最新的编译效果。

## 参考资料

- [Optimizing canvas](https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Optimizing_canvas)
- [Improving HTML5 Canvas Performance](https://www.html5rocks.com/en/tutorials/canvas/performance/)
- [HTML5 Canvas鼠标与键盘事件](https://blog.csdn.net/jia20003/article/details/9242423)
