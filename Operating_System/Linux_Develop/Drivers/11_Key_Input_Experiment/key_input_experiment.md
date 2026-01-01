# 按键输入
本章只是为了学习一下GPIO的输入功能，实际的按键驱动再Linux下有input子系统用于输入设备
## 程序编写
### 设备树修改
```dts
key{
    compatible = "alientek,key";
    pinctrl-0 = <&key_gpio>;
    pinctrl-names = "alientek,key";
    key-gpio = <&gpio3 RK_PC5 GPIO_ACTIVE_HIGH>;
    status = "okay";
};
```
