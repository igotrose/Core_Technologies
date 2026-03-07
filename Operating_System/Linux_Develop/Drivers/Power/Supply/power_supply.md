# 电源供电子系统解析（power_supply + sysfs + rk817 流程）

## 1) power_supply 子系统概览
- 目标：统一电池、电源适配器、USB 供电、充电器等设备的状态与参数。
- 关键对象：每个供电设备对应一个 `power_supply` 设备实例，驱动通过 `power_supply_desc` 提供属性、回调和名字。
- 核心接口：
  - `power_supply_register/devm_power_supply_register`：注册设备
  - `power_supply_changed`：通知状态变化，触发 sysfs/uevent 更新
  - `get_property`：读取属性值（电压、电流、容量、状态等）

## 2) sysfs 是如何提供的
power_supply 框架会自动创建 sysfs 节点：
- 目录：`/sys/class/power_supply/<name>/`
- 节点来源：驱动在 `power_supply_desc.properties` 中声明的属性集合
- 属性读写：内核会将属性映射为 sysfs 文件，读时调用 `get_property`，写时调用 `set_property`（若实现）
- 常见文件示例：
  - `status`（充电状态）
  - `capacity`（百分比）
  - `voltage_now`、`current_now`、`temp`（单位通常为 uV/uA/0.1C）
  - `online`（适配器/USB 是否在线）
- 更新机制：
  - 驱动调用 `power_supply_changed()` 后，内核会刷新这些属性
  - 同时触发 uevent，用户空间（如 upower、Android healthd）会收到更新

## 3) rk817 电池驱动的 power_supply 流程
文件：`Operating_System/Linux_Develop/Drivers/Power/Supply/rk817_battery.c`

### 3.1 设备注册
- `rk817_bat_init_power_supply()`：
  - 使用 `devm_power_supply_register()` 注册电池设备
  - `rk817_bat_desc` 定义 name/props/get_property
- `rk809_chg_init_power_supply()`：
  - 注册充电器供电设备（如充电状态/电流）

### 3.2 属性提供
- `rk817_bat_props[]`：声明电池属性集合
- `rk817_battery_get_property()`：具体返回值逻辑
  - `voltage_now`：`rk817_bat_get_battery_voltage()`
  - `current_now`：`rk817_bat_get_avg_current()`
  - `capacity`：`rk817_get_capacity_leve()`（内部使用 rsoc/dsoc 计算）
  - `status`：根据充电状态/外部电源状态判断

### 3.3 状态更新（周期 + 插拔中断）
- 周期性 work：`rk817_battery_work()`
  - 更新电压/电流/容量
  - 运行 DSOC 算法（平滑/归零/结束充电）
  - 保存数据并 `power_supply_changed()`
- 插拔事件：
  - 中断处理后触发 `power_supply_changed()`，及时更新 sysfs 和 uevent

## 4) rk817 低电“归零算法”流程（rk817_bat_zero_algorithm）
这是 DSOC（显示电量）在低电压区间的校准策略，防止掉电前电量显示突变。

### 4.1 入口条件
在主流程中，当低电压/低电量满足条件时进入：
- `rk817_bat_display_smooth()` 内部根据电压阈值触发零算法
- 阈值来自 DT：`zero_algorithm_vol`

### 4.2 关键函数与思路
1) `rk817_bat_zero_algo_prepare()`
   - 将 `zero_dsoc` 与平滑 DSOC 对齐
   - 调用 `rk817_bat_calc_zero_linek()` 计算零电区间的下降斜率

2) `rk817_bat_calc_zero_linek()`
   - 估算关机电压（`pwroff_vol` + 电流/路径电阻修正）
   - 由当前电压估算 OCV，再映射为 `ocv_soc/ocv_cap`
   - 计算 `xsoc = ocv_soc - dead_soc`（可用电量区间）
   - 根据负载大小、当前 dsoc、xsoc 差值调整 `zero_linek`（降速/保留）
   - 处理极低电压：超过 `WAIT_SHTD_DROP_SEC` 直接把 dsoc 拉到 0

3) `rk817_bat_zero_algorithm()`
   - 根据剩余容量变化 `delta_cap` 估算 `delta_soc`
   - 触发条件：
     - `delta_soc` >= `MIN_ZERO_DSOC_ACCURACY`
     - 或超时 `MIN_ZERO_OVERCNT`
     - 或 `zero_linek == 0`
   - 更新 `zero_dsoc -> dsoc`，并再次更新 `zero_linek`
   - 若 rsoc 极低但 OCV 显示仍有余量，会重置库仑计容量

### 4.3 结果
低电区间 DSOC 会以受控速度下降，尽量避免电量显示跳变，同时保留一部分“安全电量”以保障关机前稳定性。

## 5) 你在 sysfs 里能看到的变化
- 低电时 `capacity` 下降更平滑
- 插拔充电器时 `status`/`online` 等属性会快速更新
- 若你需要观察具体属性变化，可持续读取：
  - `/sys/class/power_supply/<battery>/capacity`
  - `/sys/class/power_supply/<battery>/voltage_now`
  - `/sys/class/power_supply/<battery>/current_now`

## 6) 电池曲线（OCV-SoC/电压-容量表）
电池曲线用于把电压映射为 SOC 或容量，是 rsoc/dsoc 算法的重要基础。

### 6.1 数据来源
- 在 DT 中以 `ocv_table` 给出电压点（mV），例如：
  - `Operating_System/Linux_Develop/Drivers/Power/Supply/rk817.dts`
  - `ocv_table = <3400 3447 3482 3525 3564 3600 3633 3662 3695 3732 3770 3814 3861 3900 3936 3978 4020 4054 4073 4099 4182 4195>;`
- 驱动会读取该表并用于电压到 SOC/容量的插值计算。

### 6.2 典型用途
- 开机/恢复时用 OCV 估算初始 SOC
  - `rk817_bat_get_ocv_voltage()` + `rk817_bat_vol_to_soc()`
- 低电归零算法里估算剩余区间
  - `rk817_bat_calc_zero_linek()` 中用 OCV 算 `xsoc`
- 休眠唤醒后做电量修正
  - 通过 relax/ocv 电压重新映射电量

### 6.3 插值与曲线含义
- 表中的电压点对应从低到高的 SOC 增长区间
- 驱动内部会根据电压落点做线性插值，得到更平滑的 SOC/容量
- 曲线越符合真实电池的放电特性，SOC 显示越稳定

### 6.4 调参建议
- `ocv_table` 要和电池型号匹配（化学体系、容量、倍率）
- 若低电跳变明显，可调整低端电压点的分布密度
- 若高电量区间不敏感，可提高高电压区间的分辨率
