#include <iostream>
#include <cstring>
#define Max 5
using namespace std;

typedef struct
{
    int segment_addr;
    int segment_last_addr;
} Box;

struct SegTable
{
    Box data[Max];
}seg_table;

int page_block[5] = { 2,3,6,8,9 };
int seg_address[5] = { 2,12,30,45,73 };
bool page_state[10];
int address;
int seg_table_register[2];
void segTable()
{
    seg_table_register[0] = seg_address[0];
    seg_table_register[1] = Max;
    memset(page_state, false, sizeof(page_state));
    for (int i = 0; i < 5; i++)
    {
        seg_table.data[i].segment_addr = seg_address[i];
        seg_table.data[i].segment_last_addr = page_block[4] + seg_address[i] - seg_table_register[0];
        page_state[page_block[i]] = true;
    }
}

void Display() {
    cout << "*****************************段表*****************************"<<endl;
    cout << "*    段号" << "     *    状态" << "    *    页表大小" << "    *    页表始址    *" << endl;
    for (int i = 0; i < 5; i++) {
        if(i==0)cout <<"*     "<< i << "       *      " << 1 << "     *        " << 1 << "       *       " << seg_address[i]<<"        *" << endl;
        else cout <<"*     "<< i << "       *      " << 1 << "     *        " << 1 << "       *       " << seg_address[i]<<"       *" << endl;
    }
    cout << "*****************************页表*****************************" << endl;
    cout << "*         页号" << "        *      状态" << "        *      存储块       *" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "*          " << i << "          *        "<< 1 << "         *         " << page_block[i] << "         *" << endl;
    }
    cout << "**************************************************************" << endl;
}
void addressTranslation()
{
    segTable();
    cout << "请输入地址：";
    cin >> address;
    for (int i = 0; i < Max; i++){
        if ((address >= seg_table.data[i].segment_addr) && (address <= seg_table.data[i].segment_last_addr){
            int index = 0;
            for (int j = 0; j < 5; j++){
                if (seg_table.data[i].segment_addr + page_block[j] - seg_table_register[0] == address
                    && page_state[page_block[j]] == true){
                    cout <<"--"<< "地址的转换为：";
                    cout << "段序号：" << i << "    段的初始地址："
                        << seg_address[i] << "    页序号：" << j
                        << "    块序号：" << page_block[j] << endl;
                    page_state[page_block[j]] = false;
                    index = 1;
                    i = Max;
                }
            }
            if (index == 0){
                cout << "!!!在页表里找不到地址!!!" <<  endl;
                i = Max;
            }
        }
        else if (address < seg_table_register[0]){
            cout << "!!!地址" << address<< " 过小!!!" << endl;
            i = Max;
        }
        else if (address > seg_table.data[Max - 1].segment_last_addr){
            cout << "!!!地址" << address << "过大!!!" << endl;
            i = Max;
        }
        else{
            if (i == Max - 1)
                cout << "!!!在段表里找不到地址!!!" << address<<  endl;
        }
    }
    addressTranslation();
}

int main()
{
    Display();
    addressTranslation();
    return 0;
}
