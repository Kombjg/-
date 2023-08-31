#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include"QDebug"
#include"iostream"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

struct character{
    QString name;
    int damage_value;
    int speed;
    int lives;
    int cooldown_initial;
    int skill;
    int cooldown_current;
};

const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    std::cout << GREEN << "Data " << RESET;
    std::cout << RED << "Modification " << RESET;
    std::cout << BLUE << "Program" << RESET << std::endl;

    int numColors = 4;
        std::string colors[] = {RED, GREEN, YELLOW, BLUE};

    for (int i = 0; i < 2; ++i) {
         std::cout << colors[i % numColors] << "Loading";
         for (int j = 0; j < 3; ++j) {
             std::cout << ".";
             std::this_thread::sleep_for(std::chrono::milliseconds(300));
         }
         std::cout << RESET << "\r"; // 移动光标回到行首并重置颜色
         std::this_thread::sleep_for(std::chrono::milliseconds(500));
      }
        system("cls");
        std::cout << GREEN << "Data " << RESET;
        std::cout << RED << "Modification " << RESET;
        std::cout << BLUE << "Program" << RESET << std::endl;;
        std::cout << "Done!" << std::endl;


    QString Qdata;
    string data;
    int numdata;
    int number;
    cout << RED <<"In this program you can add characters according to the instructions"<<endl;
    cout <<endl;
    cout<<YELLOW<<"Enter the number of characters to add"<<endl;
    cin >> number;
    cout<<endl<<endl;
    QFile file("DATA.FLL"); // 打开文件
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file); // 创建文本输出流
        for(int i =0;i<number;i++){
            cout << RESET <<"input the character's name"<<BLUE<<"  AND  "<<RED<<"It is recommended to only enter English"<<endl<<RESET;
            cin >> data;
            Qdata = QString::fromStdString(data);
            out << Qdata; // 将 QString 写入文件
            out << '\n';
            cout << "Input the damage_value"<<endl;
            cout << RED <<"Warning:" << BLUE<<" The following data can only be integers"<<endl<<RESET;
            cin >> numdata;
            out << numdata;
            out << ' ';
            cout << "Input the speed"<<endl;
            cin >> numdata;
            out << numdata;
            out << ' ';
            cout << "Input the lives"<<endl;
            cin >> numdata;
            out << numdata;
            out << ' ';
            cout << "Input the cooldown_initial"<<endl;
            cin >> numdata;
            out << numdata;
            out << ' ';
            cout << "Input the skill(the number of it)"<<endl;
            cin >> numdata;
            out << numdata;
            out << ' ';
            cout << "Input the attack_range"<<endl;
            cin >> numdata;
            out << numdata;
            out << ' ';
        }

        file.close(); // 关闭文件
        qDebug() << "Data saved successfully.";
    }
    else {
         qDebug() << "Failed to open file for writing. Error: " << file.errorString();
    }
    return a.exec();
}
