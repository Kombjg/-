#ifndef GAME_H
#define GAME_H

#include <QLabel>
#include <QMainWindow>
#include "QDebug"
using namespace std;
//the skill is a int
//use hash map to store the location[two-dimensional array to store the sotiation of soldier]
//as the mouse was clicked, then use the coordinate to get the data in hash map
struct Character{

    int id;

    QString name;

    int damage_value;

    int speed;

    int lives;

    int cooldown_initial;

    int skill;

    int cooldown_current;

    int attack_range;


};

namespace Ui {
class game;
}

class game : public QMainWindow
{
    Q_OBJECT

public:


    explicit game(QWidget *parent = nullptr);

    ~game();

    void paintEvent(QPaintEvent *) override;

    void getCard();

    void setDisabledForbutton();

    void setEnabledForbutton();


    void attackBoss(short damage);
private slots:

    void on_end_clicked();

    void on_getCharacter_clicked();

    void on_moveTheCharacter_clicked();

    void on_attack_clicked();

    void on_useSkill_clicked();

    void on_finish_clicked();

private:

    Ui::game *ui;

    void getData();

    vector<Character> characters;

    QPixmap star;  // onload star

    char space;

    int numsofcharacter;

    QPoint clickPos;

    short x_mouse;

    short y_mouse;

    Character charactersInBattleFiled[8][5];//如果没有的话是-1   //先列后行

    void iniCharacterInBattleFiled();

    short x_objcard_position;

    short y_objcard_position;

    bool mouseEventSuccessOrNot;



    //get CARD randomly
    short theCardJustGet;

    bool gettingCard;

    short myMaxCard;

    short BossMaxCard;

    short myhaveGotCardnums;

    short bosshaveGotCardnums;



    //Result



    void victory();

    void defeat();

    //HP

    short my_lives;
    short boss_lives;

    //attack

    short characterDieOrNot(Character & character);

    vector<QMovie*> skillImages;

    void IamBeingAttack(short damage);

    short base_damage;

    short getAline();

    short getAcolumn();

    short getAnumberofthree();

    short isRobot;

    short numsOfBossSkill;

    void use_boss_skill();

    short idOfBossSkill;

    void robotSkillRandomly();

    short randomBossSkillnumber;

    //be used

    void activeDebug(Character & character);

    bool isCoolDown;

    void hasBeenUsed();

    bool justGetCard;

    short maxDisatance;

    void coolDownAll();//用完技能重置为original+1,每回合除了抽卡都全部减去1

    QString prompt;

    vector<QPixmap> images;

    QString image_path;

    //short optionIDofMission;//0:have none, 1:

    //onload the array of label which show the images
    QLabel battlefieldWithImages[8][5];

protected:

    void mousePressEvent(QMouseEvent* event) override;

};



#endif // GAME_H
