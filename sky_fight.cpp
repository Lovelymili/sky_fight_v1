#include <iostream>
#include <easyx.h>
#include <conio.h>
#include <random>
#include <thread>
#include <chrono>

#include <mutex>
#include <future>
#include "log.h"
#include <vector>
std::mutex mtx;

plane createenemy();


int main() {
    initgraph(400, 600);

    IMAGE img[4];
    loadimage(&img[0], "images\\background.png", 400, 600);
    loadimage(&img[1], "images\\me2.png", 50, 80); // 我方飞机
    loadimage(&img[2], "images\\bullet2.png", 8, 8); // 子弹
    loadimage(&img[3], "images\\enemy1.png", 25, 25); // 敌方飞机

    plane Myplane = { 200, 500, 100 }; // 初始化自己的飞机

    // 使用 std::async 代替 promise 和 future

    //std::cout << enemy.x << std::endl;
    std::default_random_engine e;
    e.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> u1(-10, 10);
    std::uniform_int_distribution<int> u2(-10, 10);

    int n = 1;
    //bullet enemybullet = { enemy.x, enemy.y };
    std::vector<bullet>  MyBullet;
    std::vector<bullet> EnemyBullet;
    std::vector<plane> enemyplanes;

    for (int i = 0; i <= 4; i++) {
        std::future<plane> enemyFuture = std::async(std::launch::async, createenemy);
        enemyplanes.push_back(enemyFuture.get());
    }
    auto lastShotTime = std::chrono::steady_clock::now(); // 记录最后一次发射子弹的时间点

    while (1) {
        // 检测按键
        if (_kbhit()) {
            int ch = _getch();
            //std::cout << "收到键盘消息: " << ch << std::endl;
            if (ch == 75) { // 左箭头键值
                Myplane.x -= 20;
            }
            else if (ch == 77) { // 右箭头键值
                Myplane.x += 20;
            }
            else if (ch == 32)
            {
                bullet mynewbullet;
                mynewbullet.x = Myplane.x;
                mynewbullet.y = Myplane.y;
                mynewbullet.n = 0;
                MyBullet.push_back(mynewbullet);
            }
            else if (ch == 27) { // ESC 键值
                break;
            }
        }
        // 锁定屏幕绘制
        mtx.lock();
        BeginBatchDraw();
        cleardevice();
        putimage(0, 0, &img[0]); // 绘制背景
        putimage(Myplane.x, Myplane.y, &img[1]); // 绘制我方飞机
        if (enemyplanes.size() > 0) {
            for (int i = 0; i <= enemyplanes.size() - 1; i++)
            {

                if (enemyplanes[i].visible = 1) {
                    putimage(enemyplanes[i].x , enemyplanes[i].y, &img[3]); // 绘制敌方飞机图像和他们发射的子弹
                    enemyplanes[i].x += u1(e); enemyplanes[i].y += u2(e);
                    auto currentTime = std::chrono::steady_clock::now();
                    std::chrono::duration<double> elapsed = currentTime - lastShotTime;

                    if (elapsed.count() >= 2.0) { // 检查是否经过了2秒
                        for (int i = 0; i < enemyplanes.size(); i++) {
                            if (enemyplanes[i].visible == 1) {
                                for (int k = 0; k < 5; k++) {
                                    bullet enemybullet;
                                    enemybullet.x = enemyplanes[i].x;
                                    enemybullet.y = enemyplanes[i].y;
                                    enemybullet.n = 0;
                                    EnemyBullet.push_back(enemybullet);
                                }
                            }
                        }
                        lastShotTime = currentTime; // 更新最后一次发射子弹的时间点
                    }
                }

                else
                {
                    // MyBullet.erase(MyBullet.begin() + i);
                }

            }

        //putimage(enemybullet.x , enemybullet.y + 30 * n, &img[2]); // 绘制子弹图像
        if (MyBullet.size() > 0) {
            for (int i = 0; i <= MyBullet.size() - 1; i++)
            {

                if (MyBullet[i].y + (-30)>= 0) {
                    putimage(MyBullet[i].x, MyBullet[i].y, &img[2]); // 绘制我方子弹图像
                    //std::cout << "绘制我方子弹" << std::endl;
                    MyBullet[i].y = MyBullet[i].y + (-30) * MyBullet[i].n;
                    MyBullet[i].n++;
                    for (int j = 0; j < enemyplanes.size(); j++)
                    {

                        //std::cout << "MyBullet[" << i << "] x: " << MyBullet[i].x << " y: " << MyBullet[i].y + (-30) * MyBullet[i].n << std::endl;
                        //std::cout << "enemyplanes[" << j << "] x: " << enemyplanes[j].x << " y: " << enemyplanes[j].y << std::endl;
                        if (abs(MyBullet[i].x - (EnemyBullet[j].x + 12.5)) <= 12.5 && abs(MyBullet[i].y - enemyplanes[j].y )<= 12)
                        {
                            enemyplanes[j].hp -= 20;
                            std::cout << "敌机" << j << "又被主播打掉20滴血,恭喜主播,对方剩余生命:" << enemyplanes[j].hp << std::endl;
                            if (enemyplanes[j].hp == 0)
                            {
                               // enemyplanes.erase(enemyplanes.begin() + i);
                                //i--;
                            }
                        }
                    }
                }
                else
                {
                    //MyBullet.erase(MyBullet.begin() + i);
                    //i--;
                }

            }
        }
        else
        {
            //std::cout << "恭喜主播全部打完了" << std::endl;
            //exit(1);
        }
        if (EnemyBullet.size() > 0) {
            for (int i = 0; i < EnemyBullet.size(); i++)
            {

                if (EnemyBullet[i].y + 30 < 600) {
                    putimage(EnemyBullet[i].x, EnemyBullet[i].y, &img[2]); // 绘制敌方子弹图像
                    //std::cout << "绘制敌方子弹" << std::endl;
                    EnemyBullet[i].y = EnemyBullet[i].y + 30 * EnemyBullet[i].n;
                    EnemyBullet[i].n++;
                    if (EnemyBullet[i].y> 600)
                    {
                        n = 1;
                    }
                    if (abs(EnemyBullet[i].x - (Myplane.x + 25)) <= 25 && abs(EnemyBullet[i].y - Myplane.y) <= 3)
                    {
                        Myplane.hp -= 20;
                        std::cout << "我方被击中!剩余血量:" << Myplane.hp << std::endl;
                       // EnemyBullet.erase(EnemyBullet.begin() + i);
                        //i--;
                        if (Myplane.hp == 0)
                        {
                            std::cout << "牢大你坠机了,这是debug,饶你狗命" << std::endl;
                           // exit(1);
                        }
                    }
                }
                else
                {
                    //EnemyBullet.erase(EnemyBullet.begin() + i);
                    //i--;
                }

            }
        }

        n++;
 
        //drawenemy(enemy, img); // 绘制敌方飞机
        EndBatchDraw();
        mtx.unlock();
        //std::cout << "飞机的坐标是:" << Myplane.x << " " << Myplane.y << std::endl; //飞机是200 500
        //std::cout << "子弹的坐标是:" << bullet.x << " " << bullet.y + 30 * (n-1) << std::endl;
        /*if (bullet.y + 30 * (n - 1) - Myplane.y <= 100) {
            std::cout << "坐标差值是:" << bullet.x - Myplane.x << " " << bullet.y + 30 * (n - 1) - Myplane.y << std::endl;
        }
        */
        }
        Sleep(100);
    }
    closegraph();
    return 0;
}

plane createenemy() {
    std::default_random_engine e;
    e.seed(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> u1(10, 380);
    std::uniform_int_distribution<int> u2(10, 50);
    plane enemy = { u1(e), u2(e), 100 }; // 初始化敌方飞机
    return enemy;
}
