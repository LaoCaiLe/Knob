# Knob

##  1. 简介

参照GitHub [SmartKnob](github.com/scottbez1/smartknob)项目制作的智能旋钮，3D模型、硬件和UI画面参考SmartKnob，软件代码独立设计。

<img src="https://github.com/LaoCaiLe/Knob/assets/70246846/f3e0c6c2-2f93-40c8-bda4-672a11888ccc" alt="knob" style="zoom:90" />

## 2. 软件

- vscode + platformIO
- lvgl 8.3

## 3. 硬件

- 主控：ESP32-WROOM-32E 8G
- 屏幕：1.28inch圆形TFT 240*240 GC9A01驱动
- 电机：2804-100kv 12N14P
- 电池：锂电池7.4V
- 氛围灯：WS2812B

## 4. 功能

基本功能：

- 定位（挡位）
- 阻尼
- 震动反馈

代码编写功能模式

- 无限制模式
- 自动回中心点模式
- 0/1开关模式
- 2PI 6挡位模式
- 2PI 10挡位模式
- 2PI限制 旋转带阻尼
- PI限制 旋转带高阻尼

## 5. todo

- 电机参数优化
- 蓝牙+hid

## 6. 参考开源网址和资料

- https://github.com/scottbez1/smartknob
- https://gitee.com/wenzhengclub/super_knob
- https://github.com/SmallPond/X-Knob
- https://oshwhub.com/45coll/a2fff3c71f5d4de2b899c64b152d3da5
- http://simplefoc.cn/#/simplefoc_translation/1%E9%A6%96%E9%A1%B5.md
- https://docs.lvgl.io/master/

