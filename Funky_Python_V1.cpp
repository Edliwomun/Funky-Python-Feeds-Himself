/*---------------------------------------------------------------------------------
|筆記：  							                  |
|這是一個貪食蛇AI，目前使用了三個struct，分別是蛇的本體，食物，以及方向       	  |
|程式流程：                                                                       |     
|1.初始化遊戲場->2.初始化蛇的狀態->3.隨機彈出食物->4.計算方向及距離->5.走一步->   |
|6.確認是否吃到->7.若吃到則3.否則8.->8.顯示目前狀態->5.                           |
|思考：                                                                           |
|有沒有辦法可以簡化這個方式？				                          |
|還沒做的功能有：								  |
|1.食物不可以出現在蛇的身上							  |
|2.不可以穿越自己的身體								  |
|3.撞到自己身體死掉                                                               |
|4.自動規劃路徑（走到滿才死掉）	                                                  |
---------------------------------------------------------------------------------*/
#include<cstdio>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>

#define ground_size 30

using namespace std;

void initialize_game(void); // 初始化遊戲
void select_food_place(void);// 隨機產生一個食物
void show_animate(void); //顯示現在狀態
void snake_initial(void); //初始化蛇
void snake_status(void); //蛇的狀態
int check_eat(void);	//檢查是否吃到
void set_direct(void);	//決定走的方向

char ground[ground_size][ground_size];//廣場大小
bool ground_check[ground_size][ground_size];//廣場布林值

struct snake{ 		
	//蛇的資料
	int score;
	int length;
	int snake_X[ground_size*ground_size+1];
	int snake_Y[ground_size*ground_size+1];
}run_snake;

struct food{
	//食物座標
	int food_X;
	int food_Y;
}snake_food;

struct direction{
	//分別存入各個方向應該走幾步，通常會有兩個值為0
	int direction_X;
	int direction_Y;
}snake_direct;

int main(){		//過程
	int position=0;
	snake_initial();
	initialize_game();
	select_food_place();
	set_direct();
	while(1){
		snake_status();
		//printf("%s ","process code 1");
		position=check_eat();
		//printf("%s\n","process code 2");
		if(position==1){
			select_food_place();	
			//printf("%s ","process code 3");
			set_direct();
			//printf("%s\n","process code 4");
		}
		show_animate();
	}		
	
}

void initialize_game(void){	//將廣場,廣場布林值及蛇頭位置初始化
	for(int i=0;i<ground_size;i++){
		for(int j=0;j<ground_size;j++){
			ground[i][j]='.';
		}
	}
	for(int i=0;i<ground_size;i++){
		for(int j=0;j<ground_size;j++){
			ground_check[i][j]=true;
		}
	}
}

void select_food_place(void){		//隨機產生食物的位置
	srand(time(NULL));
	snake_food.food_X=rand()%ground_size;
	snake_food.food_Y=rand()%ground_size;
	ground[snake_food.food_X][snake_food.food_Y]='O';
	ground_check[snake_food.food_X][snake_food.food_Y]=false;
}

void show_animate(void){		//顯示動畫
	for(int j=0;j<ground_size;j++){
		for(int i=0;i<ground_size;i++){
			printf("  %c",ground[i][j]);
		}
		printf("%c",'\n');
	}
	printf("  The Snake's head is at: (%d,%d)\n",run_snake.snake_X[0],run_snake.snake_Y[0]);
	printf("  The food is at: (%d,%d)\n",snake_food.food_X,snake_food.food_Y);
	printf("  Snake has eaten %d foods.\n",run_snake.score);
	printf("  Snake's length is: %d\n",run_snake.length);
	usleep(400000);
	system("clear");
}

void snake_initial(void){		//初始化蛇的物件
	int default_length=5;
	//printf("%s","Please input the snake length: ");
	//scanf("%d",&default_length);		客製化輸入蛇得出長度
	run_snake.score=0;		//初始分數
	run_snake.length=default_length;	//初始長度
	for(int i=0;i<run_snake.length;i++){
		run_snake.snake_X[i]=0;
		run_snake.snake_Y[i]=i;
	}
	ground_check[0][0]=ground_check[0][1]=ground_check[0][2]=ground_check[0][3]=ground_check[0][4]=0;
}

void snake_status(void){		//蛇走路
/*------------------------------------------------------------------------------------------------
|將目前的陣列值向後移一位，然後第一個剛好就是剛剛頭的數值，就可以依照方向去改變蛇頭的座標        |
|用迴圈將前length項的資料變成蛇的身體，最後一位被移出去的，就是已經不是身體了，把他改回.         |
-------------------------------------------------------------------------------------------------*/
	if(snake_direct.direction_X>0){
		for(int i=run_snake.length-1;i>=0;i--){
			run_snake.snake_X[i+1]=run_snake.snake_X[i];
			run_snake.snake_Y[i+1]=run_snake.snake_Y[i];
		}
		run_snake.snake_X[0]++;
		snake_direct.direction_X--;
	}
	else if(snake_direct.direction_X<0){
		for(int i=run_snake.length-1;i>=0;i--){
			run_snake.snake_X[i+1]=run_snake.snake_X[i];
			run_snake.snake_Y[i+1]=run_snake.snake_Y[i];
		}
		run_snake.snake_X[0]--;
		snake_direct.direction_X++;
	}
	else if(snake_direct.direction_Y>0){
		for(int i=run_snake.length-1;i>=0;i--){
			run_snake.snake_X[i+1]=run_snake.snake_X[i];
			run_snake.snake_Y[i+1]=run_snake.snake_Y[i];
		}
		run_snake.snake_Y[0]++;
		snake_direct.direction_Y--;
	}
	else if(snake_direct.direction_Y<0){
		for(int i=run_snake.length-1;i>=0;i--){
			run_snake.snake_X[i+1]=run_snake.snake_X[i];
			run_snake.snake_Y[i+1]=run_snake.snake_Y[i];
		}
		run_snake.snake_Y[0]--;
		snake_direct.direction_Y++;
	}
	for(int i=0;i<run_snake.length;i++){
		ground[run_snake.snake_X[i]][run_snake.snake_Y[i]]='@';
	}
	ground[run_snake.snake_X[run_snake.length]][run_snake.snake_Y[run_snake.length]]='.';
	return ;
	/*測試用的code別理他
	ground[run_snake.snake_X[0]][run_snake.snake_Y[0]]='@';
	ground[run_snake.snake_X[0]][run_snake.snake_Y[0]-1]='.';
	run_snake.snake_Y[0]++;	

	if(run_snake.snake_Y[0]>=ground_size)run_snake.snake_Y[0]=0;
	if(run_snake.snake_X[0]>=ground_size)run_snake.snake_X[0]=0;
	if(run_snake.snake_Y[0]<0)run_snake.snake_Y[0]=ground_size;
	if(run_snake.snake_X[0]<0)run_snake.snake_X[0]=ground_size;*/
}
int check_eat(void){
	//若座標重疊，則回傳吃到的1
	if(run_snake.snake_X[0]==snake_food.food_X && run_snake.snake_Y[0]==snake_food.food_Y){
		printf("\a");
		run_snake.score++;//分數+1
		run_snake.length++;//長度+1
		return 1;
	}
	else return 0;
}
void set_direct(void){
	/*決定方向：
		食物座標-蛇頭座標。
		若X<0向左走，若X>0向右走
		若Y<0向下走，若Y>0向上走
	*/
	snake_direct.direction_X=(snake_food.food_X)-(run_snake.snake_X[0]);
	snake_direct.direction_Y=(snake_food.food_Y)-(run_snake.snake_Y[0]);
}
