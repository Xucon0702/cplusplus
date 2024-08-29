#!/bin/bash

# 定义输入目录和输出目录
TOP_PROTO_DIR=`pwd`
PROTO_DIR=$TOP_PROTO_DIR/protobuf/proto
PROTO_OUT_DIR=$TOP_PROTO_DIR/protobuf/proto_out

echo "PROTO_DIR:$PROTO_DIR;PROTO_OUT_DIR:$PROTO_OUT_DIR"

# 创建输出目录（如果不存在）
if [ ! -d "$PROTO_OUT_DIR" ]; then
    mkdir -p "$PROTO_OUT_DIR"
fi

find "$PROTO_DIR" -type f -name "*.proto" | while read -r file; do
    # 获取文件名（不带路径）
    filename=$(basename "$file")
    
    # 计算目标输出路径
    # 生成目录下添加原文件名目录的结构
    # output_path=$PROTO_OUT_DIR/${filename%.proto}
    output_path=$PROTO_OUT_DIR/

    # # 创建输出目录（如果不存在）
    # if [ ! -d "$output_path" ]; then
    #     mkdir -p "$output_path"
    # fi

    # 生成对应的文件
    # 指定 --proto_path 选项
    protoc --proto_path="$PROTO_DIR" --cpp_out=$output_path "$file"

    # echo "output_path:$output_path"
    # echo "filename:$filename;${filename%.proto}"    
    # echo "file:$file"
done

# 输出完成信息
echo "Proto files processed and generated in $PROTO_OUT_DIR"