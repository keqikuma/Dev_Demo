
# I2C 驱动模型框架总结（以 Linux 内核为例）

本篇文档以 Linux I2C 驱动为例，总结其驱动模型框架，包括核心结构体、工作流程、设备匹配机制及驱动模板示例。

---

## 一、驱动模型总览

I2C 驱动模型与 Linux 统一的设备模型架构（driver-model）融合，遵循“总线-设备-驱动”三层模型。

```
          ┌──────────────┐
          │  struct bus_type （i2c_bus_type） │
          └──────┬───────┘
                 │
     ┌────────────┴─────────────┐
     │                          │
struct i2c_client       struct i2c_driver
（表示从设备）           （驱动程序）
     │                          │
     └──────────匹配───────────┘
```

---

## 二、核心结构体说明

### 1. struct i2c_client（表示 I2C 从设备）

```c
struct i2c_client {
    unsigned short addr;               // I2C 从设备地址
    const char *name;                 // 名字，用于匹配驱动
    struct i2c_adapter *adapter;      // 所属适配器（I2C 控制器）
    struct device dev;                // 内嵌的 device 对象
    ...
};
```

### 2. struct i2c_driver（表示驱动）

```c
struct i2c_driver {
    const char *driver.name;          // 驱动名称
    int (*probe)(...);                // 匹配成功时调用
    int (*remove)(...);               // 移除时调用
    const struct i2c_device_id *id_table;   // 匹配表
    const struct of_device_id *of_match_table; // 设备树匹配表
    struct device_driver driver;      // 内嵌的通用 driver 对象
};
```

### 3. struct i2c_adapter（控制器/主机）

每个适配器表示一个主机控制器（比如 I2C0、I2C1），负责发起读写操作。

---

## 三、设备与驱动匹配机制

匹配的核心流程如下：

1. 注册 i2c_client（platform 层 or 设备树）
2. 注册 i2c_driver 驱动
3. 根据 `i2c_client->name` 匹配 `i2c_driver->id_table.name`
4. 若匹配成功，调用 `i2c_driver.probe()`

设备树匹配则使用 `.of_match_table`

---

## 四、典型工作流程

### 设备注册（通过 platform 或设备树）

```c
&i2c1 {
    status = "okay";
    mychip@50 {
        compatible = "myvendor,mychip";
        reg = <0x50>;
    };
}
```

### 驱动注册

```c
static const struct i2c_device_id mychip_id[] = {
    { "mychip", 0 },
    { }
};

static const struct of_device_id mychip_of_match[] = {
    { .compatible = "myvendor,mychip" },
    { }
};

static struct i2c_driver mychip_driver = {
    .driver = {
        .name = "mychip",
        .of_match_table = mychip_of_match,
    },
    .probe = mychip_probe,
    .remove = mychip_remove,
    .id_table = mychip_id,
};
module_i2c_driver(mychip_driver);
```

---

## 五、驱动模板简要示例

```c
static int mychip_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    dev_info(&client->dev, "mychip detected at 0x%x\n", client->addr);
    return 0;
}

static int mychip_remove(struct i2c_client *client)
{
    dev_info(&client->dev, "mychip removed\n");
    return 0;
}
```

---

## 六、常用 API

| 函数 | 作用 |
|------|------|
| `i2c_smbus_read_byte_data()` | 读取寄存器 |
| `i2c_smbus_write_byte_data()` | 写寄存器 |
| `devm_ioremap()` | 内存映射 |
| `devm_kzalloc()` | 自动释放内存分配 |

---

## 七、总结

| 项目 | 内容 |
|------|------|
| 通信协议 | I²C（双线制，主从结构） |
| 总线类型 | `i2c_bus_type` |
| 设备结构 | `i2c_client` |
| 驱动结构 | `i2c_driver` |
| 匹配机制 | name / compatible / id_table |

---
