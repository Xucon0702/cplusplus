protobuf的使用
第一步：定义 .proto 文件
第二步：生成 C++ 代码
    sudo apt-get install protobuf-compiler libprotobuf-dev
    protoc --cpp_out=. person.proto
第三步：编写发送和接收代码
第四步：编译和运行


工程中先执行脚本生成proto对应的所有文件:generate_proto_files.sh 