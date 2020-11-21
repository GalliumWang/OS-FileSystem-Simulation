#ifndef _operate_
#define _operate_
#define MAX_NAME_LENGTH  20    //文件名或者目录名字最大字节数

#include<iostream>
#include<string>
#include<iomanip>
//#include<stdlib.h>
//#include<stdio.h>


#include "rang.hpp"
using namespace std;

typedef struct OneFile         //文件结构体
{
    char name[MAX_NAME_LENGTH];      //文件名
    int size;                 //文件大小
    struct OneFile* nextFile;  //指向文件列表中下一个文件
    string content;           //文件内容
} OneFile;

typedef struct OneDir          //目录结构体
{
    char name[MAX_NAME_LENGTH];      //目录名字
    int size;                 //目录大小
    OneDir* nextDir;           //后继目录
    OneDir* preDir;            //前继目录
    OneFile* filePtr;          //该目录下的文件链表指针
    OneDir* dirPtr;            //该目录下的目录链表指针
} OneDir;


class OneFileSystem //文件系统类
{
private:
    OneDir* currentDir; //当前目录
    OneFile* copytempfile; //用于拷贝文件时的临时文件
    OneDir* copytempdir; //用于拷贝目录时的临时目录
    OneDir* root; //根目录

    char password[MAX_NAME_LENGTH];   //用户密码
    char name[MAX_NAME_LENGTH];   //用户名称
    int size; //用户所使用空间大小

public:
    OneFileSystem();
    ~OneFileSystem();

    bool mkFile(); //创建文件
    bool ptFile(); //粘贴文件
    bool edFile(); //编辑文件
    bool dele_file(OneFile* file);//删除文件
    bool deleteFile(); //删除文件前的逻辑判断
    bool reFile(); //重命名文件
    bool readFile(); //打开文件

    bool dele_dir(OneDir* d); //删除目录
    bool deleteDir(); //删除目录前的逻辑判断
    bool mkDir(); //创建目录
    bool reDir(); //重命名目录
    bool ptDir(); //粘贴目录
    bool showCurrentDir(); //显示当前目录内容
    bool show_path(OneDir* dir); //显示路径的部分实现
    bool showPath(); //显示路径
    bool goback(); //返回上一级目录
    bool gobacktoRoot();//返回根目录
    bool gotoDir();//访问下级目录或绝对路径

    OneDir* copy_dir(OneDir* dir);//复制目录
    OneDir* cpDir(); //复制目录前的逻辑判断
    OneFile* copy_file(OneFile* h); //复制文件
    OneFile* cpFile(); //复制文件前的逻辑判断

    bool setUser(char*, char*);//设置用户名与密码
    bool search();//检索文件或目录
    bool search_file(char n[MAX_NAME_LENGTH]); //递归调用检索文件
    bool search_dir(char n[MAX_NAME_LENGTH]);  //递归调用检索目录

    int getSize();//获得用户所在目录大小
    int getRemain();//获得剩余空间大小
    OneDir* getCurrentdir(); //获得当前目录
};

#endif // 

