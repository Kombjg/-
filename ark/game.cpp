#include "game.h"
#include "ui_game.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <qfile.h>
#include <qtextstream.h>
#include <QRandomGenerator>
#include <QMovie>
#include <QThread>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <qtimer.h>
#include <QGraphicsView>
//      nums  Proportion  pixmap
//        8 * 6 * 35  =  1680
//5 * 6 * 35  =  1050

//*-------22 51 55 SOC 04-73---------*//
//WARNING the x,y is not as same between coordinate and array


game::game(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::game)
{



    QIcon icon(":/head.jpg"); // 请将图标路径替换为你的图标路径
    setWindowIcon(icon);
    setWindowTitle("方舟的骑士");

    setFixedSize(1920,1200);

    getData();

    numsofcharacter = characters.size();

        ui->setupUi(this);


//    ui->setupUi(this);


}

void game::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    for(int i = 0; i <= 8; i++){
        painter.drawLine(210 * i, 0, 210 * i, 1050);
    }

    for(int i = 0; i <= 5; i++){  //draw lines
        painter.drawLine(0, 210 * i, 1680, 210 * i);
    }

}

void game::setDisabledForbutton()
{
    ui->getCharacter->setDisabled(true);
    ui->moveTheCharacter->setDisabled(true);
    ui->attack->setDisabled(true);
    ui->useSkill->setDisabled(true);
    ui->finish->setDisabled(true);
}

void game::setEnabledForbutton()
{
    ui->getCharacter->setDisabled(false);
    ui->moveTheCharacter->setDisabled(false);
    ui->attack->setDisabled(false);
    ui->useSkill->setDisabled(false);
}

game::~game()
{
    delete ui;
}

void game::on_end_clicked()
{
    QCoreApplication::exit();
}

void game::getData()
{
    //onload the skill movie
//    for (int i = 0; i < 4; ++i) {
//        QString gifPath = QString("path_to_gif/BS%1.gif").arg(i);
//        QMovie movie(gifPath);
//        skillImages.push_back*movie);
//    }
    for (int i = 0; i < 4; ++i) {
        QString gifPath = QString(":/BS0%1.gif").arg(i);
        qDebug() << gifPath;
        QMovie* movie = new QMovie(gifPath);
        skillImages.push_back(movie);
    }


    QPixmap star(QString(":/star.png"));


    numsOfBossSkill = 4;

    justGetCard = false;

    isCoolDown = false;

    gettingCard = false;

    myhaveGotCardnums = 0;

    bosshaveGotCardnums = 0;

    //read in OPTION
    QFile fileoption(":/OPTION.FLL");
    if (fileoption.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fileoption);

        in >> myMaxCard;
        in >> BossMaxCard;
        in >> my_lives;
        in >> boss_lives;
        in >> maxDisatance;
        in >> isRobot;
        in >> base_damage;

       fileoption.close();
       qDebug() << "Option read successfully.";

       qDebug() << "myMaxCard:" << myMaxCard;
       qDebug() << "BossMaxCard:" << BossMaxCard;
       qDebug() << "my_lives:" << my_lives;
       qDebug() << "boss_lives:" << boss_lives;
       if(isRobot){
           qDebug() << "is Robot";
       }
       else{
           qDebug() << "isn't Robot";
       }
    }


    //onload character Data
    QFile file(":/DATA.FLL");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        int count = 0;
        while (!in.atEnd()) {
            Character character;

            // read character's name
            character.name = in.readLine();
            image_path = ":/img/" + character.name + ".png";


            //onload the image of character
            QPixmap pixmap(image_path);
            images.push_back(pixmap);

            // read damage_value, speed, lives, cooldown_initial, and skill
            in >> character.damage_value >> character.speed
                >> character.lives >> character.cooldown_initial >> character.skill>>character.attack_range;
            character.id = count;
            character.cooldown_current = character.cooldown_initial;
            count++;
            characters.push_back(character);

           in >> space;//skip the last spaces
       }
       file.close();
       qDebug() << "Data read successfully.";

       // debug
       for (const Character &character : characters) {
           qDebug() << "ID:"   << character.id
                    << "Name:" << character.name
                    << "Damage Value:" << character.damage_value
                    << "Speed:" << character.speed
                    << "Lives:" << character.lives
                    << "Cooldown Initial:" << character.cooldown_initial
                    << "Skill:" << character.skill
                    << "Attack_range"<<character.attack_range;
        }
    }
    else {
       qDebug() << "Failed to open file for reading. Error:" << file.errorString();
       QMessageBox::critical(nullptr, "Error", "Missing core file, game is about to close.");
       QCoreApplication::exit();
   }

    //onload the player's HP

//    ui->mylives->setValue(my_lives);

//    ui->bosslives->setValue(boss_lives);

    //onload the battlefield
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 5; j++){
            battlefieldWithImages[i][j].setParent(this);
            battlefieldWithImages[i][j].move(i*210,j*210-135);
            battlefieldWithImages[i][j].show();
        }
    }

    for(int i = 0; i<8; i++){
        for(int j = 0; j<5; j++){
            charactersInBattleFiled[i][j].id = -1;
        }
    }


}

void game::iniCharacterInBattleFiled()
{
    charactersInBattleFiled[x_objcard_position][y_objcard_position]= characters[theCardJustGet];
}

void game::victory()
{
    QPixmap victory_image(":/victory.jpg");
    ui->result->setPixmap(victory_image.scaled(QSize(800,600)));
    setDisabledForbutton();
}

void game::defeat()
{

    QPixmap defeat_image(":/defeat.jpg");
    ui->result->setPixmap(defeat_image.scaled(QSize(800,600)));
    setDisabledForbutton();
}

short game::characterDieOrNot(Character &character)
{
    if(character.lives <= 0){
        character.id = -1;
        return 1;
    }
    return 0;
}

void game::IamBeingAttack(short damage)
{

    my_lives -= damage;
    ui->mylives->setValue(my_lives);
    if(my_lives < 0){
        defeat();
    }
}

void game::attackBoss(short damage)
{

    boss_lives -= damage;
    ui->bosslives->setValue(boss_lives);
    if(boss_lives < 0){
        victory();
    }
}


short game::getAline()
{
    return QRandomGenerator::global()->bounded(0, 5);
}

short game::getAcolumn()
{
    return QRandomGenerator::global()->bounded(0, 8);
}

short game::getAnumberofthree()
{
    short number = QRandomGenerator::global()->bounded(0, 10);
    if(0 < number && number <2){
        return 0;
    }
    else if(number >= 8){
        return 2;
    }
    else{
        return 1;
    }
}

void game::use_boss_skill()
{
    robotSkillRandomly();
    short line;
    short column;
    short damage = base_damage + getAnumberofthree() + idOfBossSkill;
    //-----------------------------------
    //idOfBossSkill = 0;
    //----------------------------------

    if (isRobot) {
        if (idOfBossSkill == 0) {
            damage = damage + 0;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 5; j++) {
                    Character character = charactersInBattleFiled[i][j];
                    if (character.id != -1) {
                        charactersInBattleFiled[i][j].lives -= damage;
                        // 特效
                        battlefieldWithImages[i][j].clear(); // 取消显示图片
                        battlefieldWithImages[i][j].setMovie(skillImages[idOfBossSkill]); // 需要的是地址
                        skillImages[idOfBossSkill]->start();

                        QTimer::singleShot(1500, this, [this, i, j, character]() {
                            skillImages[idOfBossSkill]->stop();
                            battlefieldWithImages[i][j].clear(); // 取消显示 GIF
                            battlefieldWithImages[i][j].setPixmap(images[character.id].scaled(QSize(210, 210))); // 设置图片
                            if (characterDieOrNot(charactersInBattleFiled[i][j])) {
                                battlefieldWithImages[i][j].clear(); // 取消显示图片
                            }
                        });
                    }
                }
            }
        }



//        else if(idOfBossSkill == 1){
//            line = getAline();
//            damage = damage + 0;
//            for(int i = 0;i<8;i++){
//                if(charactersInBattleFiled[i][line].id != -1){
//                    charactersInBattleFiled[i][line].lives -= damage;
//                }
//            }
//        }
        else if (idOfBossSkill == 1) {   //只命中第一个
            int line = getAline();
            damage = damage + 0;
            for (int i = 0; i < 8; i++) {
                if (charactersInBattleFiled[i][line].id != -1) {
                    Character character = charactersInBattleFiled[i][line];
                    charactersInBattleFiled[i][line].lives -= damage;

                    // 特效
                    battlefieldWithImages[i][line].clear(); // 取消显示图片
                    battlefieldWithImages[i][line].setMovie(skillImages[idOfBossSkill]); // 需要的是地址
                    skillImages[idOfBossSkill]->start();

                    QTimer::singleShot(1500, this, [this, i, line, character]() {
                        skillImages[idOfBossSkill]->stop();
                        battlefieldWithImages[i][line].clear(); // 取消显示 GIF
                        battlefieldWithImages[i][line].setPixmap(images[character.id].scaled(QSize(210, 210))); // 设置图片
                        if (characterDieOrNot(charactersInBattleFiled[i][line])) {
                            battlefieldWithImages[i][line].clear(); // 取消显示图片
                        }
                    });

                    break;
                }
            }
        }

        else if(idOfBossSkill == 2){
            IamBeingAttack(damage+1);
        }

        else {
            int i = 0;
            QTimer *effectTimer = new QTimer(this);
            connect(effectTimer, &QTimer::timeout, this, [=]() mutable {
                line = getAline();
                column = getAcolumn();
                if (charactersInBattleFiled[column][line].id != -1) {
                    Character character = charactersInBattleFiled[column][line];
                    charactersInBattleFiled[column][line].lives -= damage;

                    // 特效
                    battlefieldWithImages[column][line].clear(); // 取消显示图片
                    battlefieldWithImages[column][line].setMovie(skillImages[idOfBossSkill]); // 需要的是地址
                    skillImages[idOfBossSkill]->start();

                    QTimer::singleShot(1500, this, [=]() {
                        skillImages[idOfBossSkill]->stop();
                        battlefieldWithImages[column][line].clear(); // 取消显示 GIF
                        battlefieldWithImages[column][line].setPixmap(images[character.id].scaled(QSize(210, 210))); // 设置图片
                        if (characterDieOrNot(charactersInBattleFiled[column][line])) {
                            battlefieldWithImages[column][line].clear(); // 取消显示图片
                        }
                    });

                    effectTimer->stop(); // 停止定时器
                }

                i++;
                if (i > 40) {
                    effectTimer->stop(); // 停止定时器
                }
            });

            // 启动定时器
            effectTimer->start(100); // 每100毫秒触发一次
        }

    }
}

void game::robotSkillRandomly()
{
    idOfBossSkill = QRandomGenerator::global()->bounded(0, 4);

}

void game::activeDebug(Character & character)
{
    //character.cooldown_current = 0;
}



void game::hasBeenUsed()
{
    //need to refresh the time of cooldown
    mouseEventSuccessOrNot = true;

}

void game::coolDownAll()
{
    int i_;
    int j_;
    short id;
    if(justGetCard){
        for(int i = 0; i<8; i++){
            for(int j = 0; j<5; j++){
                id = charactersInBattleFiled[i][j].id;
                if(id != -1){
                    if(charactersInBattleFiled[i][j].cooldown_current > 0){
                        charactersInBattleFiled[i][j].cooldown_current--;
                    }
                    if(charactersInBattleFiled[i][j].cooldown_current <= 0){
                        // 将 battlefieldWithImages[targetRow][targetCol] 的图片替换为 star 图片
                           battlefieldWithImages[i][j].setPixmap(QPixmap(":/star.png").scaled(QSize(210, 210)));
                           int _i = i;
                           int _j = j;
                           // 使用 QTimer 在 1 秒后恢复图片
                           QTimer::singleShot(1000, [=]() {
                               battlefieldWithImages[_i][_j].clear();
                               battlefieldWithImages[_i][_j].setPixmap(images[charactersInBattleFiled[_i][_j].id].scaled(QSize(210, 210))); // 设置图片
                           });
                    }


                }
            }
        }
        justGetCard = false;
    }

}

void game::mousePressEvent(QMouseEvent *event)
{

    //The x, y in mouseEvent is opposite to the x, y of the array
    clickPos = event->pos();
    x_mouse = clickPos.x();
    y_mouse = clickPos.y();
    if(x_mouse > 1680 || y_mouse > 1050){
        mouseEventSuccessOrNot = false;
        return ;
    }
    x_objcard_position = x_mouse / 210;  //the x,y of charactersInBattleFiled
    y_objcard_position = y_mouse / 210;
    qDebug() << "x:"<<x_objcard_position<<endl<<"y:"<<y_objcard_position<<endl;
    mouseEventSuccessOrNot = true;




    int id;
    id = charactersInBattleFiled[x_objcard_position][y_objcard_position].id;
    if(id != -1){
        Character character = charactersInBattleFiled[x_objcard_position][y_objcard_position];
        //Character character = characters[0];
        prompt = "已选中 " + character.name +
                 "  伤害:  " + QString::number(character.damage_value) +
                 "  速度:  " + QString::number(character.speed) +
                 "  生命值:  " + QString::number(character.lives) +
                 "  剩余冷却:  " + QString::number(character.cooldown_current) +
                 "  攻击距离:   " + QString::number(character.attack_range);
        qDebug()<<prompt;
        ui->prompt->setText(prompt);
    }
    else{
        ui->prompt->setText("无任何角色");
    }



    //solve to Get card randomly
    if(gettingCard){
        if(id != -1){
            ui->prompt->setText("这里已被选中");
            return;
        }

        if(x_objcard_position < 2){//have got the id, ini the battlefiled according to id
            charactersInBattleFiled[x_objcard_position][y_objcard_position].id = theCardJustGet;
            iniCharacterInBattleFiled();
            battlefieldWithImages[x_objcard_position][y_objcard_position].setPixmap(images[theCardJustGet].scaled(QSize(210,210)));//scaled the image
            myhaveGotCardnums++;
            gettingCard = false;
            //set the button disabled
            justGetCard = true;
            //setDisabledForbutton();
            hasBeenUsed();
        }
        else{
            ui->prompt->setText("取消选中");
            gettingCard = false;
            return;
        }
        ui->getCharacter->setEnabled(0);
    }
}


void game::on_getCharacter_clicked()
{

    mouseEventSuccessOrNot = false;
    theCardJustGet = QRandomGenerator::global()->bounded(0, numsofcharacter);
    gettingCard = true;
    prompt = "已抽中 " + characters[theCardJustGet].name + " ,选择英雄位置,必须位于前两格";
    ui->prompt->setText(prompt);  //may be repeat





}

void game::on_moveTheCharacter_clicked()
{
    Character character = charactersInBattleFiled[x_objcard_position][y_objcard_position];
    //----------------------------
    activeDebug(character);
    //----------------------------
    int speed = character.speed;
    qDebug()<<"******"<<character.cooldown_current;
    if(character.id == -1){
        ui->prompt->setText("重新选择");
        return;
    }
    if(charactersInBattleFiled[x_objcard_position + speed][y_objcard_position].id != -1){
        ui->prompt->setText("移动目标位置已被占用");
        return;
    }
    if(mouseEventSuccessOrNot && character.cooldown_current == 0 && speed+x_objcard_position <7){//最多到倒数第二格
                                                //最多到倒数第二格
        charactersInBattleFiled[x_objcard_position + speed][y_objcard_position] = character;
        charactersInBattleFiled[x_objcard_position][y_objcard_position].id = -1;
        battlefieldWithImages[x_objcard_position][y_objcard_position].clear();//取消显示图片
        battlefieldWithImages[x_objcard_position + speed][y_objcard_position].setPixmap(images[character.id].scaled(QSize(210,210)));//set image
        //cool down
         charactersInBattleFiled[x_objcard_position + speed][y_objcard_position].cooldown_current = charactersInBattleFiled[x_objcard_position][y_objcard_position].cooldown_initial;


    }
    else if(character.cooldown_current != 0){
        ui->prompt->setText("冷却不足");
        return;
    }
    else if(mouseEventSuccessOrNot){
        ui->prompt->setText("没有选择");
        return;
    }
    else if(x_objcard_position == maxDisatance){
        return;
    }
    else if(speed+x_objcard_position >(maxDisatance + 1)){
        charactersInBattleFiled[maxDisatance][y_objcard_position] = character;
        charactersInBattleFiled[x_objcard_position][y_objcard_position].id = -1;
        battlefieldWithImages[x_objcard_position][y_objcard_position].clear();//取消显示图片
        battlefieldWithImages[maxDisatance][y_objcard_position].setPixmap(images[character.id].scaled(QSize(210,210)));//set image
       //cool down
        charactersInBattleFiled[maxDisatance][y_objcard_position].cooldown_current = charactersInBattleFiled[x_objcard_position][y_objcard_position].cooldown_initial;

    }
    hasBeenUsed();
//    ui->getCharacter->setEnabled(0);
}

void game::on_attack_clicked()
{

    Character character = charactersInBattleFiled[x_objcard_position][y_objcard_position];
    //----------------------------
    activeDebug(character);
    //----------------------------
    //int speed = character.speed;
//    short theLastAim;
    short damage = character.damage_value;
    if(mouseEventSuccessOrNot && character.cooldown_current == 0 ){//

        if(x_objcard_position + character.attack_range > 7){
//            theLastAim = 8;
//            for(int i = x_objcard_position;i<7;i++){
//                if(charactersInBattleFiled[x_objcard_position][i].id != -1){
//                    //等待扩展可以对地方召唤物进行攻击
//                }
//            }
            attackBoss(damage);
            //cool down
             charactersInBattleFiled[x_objcard_position][y_objcard_position].cooldown_current = charactersInBattleFiled[x_objcard_position][y_objcard_position].cooldown_initial;
        }
        else{
            ui->prompt->setText("未命中任何目标");
            return;
        }


    }


}

void game::on_useSkill_clicked()
{
    mouseEventSuccessOrNot = false;
}

void game::on_finish_clicked()
{
    use_boss_skill();
    coolDownAll();
    ui->getCharacter->setEnabled(1);
}

