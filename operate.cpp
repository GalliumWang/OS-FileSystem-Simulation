#include "operate.h"
#include <unistd.h>
#include <iostream>
#include  <stdio.h>
#include  <stdlib.h>
#include <string.h>
using namespace std;

int disk_size = 100; //虚拟磁盘空闲空间大小

string Invaild[] = { "/","\\", ":","<",">","|","*","&" };  //命名中的非法字符

OneFileSystem::OneFileSystem() {
    size = 0;
    currentDir = nullptr;
    copytempfile = nullptr;
    copytempdir = nullptr;
}

OneFileSystem::~OneFileSystem() {
    disk_size += size;		//释放空间
    size = 0;				// 置0

    OneDir* directory = root;
    OneFile* file = currentDir->filePtr;
    while (file != nullptr) {
        if (file->nextFile == nullptr) {
            this->dele_file(file);
            file = nullptr;
            break;
        }
        while (file->nextFile->nextFile != nullptr)
            file = file->nextFile;
        this->dele_file(file->nextFile);
        file->nextFile = nullptr;
        file = currentDir->filePtr;
    }
    while (directory != nullptr) {
        if (directory->nextDir == nullptr) {
            this->dele_dir(directory);
            directory = nullptr;
            break;
        }
        while (directory->nextDir->nextDir != nullptr)
            directory = directory->nextDir;
        this->dele_dir(directory->nextDir);
        directory->nextDir = nullptr;
        directory = root;
    }
}

bool OneFileSystem::mkFile() {

    OneFile* tempFile = nullptr;
    tempFile = new OneFile;
    cin >> tempFile->name;
    /*命名检测*/
    string tempname(tempFile->name);
    for (int i = 0;i < 8;++i)
    {
        if (tempname.find(Invaild[i], 0) != string::npos)//从字符串的下标为0处开始查找非法字符
        {
            cout << "文件命名包含非法字符！          -出错" << endl;
            return false;
        }
    }

    if (currentDir->filePtr == nullptr) {
        tempFile->nextFile = currentDir->filePtr;
        currentDir->filePtr = tempFile;
    }
    else {
        OneFile* q = new OneFile;
        q = currentDir->filePtr;
        while (q != nullptr) {
            if (strcmp(tempFile->name, q->name) == 0) {
                cout << "文件已存在！             -出错" << endl;
                return false;
            }
            q = q->nextFile;
        }

        /*重置链表结构*/
        tempFile->nextFile = currentDir->filePtr;
        currentDir->filePtr = tempFile;
        OneDir* ptr = currentDir;

        /*更改上级目录的大小*/
        while (ptr != nullptr) {
            ptr->size += tempFile->size;
            ptr = ptr->preDir;
        }
    }
    currentDir->filePtr->size = 0;
    cout << "新建文件成功！" << endl;
    disk_size = disk_size - tempFile->size;
    size += tempFile->size;
    return true;
}

bool OneFileSystem::mkDir() {
    OneDir* tempFile, * ptr;
    tempFile = new OneDir;
    cin >> tempFile->name;

    string tempname(tempFile->name);
    for (int i = 0;i < 8;++i)
    {
        if (tempname.find(Invaild[i], 0) != string::npos)
        {
            cout << "目录名称包含非法字符！           -出错" << endl;
            return false;
        }
    }

    tempFile->dirPtr = nullptr;
    tempFile->size = 0;
    tempFile->filePtr = nullptr;
    tempFile->nextDir = nullptr;
    if (currentDir->dirPtr == nullptr)
        ptr = nullptr;
    else
        ptr = currentDir->dirPtr;

//	 1. 目录下没有子目录
//	 2. 目录下有子目录，命名冲突
//	 3. 目录下有子目录，无命名冲突

    while (ptr != nullptr) {
        if (strcmp(ptr->name, tempFile->name) == 0) {
            cout << "同名目录已存在！          -出错" << endl;
            return false;
        }
        ptr = ptr->nextDir;
    }

    /*重置链表结构*/
    tempFile->preDir = currentDir;
    tempFile->nextDir = currentDir->dirPtr;
    currentDir->dirPtr = tempFile;

    cout << "创建目录成功！" << endl;
    return true;
}

bool OneFileSystem::dele_file(OneFile* file) {
    cout<<"正在删除文件"<<file->name<<endl;
    delete file;
    file = nullptr;
    return true;
}

bool OneFileSystem::deleteFile() {
    char temp[MAX_NAME_LENGTH];

    cin >> temp;
    OneFile* file;
    OneFile* previous = nullptr;
    file = currentDir->filePtr;
//	 判断该目录下有无需要删除的文件

    while (file != nullptr) {
        if (!strcmp(file->name, temp))
            break;
        previous = file;
        file = file->nextFile;
    }
    if (file == nullptr) {
        cout << "该文件不存在              -出错" << endl;
        return false;
    }
    disk_size += file->size;
    OneDir* directory = currentDir;
    while (directory != nullptr) //修改删除文件后各级目录的大小
    {
        directory->size -= file->size;
        directory = directory->preDir;
    }

    if (file == currentDir->filePtr)//删除文件结点
        currentDir->filePtr = currentDir->filePtr->nextFile;
    else
        previous->nextFile = file->nextFile;
    size -= file->size;
    delete file;
    file = nullptr;
    cout << "文件已删除！" << endl;
    return true;
}

bool OneFileSystem::dele_dir(OneDir* directory) {
    OneDir* dir = directory->dirPtr;
    OneFile* file = directory->filePtr;
    if(file!= nullptr){
        cout<<"该目录下包含文件！             -警告"<<endl;
        sleep(2);
        while(directory->filePtr->nextFile!=nullptr) {    //删除此目录下的文件
            file = directory->filePtr;
            while (file->nextFile->nextFile != nullptr)  //寻找最后一个文件结点
                file = file->nextFile;
            this->dele_file(file->nextFile);
            file->nextFile = nullptr;
        }
            if(directory->filePtr->nextFile == nullptr){
                this->dele_file(directory->filePtr);
                directory->filePtr = nullptr;
            }
        }
        if(dir!= nullptr){
            cout<<"该目录下包含子目录！             -警告"<<endl;
            while(directory->dirPtr->nextDir!= nullptr){  //删除此目录下的子目录
                dir = directory ->dirPtr;
                while(dir->nextDir->nextDir!= nullptr) //寻找最后一个文件结点
                dir = dir->nextDir;
                this->dele_dir(dir->nextDir);
                dir->nextDir= nullptr;
            }
            if(directory->dirPtr->nextDir== nullptr){
                this->dele_dir(directory->dirPtr);
                directory->dirPtr = nullptr;
            }
        }
    cout<<"正在删除目录"<<directory->name<<endl;
    delete directory,dir,file;
    directory = nullptr;
    return true;
}

bool OneFileSystem::deleteDir() {
    char n[MAX_NAME_LENGTH];

    OneDir* tempFile, * pre = nullptr;
    tempFile = root;
    tempFile = currentDir->dirPtr;
    cin >> n; //删除的目录

    /*查找所需要删除的目录*/
    while (tempFile != nullptr) {
        if (strcmp(tempFile->name, n) == 0)
        {
            pre = tempFile;break;
        }
        tempFile = tempFile->nextDir;
    }

    if (tempFile == nullptr) {
        cout << "目录不存在,删除出错！            -出错！" << endl;
        return false;
    }
    disk_size += tempFile->size;
    if (tempFile == currentDir->dirPtr)
        currentDir->dirPtr = currentDir->dirPtr->nextDir;
    else
        tempFile->preDir->nextDir = tempFile->nextDir;

    pre = currentDir;
    while (pre != nullptr) //修改删除目录后各级目录大小
    {
        pre->size -= tempFile->size;
        pre = pre->preDir;
    }
    size -= tempFile->size;
    this->dele_dir(tempFile);

    cout << "目录删除成功！" << endl;
    return true;
}


bool OneFileSystem::gotoDir() {
    char name[100];
    OneDir* tempDir;
    const char *prefix = "/";
    tempDir = currentDir->dirPtr;
    cin >> name;
    char *dir;
    dir = strtok(name,prefix);
    while(dir)
    {
        while(tempDir != nullptr){
            if(strcmp(tempDir->name,dir)==0){
                currentDir = tempDir;
                tempDir = currentDir->dirPtr;
                break;
            }
            else{
                tempDir = tempDir -> nextDir;
            }
        }
        dir = strtok(nullptr,prefix);
    }
    return true;
}

bool OneFileSystem::readFile() {
    char n[MAX_NAME_LENGTH];
    cin >> n;
    OneFile* file = currentDir->filePtr;
    while (file != nullptr) {
        if (strcmp(file->name, n) == 0) {
            cout << file->content << endl;
            return true;
        }
        file = file->nextFile;
    }
    cout << "该文件不存在！              -出错" << endl;
    return false;
}

bool OneFileSystem::reDir() {
    OneDir* ptr;
    char n1[MAX_NAME_LENGTH], n2[MAX_NAME_LENGTH];
    cin >> n1;
    cin >> n2;
    ptr = currentDir->dirPtr;
    string tempname=n2;
    for (int i = 0;i < 8;++i)
    {
        if (tempname.find(Invaild[i], 0) != string::npos)//从字符串的下标为0处开始查找非法字符
        {
            cout << "目录命名包含非法字符！          -出错" << endl;
            return false;
        }
    }
    while (ptr != nullptr) {
        if (strcmp(ptr->name, n2) == 0) {
            cout << "目录名已存在！        -出错" << endl;
            return false;
        }
        ptr = ptr->nextDir;
    }
    ptr = currentDir->dirPtr;
    while (ptr != nullptr) {
        if (strcmp(ptr->name, n1) == 0)
            break;
        ptr = ptr->nextDir;
    }
    if (ptr == nullptr) {
        cout << "不存在该目录！         -出错" << endl;
        return false;
    }
    strcpy(ptr->name, n2);
    cout << "目录重命名成功！" << endl;
    return true;
}

bool OneFileSystem::reFile() {
    OneFile* ptr;
    char n1[MAX_NAME_LENGTH], n2[MAX_NAME_LENGTH];
    cin >> n1;
    cin >> n2;
    ptr = currentDir->filePtr;
    while (ptr != nullptr) {
        if (!strcmp(ptr->name, n2)) {
            cout << "该文件名已存在！           -出错" << endl;
            return false;
        }
        ptr = ptr->nextFile;
    }
    string tempname(n2);
    for (int i = 0;i < 8;++i)
    {
        if (tempname.find(Invaild[i], 0) != string::npos)//从字符串的下标为0处开始查找非法字符,如果没找到，返回一个特别的标志c++中用npos表示
        {
            cout << "文件命名包含非法字符！          -出错" << endl;
            return false;
        }
    }
    ptr = currentDir->filePtr;
    while (ptr != nullptr) {
        if (!strcmp(ptr->name, n1))
            break;
        ptr = ptr->nextFile;
    }
    if (ptr == nullptr) {
        cout << "文件不存在！            -出错" << endl;
        return false;
    }
    strcpy(ptr->name, n2);
    cout << "重命名文件成功！" << endl;
    return true;
}

OneDir* OneFileSystem::copy_dir(OneDir* directory) {
    OneFile* fh;
    OneDir* dh, * dir;
    dir = new OneDir;
    dir->dirPtr = nullptr;
    dir->filePtr = nullptr;
    dir->nextDir = nullptr;
    dir->preDir = nullptr;
    fh = directory->filePtr;
    dh = directory->dirPtr;

    while (fh != nullptr) {
        OneFile* file;
        file = copy_file(fh);
        file->nextFile = dir->filePtr;
        dir->filePtr = file;
        fh = fh->nextFile;
    }

    while (dh != NULL) {
        OneDir* dire;
        dire = copy_dir(dh);
        dire->preDir = dir;
        dire->nextDir = dir->dirPtr;
        dir->dirPtr = dire;

        dh = dh->nextDir;
    }

    strcpy(dir->name, directory->name);
    dir->size = directory->size;
    return dir;
}

OneDir* OneFileSystem::cpDir() {
    OneDir* ptr;
    char n[MAX_NAME_LENGTH];
    cin >> n;
    ptr = currentDir->dirPtr;
    while (ptr != nullptr) {
        if (!strcmp(ptr->name, n))
            break;
        ptr = ptr->nextDir;
    }
    if (ptr == nullptr) {
        cout << "目录不存在！            -出错" << endl;
        return nullptr;
    }
    if (copytempdir != NULL) {
        dele_dir(copytempdir);
    }
    copytempdir = copy_dir(ptr);
    cout << "目录拷贝成功！" << endl;
    return copytempdir;
}

OneFile* OneFileSystem::copy_file(OneFile* ptr) {
    OneFile* file = new OneFile;
    file->size = ptr->size;
    strcpy(file->name, ptr->name);
    file->content = ptr->content;
    return file;
}

OneFile* OneFileSystem::cpFile() {
    OneFile* ptr;
    char n[MAX_NAME_LENGTH];
    cin >> n;
    ptr = currentDir->filePtr;

    while (ptr != nullptr) {
        if (!strcmp(ptr->name, n))
            break;
        ptr = ptr->nextFile;
    }
    if (ptr == nullptr) {
        cout << "文件不存在！             -出错" << endl;
        return nullptr;
    }
    copytempfile = copy_file(ptr);
    cout << "文件复制成功！" << endl;
    return copytempfile;
}

bool OneFileSystem::ptDir() {
    OneDir* ptr, * d1;
    ptr = currentDir->dirPtr;

    if (copytempdir == nullptr) {
        cout << "源目录未拷贝成功            -出错" << endl;
        return false;
    }
    if (ptr == nullptr) {
        if (disk_size < copytempdir->size) {
            cout << "剩余空间不足！             -出租";
            return false;
        }
        disk_size -= copytempdir->size;
        d1 = copy_dir(copytempdir);
        currentDir->dirPtr = d1;
        d1->preDir = currentDir;
        currentDir->size += copytempdir->size;

    }
    else {
        while (ptr != nullptr) {
            if (!strcmp(ptr->name, copytempdir->name))
                break;
            ptr = ptr->nextDir;
        }
        if (ptr != nullptr) {
            cout << "目录已存在！         -出错" << endl;
            return false;
        }
        else {
            if (disk_size < copytempdir->size) {
                cout << "剩余空间不足！            -出错";
                return false;
            }
            d1 = copy_dir(copytempdir);
            d1->nextDir = currentDir->dirPtr;
            currentDir->dirPtr = d1;
            d1->preDir = currentDir;
            currentDir->size += copytempdir->size;
        }
    }
    cout << "粘贴目录成功！" << endl;
    return true;
}

bool OneFileSystem::ptFile() {
    OneFile* ptr = currentDir->filePtr;
    OneFile* pTemp = ptr;
    if (copytempfile == nullptr) {
        cout << "临时文件未创建成功！              -出错" << endl;
        return false;
    }
    /*如果当前目录没有文件*/
    if (ptr == nullptr) {
        if (disk_size < copytempfile->size) {
            cout << "剩余空间不足            -出错";
            return false;
        }
        currentDir->filePtr = copy_file(copytempfile);
        currentDir->size += copytempfile->size;
        cout << "文件粘贴成功！" << endl;
    }
    else {
        while (ptr != nullptr) {
            if (!strcmp(ptr->name, copytempfile->name)) {
                cout << "该文件名已存在！               -出错" << endl;
                return false;
            }
            ptr = ptr->nextFile;
        }

        if (disk_size < copytempfile->size) {
            cout << "剩余空间不足              -出错";
            return false;
        }
        currentDir->filePtr = copy_file(copytempfile);
        currentDir->filePtr->nextFile = pTemp;
        currentDir->size += copytempfile->size;
        cout << "文件粘贴成功！" << endl;
        return true;
    }
    return false;
}

bool OneFileSystem::edFile() {
    char n[MAX_NAME_LENGTH];
    char* input =NULL;
    input = (char *)malloc(100 * sizeof(char));
    cout<<"请输入需要存储的内容"<<endl;
    cin >> n;
    OneFile* file = currentDir->filePtr;
    while (file != nullptr) {
        if (!strcmp(file->name, n)) {
            scanf("%[^\t]", input);
            file->content = input;
            file->size = strlen(input)-1;
            disk_size -= file->size;
            OneDir* directory = currentDir;
            while (directory != nullptr)//修改编辑文件后各级目录的大小
            {
                directory->size += file->size;
                directory = directory->preDir;
            }
            cout << "编辑已存储!" << endl;
            size += file->size;
            return true;
        }
        file = file->nextFile;
    }

    cout << "文件未找到！                -出错" << endl;
    return false;
}

bool OneFileSystem::show_path(OneDir* directory)//显示当前路径的函数
{
    if (directory->preDir == nullptr)
        cout<<rang::fg::green << root->name<<rang::style::reset;
    if (directory->preDir != nullptr) {
        this->show_path(directory->preDir);//递归调用此函数
        cout <<rang::fg::green<< directory->name<<rang::style::reset;
    }
    cout << "/";
    return true;
}

bool OneFileSystem::showCurrentDir() {
    OneDir* directory = currentDir->dirPtr;
    OneFile* file = currentDir->filePtr;
    if (directory == nullptr && file == nullptr) {
        cout << "当前目录为空" << endl;
        return false;
    }
    cout << "当前目录下存在：" << endl;

    if (directory != nullptr) {
        cout << "\t" << "目录名" << "\t\t" << "目录大小" << endl;
        while (directory != nullptr) {
            cout << "\t" << directory->name << "\t\t" << directory->size << endl;
            directory = directory->nextDir;
        }
        cout << endl;
    }

    if (file != nullptr) {
        cout << "\t" << "文件名" << "\t\t" << "文件大小" << "\t" << endl;
        while (file != nullptr) {
            cout << "\t" << file->name << "\t\t" << file->size << "\t" << endl;
            file = file->nextFile;
        }
    }
    return true;
}

bool OneFileSystem::showPath()//显示当前路径
{
    show_path(currentDir);
    return true;
}

bool OneFileSystem::goback() {
    if (currentDir->preDir == nullptr) {
        cout << "当前为root目录！" << endl;
        return false;
    }
    currentDir = currentDir->preDir;
    return true;
}

bool OneFileSystem::setUser(char* n, char* c) {
    OneDir* root = new OneDir;
    strcpy(root->name, n);
    strcpy(name, n);
    strcpy(password, c);

    this->root = root;
    currentDir = root;
    currentDir->preDir = nullptr;
    currentDir->dirPtr = nullptr;
    currentDir->filePtr = nullptr;
    currentDir->nextDir = nullptr;
    return true;
}

int OneFileSystem::getSize() {
    return size;
}

int OneFileSystem::getRemain() {
    return disk_size;
}

OneDir* OneFileSystem::getCurrentdir() {
    return currentDir;
}

bool OneFileSystem::search() {
    char n[MAX_NAME_LENGTH];
    cin >> n;

    OneDir* directory = currentDir->dirPtr;
    OneFile* file = currentDir->filePtr;

    while(currentDir->preDir != nullptr){
        goback();
    }

    search_file(n);
    search_dir(n);
    return true;
}

bool OneFileSystem::search_file(char n[MAX_NAME_LENGTH]) {

    OneDir* directory = currentDir->dirPtr;
    OneFile* file = currentDir->filePtr;
    OneDir* tempDir;
    tempDir = currentDir->dirPtr;

    if (directory == nullptr && file == nullptr)
        return false;


    if (file != nullptr) {
        while (file != nullptr) {
            if (strcmp(file->name, n) == 0) {
                cout << "找到该文件，目录在：" ;
                show_path(currentDir);
                cout<< endl;
                break;
            }
            else {
                file = file->nextFile;
            }
        }
    }

    if (directory != nullptr) {
        while (tempDir != nullptr) {
            currentDir = tempDir;
            search_file(n);
            currentDir = currentDir->preDir;
            tempDir = tempDir->nextDir;
        }
    }

    return true;
}

bool OneFileSystem::search_dir(char n[MAX_NAME_LENGTH]) {

    OneDir* directory = currentDir->dirPtr;
    OneDir* tempDir = currentDir->dirPtr;

    if (directory != nullptr) {
        while (tempDir != nullptr) {

            if (strcmp(tempDir->name, n) == 0) {
                cout << "找到该目录，目录在：" ;
                show_path(currentDir);
                cout<< endl;
            }
            currentDir = tempDir;
            search_dir(n);
            currentDir = currentDir->preDir;
            tempDir = tempDir->nextDir;
        }
    }
    return true;
}

bool OneFileSystem::gobacktoRoot(){
    while (currentDir->preDir !=nullptr)
        currentDir = currentDir->preDir;
    return true;
}