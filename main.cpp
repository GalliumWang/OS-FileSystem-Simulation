#include "operate.h"
//#include <unistd.h>
#include <stdio.h>
#include  <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "rang.hpp"
using namespace std;

/*用户注册写入文件*/
int regist(char username[30], char password[30]) {

    int returnStatus=1;
    FILE* userfile;//文件指针
    if ((userfile = fopen("users.ini", "a+")) == NULL) {
        printf("用户验证文件无法打开\n");
        returnStatus=0;
    }else {
        fprintf(userfile, "%s %s\n", username, password);
    }
    
    fclose(userfile);
    return returnStatus;
}

/*用户名密码检验是否已经注册*/
int login(char username[30], char password[30]) {
    char user[30],pass[30];
    FILE* userfile;//文件指针
    if ((userfile = fopen("users.ini", "r")) == NULL) {
        printf("信息文件损坏\n");
        fclose(userfile);
        return 0;
    }
    else {
        while (!feof(userfile)) {
            fscanf(userfile, "%s%s", user, pass);
            if ((strcmp(username, user) == 0) && (strcmp(password, pass) == 0)) {
                fclose(userfile);
                return 1;
            }
        }
    }
    fclose(userfile);
    return 0;
}

void menu() {
    cout<<endl;
    cout << "                                  文件操作                                   " << endl;
    cout << "* 1.mkfile + <string>                     新建一个名为<string>的文件          *" << endl;
    cout << "* 2.rmfile + <string>                     删除文件名为<string>的文件          *" << endl;
    cout << "* 3.refile + <string> + <string>          重命名文件为第二个string            *" << endl;
    cout << "* 4.cpfile + <string>                     复制文件,文件名为<string>           *" << endl;
    cout << "* 5.ptfile                                粘贴文件到当前目录                  *" << endl;
    cout << "* 6.opfile  + <string>                    打开文件,文件名为<string>           *" << endl;
    cout << "* 7.edfile + <string>                     编辑文件,文件名为<string>           *" << endl;
    cout << "                                  目录操作                                   " << endl;
    cout << "* 8.mkdir + <string>                      新建一个名为<string>的目录          *" << endl;
    cout << "* 9.rmdir + <string>                      删除目录,目录名为<string>           *" << endl;
    cout << "* 10.redir + <string> + <string>          重命名目录为第二个string            *" << endl;
    cout << "* 11.cpdir + <string>                     复制目录,目录名为<string>           *" << endl;
    cout << "* 12.ptdir                                粘贴目录                            *" << endl;
    cout << "* 13.list                                 显示目录下所有内容                  *" << endl;
    cout << "                                  其他操作                                   " << endl;
    cout << "* 14.cd. + <string>                       打开子目录                          *" << endl;
    cout << "* 15.cd..                                 返回上一级目录                      *" << endl;
    cout << "* 16.about                                关于本机                            *" << endl;
    cout << "* 17.search + <string>                    检索文件/目录                       *" << endl;
    cout << "* 18.clear                                清屏                                *" << endl;
    cout << "* 19.exit                                 注销                                *" << endl;
    cout << "* 20.root                                 回到根目录                          *" << endl;
    cout << endl;
}


int fsOperate(char name[30], char pass[30]) {
    OneFileSystem oneFS;
    oneFS.setUser(name, pass);
    while (true) {
        system("clear");
        menu();
        while (true) {
            cout << endl;
            string choice;
            oneFS.showPath();
            cin >> choice;
            if (choice == "mkdir")
                oneFS.mkDir();
            else if (choice == "mkfile")
                oneFS.mkFile();
            else if (choice == "rmdir")
                oneFS.deleteDir();
            else if (choice == "rmfile")
                oneFS.deleteFile();
            else if (choice == "redir")
                oneFS.reDir();
            else if (choice == "refile")
                oneFS.reFile();
            else if (choice == "cd")
                oneFS.gotoDir();
            else if (choice == "root")
                oneFS.gobacktoRoot();
            else if (choice == "opfile")
                oneFS.readFile();
            else if (choice == "list")
                oneFS.showCurrentDir();
            else if (choice == "cpdir")
                oneFS.cpDir();
            else if (choice == "cpfile")
                oneFS.cpFile();
            else if (choice == "ptdir")
                oneFS.ptDir();
            else if (choice == "ptfile") {
                oneFS.ptFile();
            }
            else if (choice == "search") {
                oneFS.search();
            }
            else if (choice == "edfile")
                oneFS.edFile();
            else if (choice == "cd..")
                oneFS.goback();
            else if (choice == "clear") {
                system("clear");
                menu();
            }
            else if (choice == "exit") {
                system("clear");
                cout << "用户: " << name << "完成注销...."<< endl;
                return 0;
            }
            else if (choice == "menu") {
                menu();
            }
            else if (choice == "about") {
                cout<<"本机剩余空间为："<<oneFS.getRemain()<<endl;
            }
            else if (choice == "mkdir?")
                cout << "创建新目录 ，格式为 mkdir [dirname]" << endl;
            else if (choice == "mkfile?")
                cout << "创建新文件，格式为 mkfile[filename]" << endl;
            else if (choice == "rmdir?")
                cout << "删除子目录，格式为 rmdir [dirname]" << endl;
            else if (choice == "rmfile?")
                cout << "删除文件，输入格式为 rmfile [filename]" << endl;
            else if (choice == "redir?")
                cout << "目录重命名，输入格式为 rmdir [dirname] [dirname]" << endl;
            else if (choice == "refile?")
                cout << "文件重命名,输入格式为 refile [filename] [filename]" << endl;
            else if (choice == "cd?")
                cout << "打开子目录，格式为 cd [dirname]" << endl;
            else if (choice == "opfile?")
                cout << "查看文件内容，格式为 opfile [filename]" << endl;
            else if (choice == "list?")
                cout << "显示目录内容列表" << endl;
            else if (choice == "cpdir?")
                cout << "复制子目录, 格式为 cpdir [dirname]" << endl;
            else if (choice == "cpfile?")
                cout << "复制文件,格式为 cpfile [filename]" << endl;
            else if (choice == "about?")
                cout << "显示剩余空间" << endl;
            else if (choice == "ptdir?")
                cout << "粘贴已复制的目录" << endl;
            else if (choice == "ptfile?")
                cout << "粘贴已复制的文件" << endl;
            else if (choice == "clear?")
                cout << "清屏" << endl;
            else if (choice == "edFile?")
                cout << "编辑文档，输入格式为 edFile[filename]" << endl;
            else if (choice == "cd..?")
                cout << "返回当前目录的上一级目录" << endl;
            else if (choice == "exit?")
                cout << "注销文件系统" << endl;
            else
                cout << "命令无效请重新输入" << endl;
        }
    }

}
int main() {
    int choice = 0; //choose what function to use
    char name[30], pass[30], repass[30];    //user info[]
    system("clear");
    while (true) {
        cout<<endl;
        cout << "\t\t*          欢迎访问OneFileSystem              *" << endl;
        cout << "\t\t*               1. 注册用户                   *" << endl;
        cout << "\t\t*               2. 登录系统                   *" << endl;
        cout << "\t\t*               3. 命令帮助                   *" << endl;
        cout << "\t\t*               4. 退出系统                   *" << endl;
        cout << endl;
        cout << "\t\t请输入相应指令(1、2、3、4): ";
        cin >> choice;
        switch (choice) {
            /*注册*/
            case 1: {

                //TODO:del this block
                // cout << "\t\t请输入用户名:";
                // cin >> name;
                // cout << "\t\t请输入密码: ";
                // cin >> pass;
                // cout << "\t\t请重复输入密码: ";
                // cin >> repass;

                bool reEnter=false;

                while (strcmp(pass, repass)!= 0) {
                    if(reEnter){
                    cout << "\t\t两次密码不一致，请重试" << endl;
                    }

                    cout << "\t\t请输入密码: ";
                    cin >> pass;
                    cout << "\t\t请确认密码: ";
                    cin >> repass;

                    reEnter=true;
                }

                if (regist(name, pass) == 1) {
                    cout << "\t\t注册成功.." << endl;
                    sleep(2);
                    system("clear");
                }
                else {
                    cout << "\t\t注册失败，请重试" << endl;
                    sleep(2);
                    system("clear");
                }
            }
                break;
                /*登录*/
            case 2: {
                cout << "\t\t请输入用户名:";
                cin >> name;
                cout << "\t\t请输入密码:";
                cin >> pass;
                if (login(name, pass) == 1) {
                    cout << "\t\t登录成功.." << endl;
                    fsOperate(name, pass);
                    system("clear");
                }
                else {
                    cout << "\t\t登录失败，请检查用户名和密码.." << endl;
                    sleep(2);
                    system("clear");
                }}
                break;
            case 3: {
                system("clear");
                cout << endl;
                cout << "\t\t*                        帮助                            *" << endl;
                cout << "\t\t*             输入help显示所有命令的使用说明            *" << endl;
                cout << "\t\t*            输入某个命令+?显示该条命令的使用说明        *" << endl;
                cout << endl;
                sleep(3);
            }
                break;

            case 4: {
                cout << "\t\t退出文件系统" << endl;
                exit(0);
            }
                break;
            default:
                break;
        }

        //FIXME
        std::cin.clear();
        std::cin.ignore(22,'\n');
        choice=0;
    }
    return 0;
}
