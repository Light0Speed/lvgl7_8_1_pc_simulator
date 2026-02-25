# 下拉 Bar 脏块问题完整分析

## 问题现象

从 `time_lbl` 开始下拉时，下拉 bar 出现**显示不全和脏块**（旧像素残留）。从 `btn` 开始下拉则不会出现此问题。

---

## 一、脏块的根因分析

### 1.1 LVGL 渲染机制基础

LVGL 不是每次改变就立即画屏。它用**脏矩形**（dirty rectangle）机制：

1. 任何对象坐标改变 → 调用 `lv_obj_invalidate()` → 把该对象覆盖的屏幕区域标记为"脏"
2. 一帧结束时，`lv_task_handler()` → `lv_refr_task()` 统一把所有"脏"区域重新绘制
3. 只重绘脏区域，不重绘整个屏幕（省性能）

**脏块 = 某块区域应该重绘但没有被标记为脏 → 保留了旧像素。**

### 1.2 `lv_obj_set_pos` 内部做了什么

```c
void lv_obj_set_pos(obj, x, y) {
    // 第1步：标记旧位置为脏（旧位置需要重绘来"擦除"旧内容）
    lv_obj_invalidate(obj);        // ← 脏矩形 A

    // 第2步：更新坐标
    obj->coords.x1 += diff.x;
    obj->coords.y1 += diff.y;
    obj->coords.x2 += diff.x;
    obj->coords.y2 += diff.y;

    // 第3步：递归更新所有子对象坐标
    refresh_children_position(obj, diff.x, diff.y);

    // 第4步：发送 COORD_CHG 信号（通知信号处理函数坐标变了）
    obj->signal_cb(obj, LV_SIGNAL_COORD_CHG, &ori);

    // 第5步：标记新位置为脏（新位置需要重绘来"画上"新内容）
    lv_obj_invalidate(obj);        // ← 脏矩形 B
}
```

正常情况下这没问题：旧位置被擦除，新位置被绘制。

### 1.3 `lv_tileview_add_element` 的副作用

```c
void lv_tileview_add_element(tileview, element) {
    lv_page_glue_obj(element, true);
}

void lv_page_glue_obj(obj, true) {
    lv_obj_set_drag_parent(obj, true);   // 拖拽事件传给父对象
    lv_obj_set_drag(obj, true);          // 这个对象可以被拖拽
}
```

所以 `time_lbl` 被设置成了**可拖拽**，**拖拽时把事件传给父对象**（scrollable → tileview）。

### 1.4 逐帧分析：脏块是怎么产生的

用户手指按在 `time_lbl` 上往下拖了 5px，以下是**一帧内**的完整执行顺序：

#### 阶段 1：`lv_indev_proc` 拖拽 scrollable

`time_lbl` 被 `lv_tileview_add_element` 设置了 `drag=true, drag_parent=true`。LVGL 的 `get_dragged_obj()` 沿父链上溯找到 `scrollable`，调用：

```c
lv_obj_set_pos(scrollable, x=-800, y=5)
```

`lv_obj_set_pos` 内部：

```
步骤A: lv_obj_invalidate(scrollable)
        → scrollable 当前位置 (0,0)-(799,599) 被标记为脏
        → 脏矩形①: y=0~599

步骤B: scrollable.coords.y1 = 0 → 5（坐标改了）
        scrollable.coords.y2 = 599 → 604
        refresh_children_position(+5)（所有子对象 coords 全部+5）

步骤C: signal_cb(COORD_CHG)  ← 触发 lv_tileview_scrl_signal（见阶段2）

步骤D: lv_obj_invalidate(scrollable)
        → scrollable 新位置 (0,5)-(799,604) 被标记为脏
        → 脏矩形②: y=5~604
```

**此时 scrollable 已从 y=0 偏移到 y=5。**

#### 阶段 2：`lv_tileview_scrl_signal(COORD_CHG)` 锁回

这是在阶段 1 步骤 C 的 `signal_cb(COORD_CHG)` 里面被调用的。

tileview 检查垂直方向：`drag_top_en == 0`（没有上面的页），scrollable 不应该垂直移动，强制锁回：

```c
// lv_tileview.c:363-366
if(!ext->drag_top_en && y > -(ext->act_id.y * h)) {
    lv_page_start_edge_flash(tileview, TOP);    // ← 触发边缘闪光动画！
    lv_obj_set_y(scrl, 正确位置);                // ← 把 scrollable 锁回 y=0！
}
```

`lv_obj_set_y` 内部又调 `lv_obj_set_pos`：

```
步骤E: lv_obj_invalidate(scrollable)
        → 脏矩形③: y=5~604（当前位置）

步骤F: scrollable.coords.y1 = 5 → 0（坐标改回来）
        scrollable.coords.y2 = 604 → 599
        refresh_children_position(-5)（所有子对象坐标全部-5，改回来）

步骤G: lv_obj_invalidate(scrollable)
        → 脏矩形④: y=0~599（锁回后位置）
```

另外 `lv_page_start_edge_flash()` 还启动了一个**边缘闪光动画**，在后续帧中会触发额外的 invalidate。

**此时 scrollable 又回到 y=0。scrollable 在一帧内经历了 0→5→0 的过程。**

#### 阶段 3：我们的代码移动 panel

```c
lv_obj_set_y(drop_down_panel, act_pt.y - 600)  // 比如 y=-595
```

```
步骤H: lv_obj_invalidate(panel)
        → 脏矩形⑤: panel 旧位置

步骤I: panel.coords 更新

步骤J: lv_obj_invalidate(panel)
        → panel 新位置 (0,-595)-(799,5) 被标记为脏
        → 但 lv_obj_area_is_visible 用屏幕 (0,0)-(799,599) 裁剪
        → 实际标记的脏矩形⑥: y=0~5（只有 5px 高的条带）
```

#### 阶段 4：帧结束，`lv_refr_task` 开始渲染

渲染器拿到这一帧累积的所有脏矩形：

```
脏矩形①: y=0~599    (scrollable 被推走前)
脏矩形②: y=5~604    (scrollable 被推走后)
脏矩形③: y=5~604    (scrollable 被锁回前，和②重复)
脏矩形④: y=0~599    (scrollable 被锁回后，和①重复)
脏矩形⑤: panel 旧位置
脏矩形⑥: y=0~5      (panel 新位置可见部分)
```

### 1.5 脏块的真正原因

**不是 tileview 覆盖了 panel**。LVGL 按对象树从底到顶绘制，panel 在最上面画，不会被 tileview 覆盖。

**真正的问题是：panel 移动后露出的 tileview 区域没有被 invalidate，旧的 panel 像素残留在那里。**

具体来说：
1. scrollable 在一帧内被 `lv_obj_set_pos` 改了两次（推走+锁回），这本身产生的脏矩形虽然大部分能互相覆盖
2. tileview 在锁回时触发了 **edge flash 动画**，在**后续帧**中持续 invalidate 的区域很小（只有闪光条），不覆盖 panel 的移动轨迹
3. panel 在后续帧中继续跟手移动，`lv_obj_set_y(panel)` 标记的脏矩形只覆盖 panel 自身的旧位置和新位置
4. **scrollable 的子对象（如 time_lbl）在 panel 移动经过的区域需要重绘（因为 panel 从上面划过去了），但没有人 invalidate 这块区域** → 旧的 panel 像素残留 → 脏块

### 1.6 渲染顺序说明

```
lv_scr_act              ← 第1层：画背景
  └── tileview          ← 第2层
      └── scrollable    ← 第3层：画所有 tile 内容(time_lbl等)
  └── leds_content      ← 第4层：画 LED 指示器
  └── drop_down_panel   ← 第5层：画下拉面板（最后画，在最上面）
```

**panel 在最上面画，不会被 tileview 覆盖。如果是"被覆盖"导致的，应该看到 tileview 的内容画在 panel 上面——但实际看到的是脏块残影（旧帧的像素留在那里），说明问题不是覆盖，而是漏画。**

---

## 二、为什么水平滑动不出现脏块

核心原因：**水平滑动时 `lv_indev_proc` 的拖拽方向和 tileview 的合法方向一致，不产生"推走又锁回"的矛盾**。

### 2.1 水平滑动的完整流程

#### `lv_indev_proc` 检测到水平拖拽

```
lv_indev_proc:
  drag_dir = LV_DRAG_DIR_HOR  ← 方向是水平
  lv_obj_set_pos(tile_scrl, x=-805, y=0)  ← 只改 x，不改 y
```

#### `lv_tileview_scrl_signal(COORD_CHG)` 检查约束

```c
// 检查水平方向：
// drag_right_en == 1（因为有下一页）→ 不触发 edge flash，不锁回

// 最后的约束代码：
if(drag_dir == LV_DRAG_DIR_HOR)
    lv_obj_set_y(scrl, -act_id.y * h);  // 锁定 Y 不变（Y 本来就没变，diff=0）
```

关键：`lv_obj_set_y` 内部调 `lv_obj_set_pos`，但：
```c
if(diff.x == 0 && diff.y == 0) return;  // Y 没变 → 直接返回，什么都不做
```

**水平滑动时 tileview 的约束代码不产生任何额外的 `lv_obj_set_pos` 调用**。没有"锁回"操作，没有额外 invalidate。

### 2.2 对比表

| | 水平滑动 | 垂直下拉 |
|---|---|---|
| `drag_dir` | `LV_DRAG_DIR_HOR` | `LV_DRAG_DIR_VER` |
| tileview 允许这个方向？ | ✅ 有左右相邻 tile | ❌ 没有上下相邻 tile |
| `drag_left/right_en` | 1 | — |
| `drag_top/bottom_en` | — | 0 |
| tileview 约束行为 | 锁 Y（Y 本来没变→`diff==0`→return） | 锁回 Y（Y 被 indev 改了→`lv_obj_set_pos`→invalidate×2） |
| edge flash | 不触发 | `lv_page_start_edge_flash(TOP)` |
| `lv_obj_set_pos` 调用次数 | 1 次（indev） | 2 次（indev + tileview 锁回） |
| invalidate 次数 | 2 | 4 + edge flash |
| 加上 panel | — | +2 = 总共 7 |
| **结果** | **干净** | **脏块** |

**一句话**：水平滑动方向是 tileview 的**合法方向**（`drag_right_en=1`），scrollable 被 indev 移动后 tileview **不锁回**；垂直下拉是**非法方向**（`drag_top_en=0`），scrollable 被 indev 移动后 tileview **强制锁回** → 同一帧内 scrollable 的坐标被来回改两次 → 多出无效脏矩形与 panel 的脏矩形冲突 → 脏块。

---

## 三、为什么从 btn 下拉不出现脏块

### 3.1 对象树结构

```
scrollable (drag=true, drag_parent=false)
  ├── tab (drag=true, drag_parent=true)        ← lv_tileview_add_element 设置
  │   ├── time_lbl (drag=true, drag_parent=true) ← lv_tileview_add_element 设置
  │   └── date_lbl (drag=true, drag_parent=true) ← lv_tileview_add_element 设置
  └── tab (drag=true, drag_parent=true)        ← lv_tileview_add_element 设置
      └── cont (drag=false, drag_parent=false)  ← menu_create 创建，没有 add_element
          └── btn (drag=true, drag_parent=true) ← lv_tileview_add_element 设置
```

### 3.2 `get_dragged_obj` 上溯链对比

LVGL 的 `indev_drag` 函数通过 `get_dragged_obj()` 沿 `drag_parent` 链向上找实际被拖拽的对象：

```c
static lv_obj_t * get_dragged_obj(lv_obj_t * obj) {
    lv_obj_t * drag_obj = obj;
    while(lv_obj_get_drag_parent(drag_obj) != false && drag_obj != NULL) {
        drag_obj = lv_obj_get_parent(drag_obj);
    }
    return drag_obj;
}
```

然后检查 `lv_obj_get_drag(drag_obj)`，如果为 `false` 就不拖拽。

| 按在哪 | `get_dragged_obj` 上溯链 | 最终 drag_obj | `drag == true?` | scrollable 被拖？ |
|---|---|---|---|---|
| `time_lbl` | time_lbl(parent=true) → tab(parent=true) → **scrollable**(parent=false) | scrollable | ✅ true | ✅ **被拖** → 脏块 |
| `btn` | btn(parent=true) → **cont**(parent=false) | cont | ❌ false | ❌ **不拖** → 无脏块 |

### 3.3 关键差异

`btn` 和 `scrollable` 之间隔了一个 `cont`，这个 `cont` 是 `menu_create` 中 `lv_cont_create(tab, NULL)` 创建的，**没有调用 `lv_tileview_add_element`**，所以它的 `drag_parent = false`（默认值）。

`get_dragged_obj` 的上溯链在 `cont` 处就**断开**了，不会到达 `scrollable`。scrollable 不被拖动，不会有"推走又锁回"，不会产生冲突的脏矩形。

而 `time_lbl` 的父对象 `tab`（由 `launcher_tileview_add_tile` 创建）**调了 `lv_tileview_add_element`**，所以 `tab` 的 `drag_parent = true`，上溯链可以一路穿透到 `scrollable`。

---

## 四、松手后 panel 卡住不收起的问题

### 4.1 问题现象

从 `date_lbl` 开始下滑时脏块问题修复了（通过 `lv_task` 持续 invalidate），但松手后 panel 停在原地，不会调用 `drop_down_snap_back` 或 `drop_down_dismiss`。

### 4.2 根因：RELEASED 信号发给了谁

**和继承关系无关**，纯粹是 LVGL 输入系统的**信号路由机制**问题。

LVGL 的 `lv_indev_proc` 在松手时有两条不同的信号路径：

```c
// lv_indev.c:1048
indev_obj_act->signal_cb(indev_obj_act, LV_SIGNAL_RELEASED, indev_act);
```

**`RELEASED` 信号发给了 `indev_obj_act`**（手指最初按下的那个对象）。

当手指按在 `date_lbl` 上时，`indev_obj_act = date_lbl`。`RELEASED` 发给 `date_lbl` → `date_lbl` 的 `signal_cb` 是 `lv_label` 的默认信号函数 → **不是我们的 `launcher_tileview_scrl_signal`**。

我们的 handler 挂在 **scrollable** 上，`RELEASED` 信号不会传到 scrollable。

### 4.3 信号路由表

| 信号 | 发给谁 | 机制 | 我们的 handler 收到？ |
|---|---|---|---|
| `PRESSING` | `date_lbl` | `indev_obj_act->signal_cb` | ❌ 直接收不到，但 drag → `lv_obj_set_pos(scrollable)` → `COORD_CHG` → ✅ 间接收到 |
| `RELEASED` | `date_lbl` | `indev_obj_act->signal_cb` | ❌ **收不到** |
| `PRESS_LOST` | `date_lbl` | `indev_obj_act->signal_cb` | ❌ **收不到** |
| `DRAG_THROW_BEGIN` | `scrollable` | `get_dragged_obj()->signal_cb` | ✅ **收到** |

### 4.4 修复

在 `DRAG_THROW_BEGIN` 和 `DRAG_END` 中也处理 panel 的 snap/dismiss：

```c
} else if(LV_SIGNAL_DRAG_END == sign || LV_SIGNAL_DRAG_THROW_BEGIN == sign) {
    if(drop_down_panel != NULL) {
        drop_down_refr_task_stop();
        // ... 判断位置，调用 snap_back 或 dismiss ...
        lv_indev_finish_drag(indev);
    }
}
```

---

## 五、解决方案汇总

### 方案 1：`lv_task` 持续 invalidate（当前采用）

下滑开始时创建 10ms 周期的 task，每次回调 `lv_obj_invalidate(drop_down_panel)`，松手或销毁时删除 task。

**优点**：简单，不需要改 LVGL 内部逻辑。
**缺点**：每 10ms 一次全 panel invalidate，性能略低。

### 方案 2：panel 放到 `layer_top`

```c
drop_down_panel = lv_obj_create(lv_disp_get_layer_top(NULL), NULL);
```

`layer_top` 有独立的渲染层，invalidate 不会与 tileview 冲突。

**优点**：干净，性能好。
**缺点**：需要注意 `layer_top` 上的对象在所有 screen 之上。

### 方案 3：跳过 ancestor + 静默 coords 修正

下拉模式下不调用 `ancestor_tileview_scrl_signal`（避免 edge flash 和锁回），同时直接写 scrollable coords 归位（绕过 `lv_obj_set_pos`，零额外 invalidate）。

**优点**：从根源消除冲突，性能最好。
**缺点**：需要自己实现 `fix_children_coords` 递归修正子对象坐标，比较 hack。

### 方案 4：禁用 scrollable 拖拽

下拉开始时 `lv_obj_set_drag(tile_scrl, false)`，松手时恢复 `true`。

**优点**：从源头阻止 scrollable 被拖动。
**缺点**：需要管理状态恢复，可能影响水平滑动。
