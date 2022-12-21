#include<iostream>
#include<fstream>
using namespace std;
#define USERNUM 10
#define MAX_OPEN 5
typedef struct file
{
    string file_name;
    bool file_protect[3];
    bool open_file_protect[3]; //仅在文件打开时有效
    int  read, write; //定义为读写指针
    int  file_length;
    struct file* next;
    file() {
        file_name = "";
        file_protect[0] = 0;
        file_protect[1] = 0;
        file_protect[2] = 0;
        open_file_protect[0] = 0;
        open_file_protect[1] = 0;
        open_file_protect[2] = 0;
        read = write = 0;
        file_length = 0;
        next = NULL;
    }
} File;

typedef struct afd {
    int length;
    File* file_head;
    afd() {
        length = 0;
        file_head = new File();
    }
}AFD;

typedef struct mfd{
    string user_name;
    File* file_head;
    AFD afd;
    mfd() {
        user_name = "";
        file_head = new File();
        afd = AFD();
    }
}MFD[USERNUM];

void init(MFD mfd) {
    fstream file;
    file.open("user_name.txt");
    for (int i = 0; i < USERNUM; i++){
        mfd[i] = struct mfd();
        file >> mfd[i].user_name;
    }
    file.close();
}

int findUserName(string user_name,MFD mfd) {
    bool is_find = false;
    for (int i = 0; i < USERNUM; i++) {
        if (mfd[i].user_name == user_name) {
            is_find = true;
            return i;
        }
    }
    if (!is_find) {
        string user;
        cout << "未找到用户，请重新输入：";
        cin >> user;
        return findUserName(user, mfd);
    }
}

bool findFileName(string file_name, struct mfd mfdsg) {
    for (File* p = mfdsg.file_head; p->next != NULL; p = p->next) {
        if (p->next->file_name == file_name)return true;
    }
    return false;
}
void Create(struct mfd mfdsg) {
    File *file=new File();
    cout << mfdsg.user_name << ">>" << "新建文件名：";
    cin >> file->file_name;
    if (findFileName(file->file_name, mfdsg)) {
        cout << endl << "!!!文件重名，请重命!!!" << endl << endl;
        Create(mfdsg);
        return;
    }
    cout << mfdsg.user_name << ">>" << "新建文件保护码(示例:1 0 1)：";
    cin >> file->file_protect[0]>>file->file_protect[1]>>file->file_protect[2];
    cout << mfdsg.user_name << ">>" << "新建文件长度：";
    cin >> file->file_length;
    file->read = file->write = 0;
    file->next = NULL;

    File* p = mfdsg.file_head;
    for (; p->next != NULL; p = p->next);
    file->next = p->next;
    p->next = file;
    
}
void Delete(struct mfd mfdsg) {
    string file_name;
    cout << mfdsg.user_name << ">>"<< "请输入欲删除的文件名：";
    cin >> file_name;
    for (File* p = mfdsg.file_head; p->next!= NULL; p = p->next) {
        if (p->next->file_name == file_name) {
            p->next = p->next->next;
            free(p->next);
            return;
        }
    }
    cout << endl << "!!!未找到文件!!!" << endl << endl;
}
void Open(struct mfd mfdsg) {
    if (mfdsg.afd.length >= MAX_OPEN) {
        cout << endl << "!!!打开文件数已达上限!!!" << endl << endl;
        return;
    };
    File* file=new File();
    string file_name;
    bool open_protect[3];
    cout << mfdsg.user_name << ">>" << "请输入欲打开的文件名：";
    cin >> file_name;
    for (File* p = mfdsg.file_head->next; p != NULL; p = p->next) {
        if (p->file_name == file_name) {
            file = p; break;
        }
    }
    if (file==NULL) {
        cout << endl << "!!!未找到文件!!!" << endl << endl; return;
    }
    cout << mfdsg.user_name << ">>" << "请输入保护码(示例:1 0 1)：";
    cin >> open_protect[0] >> open_protect[1] >> open_protect[2];

    File* xfile=new File();
    *xfile = *file;
    if (xfile->file_protect[0] == 0 || open_protect[0] == 0) {
        cout << endl << "!!!没有读取权限!!!" << endl << endl;return;
    }
    else {
        xfile->open_file_protect[0] = 1;
    }
    if (xfile->file_protect[1] < open_protect[1])cout << endl << "!!!没有写入权限!!!" << endl << endl;
    if (xfile->file_protect[2] < open_protect[2])cout << endl << "!!!没有执行权限!!!" << endl << endl;
    xfile->open_file_protect[1] = min(xfile->file_protect[1], open_protect[1]);
    xfile->open_file_protect[2] = min(xfile->file_protect[2], open_protect[2]);

    File* p = mfdsg.afd.file_head;
    for (; p->next != NULL; p = p->next);
    xfile->next = p->next;
    p->next = xfile;
}
void Close(struct mfd mfdsg) {
    string file_name;
    cout << mfdsg.user_name << ">>" << "请输入欲关闭的文件名：";
    cin >> file_name;
    for (File* p = mfdsg.afd.file_head; p->next != NULL; p = p->next) {
        if (p->next->file_name == file_name) {
            p->next = p->next->next;
            free(p->next);
            return;
        }
    }
    cout << endl << "!!!未在打开文件中找到文件!!!"<<endl << endl;
}
void Read(struct mfd mfdsg) {
    string file_name;
    cout << mfdsg.user_name << ">>" << "请输入欲读取的文件名：";
    cin >> file_name;
    for (File* p = mfdsg.afd.file_head; p->next != NULL; p = p->next) {
        if (p->next->file_name == file_name) {
            if (p->next->open_file_protect[0]) {
                p->next->read++;
                return;
            }
            else {
                cout << endl << "!!!无权限!!!" << endl << endl;
                return;
            }
        }
    }
    cout << endl << "!!!未在打开文件中找到文件!!!" << endl << endl;
}
void Write(struct mfd mfdsg) {
    string file_name;
    cout << mfdsg.user_name << ">>" << "请输入欲写入的文件名：";
    cin >> file_name;
    for (File* p = mfdsg.afd.file_head; p->next != NULL; p = p->next) {
        if (p->next->file_name == file_name) {
            if (p->next->open_file_protect[1]) {
                p->next->write++;
                return;
            }
            else {
                cout <<endl<< "!!!无权限!!!" << endl<<endl;
                return;
            }
        }
    }
    cout << endl << "!!!未在打开文件中找到文件!!!" << endl << endl;
}

void reviseFileName(struct mfd mfdsg) {
    string file_name;
    cout << mfdsg.user_name << ">>" << "请输入欲修改的文件名：";
    cin >> file_name;
    for (File* p = mfdsg.file_head; p->next != NULL; p = p->next) {
        if (p->next->file_name == file_name) {
            cout<< mfdsg.user_name << ">>" << "请输入新的文件名：";
            cin >> p->next->file_name;
            return;
        }
    }
    cout << endl << "!!!未找到文件!!!" << endl << endl;
}
void reviseFilePermission(struct mfd mfdsg) {
    string file_name;
    cout << mfdsg.user_name << ">>" << "请输入欲修改的文件名：";
    cin >> file_name;
    for (File* p = mfdsg.file_head; p->next != NULL; p = p->next) {
        if (p->next->file_name == file_name) {
            cout << mfdsg.user_name << ">>" << "请输入新的保护码(示例:1 0 1)：" ;
            cin >> p->next->file_protect[0] >> p->next->file_protect[1] >> p->next->file_protect[2];
            return;
        }
    }
    cout << endl << "!!!未找到文件!!!" << endl << endl;
}

void Display(struct mfd mfdsg) {
    cout << "-------------------"<<mfdsg.user_name << "的文件-------------------" << endl;
    for (File* p = mfdsg.file_head; p->next != NULL; p = p->next) {
        cout << "-" << p->next->file_name << endl;
    }
    cout << "-----------------"<<mfdsg.user_name << "的打开文件-----------------" << endl;
    for (File* p = mfdsg.afd.file_head; p->next != NULL; p = p->next) {
        cout << "-" << p->next->file_name << endl;
    }
    cout << "----------------------------------------------" << endl;
}

void executeOperation(struct mfd mfdsg) {
    system("cls");
    int command;
    cout << "****************** 请选择操作执行**********************" << endl;
    cout << "*    (1)创建文件      (2)删除文件      (3)打开文件    *" << endl;
    cout << "*    (4)关闭文件      (5)读取文件      (6)写入文件    *" << endl;
    cout << "*    (7)修改文件名    (8)修改文件权限  (0)退出程序    *" << endl;
    cout << "*******************************************************" << endl;
    cout << mfdsg.user_name << ">>";
    cin >> command;

    switch (command) {
    case 1:Create(mfdsg);  break;
    case 2:Delete(mfdsg); break;
    case 3:Open(mfdsg);  break;
    case 4:Close(mfdsg);  break;
    case 5:Read(mfdsg); break;
    case 6:Write(mfdsg); break;
    case 7:reviseFileName(mfdsg); break;
    case 8:reviseFilePermission(mfdsg); break;
    case 0:cout << "程序退出"; exit(0); break;
    default:cout << "!!!输入错误，请重新输入!!!" << endl;
        executeOperation(mfdsg); return; break;
    }
    Display(mfdsg);
    system("pause");
    executeOperation(mfdsg);
}

int main() {
    MFD mfd;
    init(mfd);
    string user_name;
    cout << "****************************************" << endl;
    cout << "*      01    02    03    04    05      *" << endl;
    cout << "*      06    07    08    09    00      *" << endl;
    cout << "****************************************" << endl;
    cout << "请输入用户名进行登录：";
    cin >> user_name;
    int index = findUserName(user_name, mfd);
    executeOperation(mfd[index]);


    return 0;
}