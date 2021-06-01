本项目在windows和linux环境(编译器为g++8.0+)下都可以通过编译。 

# 构建
## 编译
```bash
# 工作目录为当前项目根目录
mkdir build
cd build
cmake ..
make -j4
```
## 生成静态库
```bash
# 工作目录为当前项目根目录
cd build
# 编译
make -j4
# 在build下生成dist文件夹，里面包含include和libs两个文件夹
make generate

# 使用clean_gen删除dist文件夹
make clean_gen
```