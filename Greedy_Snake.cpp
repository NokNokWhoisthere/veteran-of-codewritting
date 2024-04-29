#define __LIBRARY__
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Windows.h>
#include <conio.h>
//#include <unistd.h>
#include <fcntl.h>

#define Up 'w'
#define Down 's'
#define Right 'd'
#define Left 'a'

#define __NR_scanning 95

static unsigned long next_s = 1;

struct Snake 
{
    int x;
    int y;
    struct Snake* next;
};

struct Snake* back = NULL;

struct Food 
{
    int x;
    int y;
};

int rand()//将srand更改过的种子值通过公式计算出结果作为随机值
{
    next_s = next_s * 1103515245 + 12345;
    return((unsigned)(next_s / 65536) % 32768);
}
/*
蛇身环路测试，如果重叠则调用deathdetective；若超出场地范围也调用deathdetective
如果是食物则调用eatfood函数
*/
int skhddt(struct Snake* s) 
{
    struct Snake* p = s->next;
    while (p != NULL) 
    {
        if (p->x == s->x && p->y == s->y) 
        {
            return 1;
            break;
        }
        p = p->next;
    }
    return 0;
}

//死亡检测:撞到墙壁或者撞到身体，死亡后打印提示信息
int deathdetective(struct Snake* head) 
{
    if (head->y >= 52 || head->x >= 28 || head->x == 0 || head->y == 0 || skhddt(head)) 
    {
        printf("You Have Died\n");
        return 1;
    }
    else return 0;
}

//吃到食物“#”加长一截身体
//随机生成蛇的位置和食物，保证蛇的位置和食物的位置不会重叠
void InitSnake(struct Snake* head, struct Snake* tail, struct Food* f) 
{
    head->next = tail;
    head->x = rand() % 22 + 1;
    head->y = rand() % 22 + 1;
    tail->x = head->x;
    tail->y = head->y - 1;
    tail->next = NULL;
    f->x = rand() % 22 + 1;
    f->y = rand() % 22 + 1;
}

//随机生成食物
void InitFood(struct Food* f) 
{
    f->x = rand() % 25 + 1;
    f->y = rand() % 48 + 1;
}

//找到食物位置并打印食物
int FoodSpot(int hang, int lie, struct Food* f) 
{
    if (hang == f->x && lie == f->y) 
    {
        printf("#");
        return 1;
    }
    else return 0;
}

//打印蛇
int nodeSpot(int hang, int lie, struct Snake* head) 
{
    struct Snake* s;
    s = head;
    for (; s != NULL; s = s->next) 
    {
        if (hang == head->x && lie == head->y) 
        {
            printf("+");
            return 1;
        }
        else if (hang == s->x && lie == s->y)
        {
            printf("*");
            return 1;
        }
    }//头节点输出+，身体结点输出*
    return 0;
}

//生成游戏场景，包括蛇，食物，墙壁
void gamePid(struct Snake* s, struct Food* f) 
{
    int hang;//定义地图位置行
    int lie;//定义地图位置列
    for (hang = 0; hang <= 28; hang++) 
    {
        for (lie = 0; lie <= 50; lie++) 
        {
            if (hang == 0 || hang == 28) 
            {//地图第一行和最后一行为“==”墙
                printf("=");
            }
            else if (lie == 0) 
            {//地图第一列和最后一列为“||”墙
                printf("||");
            }
            else if (lie == 50) 
            {
                printf(" ||");
            }
            else if (nodeSpot(hang, lie, s));  //在对应位置打印蛇头和蛇身
            else if (FoodSpot(hang, lie, f));   //在对应位置打印食物
            else printf(" ");
        }
        printf("\n");
    }
    Sleep(500);     //停顿0.5s再刷新屏幕
}

//扫描键盘输入流中的内容
void sys_scanning(char* direction) 
{
    char d;
    if (_kbhit()) 
    {
        d = _getch();
        switch (d) 
        {
            case Up: 
            {
                if (*direction == Down)
                    break;
                *direction = d;
            }
            case Right: 
            {
                if (*direction == Left)
                    break;
                *direction = d;
            }
            case Down: 
            {
                if (*direction == Up)
                    break;
                *direction = d;
            }
            case Left: 
            {
                if (*direction == Right)
                    break;
                *direction = d;
            }
            break;
        }
    }
}
//蛇头移动后，整个身体跟着移动
void SnakeBodyMove(struct Snake* head, int x, int y) 
{
    struct Snake* p = head->next;
    int t0_x = 0;
    int t0_y = 0;
    while (p != NULL) 
    { //将蛇身链表的x，y值跟随着蛇头移动
        t0_x = p->x;    //先将此段蛇身值保存，覆盖下一段蛇身
        t0_y = p->y;
        p->x = x;       //前一段蛇身或蛇头的坐标值覆盖此段蛇身
        p->y = y;
        x = t0_x;       //此段蛇身数据留给下一段蛇身，若没有后续蛇身则不做操作
        y = t0_y;
        p = p->next;
    }
}

void eatfood(struct Snake* s, struct Food* f) 
{
    if (s->x == f->x && s->y == f->y) 
    {
        struct Snake* ss = (struct Snake*)malloc(sizeof(struct Snake));
        back->next = ss;
        back = back->next;
        back->next = NULL;
    }
}

//控制蛇移动，初始默认向右移动
void SnakeMove(struct Snake* s, struct Food* f) 
{
    int i = 0;
    int tmp = 0;
    int x, y = 0;
    char direction = Right; //起始默认移动方向向右
    struct Snake* p = s;
    //如果输入相反方向则此方向操作无效
    /*
    在不中断程序的情况下实现输入，
    没有输入就照常运行SnakeMove，有输入有更改移动方向
    */
    while (1) 
    {
        if (direction == Right) {    //向右移动
            for (i = s->y; i <= 50 && Right == direction; i++)
            {
                x = s->x;
                y = s->y;
                s->y++;
                eatfood(s, f);
                SnakeBodyMove(s, x, y);
                if (deathdetective(s))
                    return;
                gamePid(s, f);
                sys_scanning(&direction);
            }
        }
        else if (direction == Up)   //向上移动
        {
            for (i = s->x; i >= 0 && Up == direction; i--)
            {
                x = s->x;
                y = s->y;
                s->x--;
                eatfood(s, f);
                SnakeBodyMove(s, x, y);
                if (deathdetective(s))
                    return;
                gamePid(s, f);
                sys_scanning(&direction);
            }
        }
        else if (direction == Left) //向左移动
        {
            for (i = s->y; i >= 0 && Left == direction; i--)
            {
                x = s->x;
                y = s->y;
                s->y--;
                eatfood(s, f);
                SnakeBodyMove(s, x, y);
                if (deathdetective(s))
                    return;
                gamePid(s, f);
                sys_scanning(&direction);
            }
        }
        else if (direction == Down) //向下移动
        {
            for (i = s->x; i <= 28 && Down == direction; i++)
            {
                x = s->x;
                y = s->y;
                s->x++;
                eatfood(s, f);
                SnakeBodyMove(s, x, y);
                if (deathdetective(s))
                    return;
                gamePid(s, f);
                sys_scanning(&direction);
            };
        }
    }
}

int main() 
{
    struct Snake* s = (struct Snake*)malloc(sizeof(struct Snake));
    struct Snake* t = (struct Snake*)malloc(sizeof(struct Snake));
    struct Food* f = (struct Food*)malloc(sizeof(struct Food));
    struct Snake* p = NULL;
    InitSnake(s, t, f);
    InitFood(f);
    back = t;
    gamePid(s, f);
    SnakeMove(s, f);
    while(s!=NULL)      //释放申请的内存空间
    {
        p = s;
        s = s->next;
        free(p);
    }
    if( f != NULL )
    {
        free(f);
    }
    return 0;
}
