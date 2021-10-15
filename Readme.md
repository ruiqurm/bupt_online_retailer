# BUPT C++ OOP
c++课程设计项目   
具体设计见doc.pdf。



linux环境(编译器为g++>8.0 + protbuf)下都可以通过编译。 

# 构建
因为链接出了点问题，所以就用qmake来编译qt了。  

## requirements
* g++ 8.0+(使用了c++ 17语法)
* google protobuf(part3服务端用到了)
* qt5+

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

# 在build下生成dist文件夹，里面包含include和libs两个文件夹
make generate

# 使用clean_gen删除dist文件夹
make clean_gen
```

## 移动静态库到ui文件夹下
```
make copy_to_ui
```
