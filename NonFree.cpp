// NonFree.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <math.h>
#include <conio.h>
#include <vector>
#pragma warning(disable:4996)

using namespace std;

const int COL_NUM = 20;// 列
const int ROW_NUM = 15;// 行

typedef const char* str;// 定义字符串

enum move_direction {
	TOP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
};

void set_cursor_pos(const int x, const int y)
{
	COORD position;
	position.X = x;
	position.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

// 初始化游戏
str** init_map()
{
	str** arr = new str*[ROW_NUM];
	for (int i = 0; i < ROW_NUM; i++)
	{
		arr[i] = new str[COL_NUM]();
	}

	return arr;
}

str** init_last_frame_map()
{
	str** arr = new str*[ROW_NUM];
	for (int i = 0; i < ROW_NUM; i++)
	{
		arr[i] = new str[COL_NUM]();
	}

	for (int i = 0; i < ROW_NUM; i++)
	{
		for (int j = 0; j < COL_NUM; j++) 
		{
			arr[i][j] = "-1";
		}
	}

	return arr;
}

// 初始化环境
// map: 地图数据
// target_pos: box目标点
// box_pos: box当前的位置
void init_env(str ** map, vector<int*>* target_pos, vector<int*>* box_pos)
{
	bool is_deal_flag = FALSE;
	for (int i = 0; i < ROW_NUM; i += 1)
	{
		for (int j = 0; j < COL_NUM; j += 1)
		{
			if (i == 0 || i == ROW_NUM - 1 || j == 0 || j == COL_NUM - 1)
			{
				// 画墙
				map[i][j] = "#";
			}
			else
			{
				is_deal_flag = FALSE;
				if (i == 1 && j == 1)
				{
					map[i][j] = "P";
					is_deal_flag = TRUE;
				}
				if (!is_deal_flag)
				{
					for (int k = 0; k < target_pos->size(); k++)
					{
						//
						if (i == (*target_pos)[k][1] && j == (*target_pos)[k][0])
						{
							map[i][j] = "*";
							is_deal_flag = TRUE;
							break;
						}
					}
				}
				if (!is_deal_flag)
				{
					for (int k = 0; k < box_pos->size(); k++)
					{
						//
						if (i == (*box_pos)[k][1] && j == (*box_pos)[k][0])
						{
							map[i][j] = "O";
							is_deal_flag = TRUE;
							break;
						}
					}
				}
				if (!is_deal_flag)
				{
					map[i][j] = " ";
				}

				//if ((i == 1 && j == 2) || (i == 1 && j ==3) )// 初始化目标点
				//{
				//	map[i][j] = "*";
				//}
				//else if ((i == 2 && j == 2) || (i == 2 && j == 3))// 初始化箱子
				//{
				//	map[i][j] = "O";
				//}
				//else
				//{
				//	map[i][j] = " ";
				//}
				
			}
		}
	}
}

// 获取用户的输入
void getUserInput(int * cur_player_pos, vector<int*>* box_pos)
{// 如果当前玩家位置上的是box, 则box移动

	move_direction direction = TOP;// 这里故意用的枚举, 其实可以不用，主要是想看一下用法
	int orign_x = cur_player_pos[0], orign_y = cur_player_pos[1];//初始位置 
	switch (getch())// getch() 回显 getche() 不回显
	{// w s --> 表示y, a,d --> 表示x
		case 'w':
			cur_player_pos[0] = max(1,cur_player_pos[0] - 1);
			direction = TOP;
			break;
		case 's':
			cur_player_pos[0] = min(ROW_NUM - 2, cur_player_pos[0] + 1);
			direction = DOWN;
			break;
		case 'd':
			cur_player_pos[1] = min(COL_NUM - 2, cur_player_pos[1] + 1);
			direction = RIGHT;
			break;
		case 'a':
			cur_player_pos[1] = max(1, cur_player_pos[1] - 1);
			direction = LEFT;
			break;
		default:
			break;
	}

	// 移动box
	for (int i = 0; i < box_pos->size(); i++)// 遍历所有box, 看当前玩家的位置是否与box_pos重叠
	{
		if (cur_player_pos[1] == (*box_pos)[i][0] && cur_player_pos[0] == (*box_pos)[i][1])
		{
				// 调整box位置, box和p的移动范围是一样的
				int temp_pos = 0;// 临时位置, 表示玩家将要移动到的位置
				bool can_find_box = FALSE;
				switch (direction)
				{
				case TOP:
					temp_pos = max(1, (*box_pos)[i][1] - 1);
					for (int j = 0; j < box_pos->size(); j++)
					{
						if ((*box_pos)[i][0] == (*box_pos)[j][0] && temp_pos == (*box_pos)[j][1])
						{
							can_find_box = TRUE;
							break;
						}
					}
					if (can_find_box)// 如果原来的位置上有一个box,则不可以移动
					{
						cur_player_pos[0] = orign_x;
					}
					else
					{
						(*box_pos)[i][1] = temp_pos;
					}
					
					break;
				case DOWN:
					temp_pos = min(ROW_NUM - 2, (*box_pos)[i][1] + 1);
					for (int j = 0; j < box_pos->size(); j++)
					{
						if ((*box_pos)[i][0] == (*box_pos)[j][0] && temp_pos == (*box_pos)[j][1])
						{
							can_find_box = TRUE;
							break;
						}
					}
					if (can_find_box)// 如果原来的位置上有一个box,则不可以移动
					{
						cur_player_pos[0] = orign_x;
					}
					else
					{
						(*box_pos)[i][1] = temp_pos;
					}
					break;
				case LEFT:
					temp_pos = max(1, (*box_pos)[i][0] - 1);// 左移, 和1取最大值
					for (int j = 0; j < box_pos->size(); j++)// 遍历所有的box
					{
						if ((*box_pos)[i][1] == (*box_pos)[j][1] && temp_pos == (*box_pos)[j][0])
						{
							can_find_box = TRUE;
							break;
						}
					}
					if (can_find_box)// 如果原来的位置上有一个box,则不可以移动
					{
						cur_player_pos[1] = orign_y;// 玩家位置不动
						// box不处理
					}
					else// 如果没有box
					{
						(*box_pos)[i][0] = temp_pos;
					}

					//if (map[temp_pos][(*box_pos)[i][1]] != "O")
					//	(*box_pos)[i][0] = temp_pos;
					//else
					//	cur_player_pos[1] = orign_y;
					break;
				case RIGHT:
					temp_pos = min(COL_NUM - 2, (*box_pos)[i][0] + 1);
					for (int j = 0; j < box_pos->size(); j++)
					{
						if ((*box_pos)[i][1] == (*box_pos)[j][1] && temp_pos == (*box_pos)[j][0])
						{
							can_find_box = TRUE;
							break;
						}
					}
					if (can_find_box)// 如果原来的位置上有一个box,则不可以移动
					{
						cur_player_pos[1] = orign_y;
					}
					else
					{
						(*box_pos)[i][0] = temp_pos;
					}
					//if (map[temp_pos][(*box_pos)[i][1]] != "O")
					//	(*box_pos)[i][0] = temp_pos;
					//else
					//	cur_player_pos[1] = orign_y;
					break;
			}

			break;
		}
	}


	// 打印玩家的位置
	//cout << cur_player_pos[0] << "," << cur_player_pos[1] << endl;

}

// 更新游戏
bool updateGame(str*** curFrameMap,int* cur_player_pos, vector<int*>* box_pos, vector<int*>* target_pos)
{
	// 如果当前玩家位置上的是box, 则box移动 ---> 这个过程放到输入里面处理吧, 比较简单
	
	// 绘制target *
	for (int i = 0; i < target_pos->size(); i++)
	{
		(*curFrameMap)[(*target_pos)[i][1]][(*target_pos)[i][0]] = "*";
	}

	
	//set_cursor_pos(cur_player_pos[0],cur_player_pos[1]);// 显示
	// 绘制box
	for (int i = 0; i < box_pos->size(); i++)
	{
		(*curFrameMap)[(*box_pos)[i][1]][(*box_pos)[i][0]] = "O";
	}

	// 玩家的显示的层级在box之上, 所以在box之后处理
	(*curFrameMap)[cur_player_pos[0]][cur_player_pos[1]] = "P";

	bool single_target_result = FALSE;// 单个箱子的检测结果
	// 处理游戏结果
	for (int i = 0; i < target_pos->size(); i++)
	{
		single_target_result = FALSE;// 默认为FALSE
		for (int j = 0; j < box_pos->size(); j++)
		{
			if ((*target_pos)[i][0] == (*box_pos)[j][0] && (*target_pos)[i][1] == (*box_pos)[j][1])// 如果target_pos上都box
			{
				single_target_result = TRUE;
				break;
			}
				
		}
		if (!single_target_result)// 只有有一个target检测未通过, 则游戏为通关
			return FALSE;
		
	}

	return TRUE;
}

// 绘制游戏
void draw(str** const map,str** const lastFrameMap, int* cur_player_pos)
{
	// 清理上一轮输出
	//system("cls"); // 这样处理会出现闪屏的情况， 不好
	set_cursor_pos(0, 0);

	// 根据第一个元素判定是不是第一次绘制
	bool is_change_line = lastFrameMap[0][0] == "-1" ? TRUE : FALSE;

	// 重新绘制地图
	for (int i = 0; i < ROW_NUM; i++)
	{
		for (int j = 0; j < COL_NUM; j++)
		{
			if (lastFrameMap[i][j] == "-1")// 如果上一帧没有内容, 直接绘制
			{
				cout << map[i][j];
			}
			else
			{
				if (lastFrameMap[i][j] != map[i][j])
				{// 当前帧 和 上一帧对比, 如果有修改, 则重新绘制
					set_cursor_pos(j, i);// 移动到当前位置, 重新修改该值
					cout << map[i][j];
				}
				else
				{

				}
			}
		}
		if (is_change_line)
		{
			cout << endl;
		}
		
	}

	// 将光标移动到玩家当前所在位置
	set_cursor_pos(cur_player_pos[1], cur_player_pos[0]);
}

// 复制地图
void copy_cur_map_to_last_frame_map(str ***map, str*** last_fram_map)
{
	for (int i = 0; i < ROW_NUM ; i++)
	{
		for (int j = 0; j < COL_NUM; j++)
		{
			(*last_fram_map)[i][j] = (*map)[i][j];
		}
	}
}

//清理玩家当前所在的位置
void clear_cur_player_pos(str*** map, int* cur_player_pos)
{
	if ((*map)[cur_player_pos[0]][cur_player_pos[1]] == "P")
		(*map)[cur_player_pos[0]][cur_player_pos[1]] = " ";
}

//更新box的目标点
void init_target_pos(vector<int*>* target_pos)
{
	target_pos->push_back(new int[2]{ 8,7 });
	target_pos->push_back(new int[2]{ 5,3 });
	target_pos->push_back(new int[2]{ 11,12 });
}

//更新玩家的目标点
void init_box_pos(vector<int*>* box_pos)
{
	box_pos->push_back(new int[2]{ 7,8 });
	box_pos->push_back(new int[2]{ 3,5 });
	box_pos->push_back(new int[2]{ 12,11 });
}

int main()
{
	str** map = init_map();// 初始化地图数据
	str** lastFrameMap = init_last_frame_map();// 初始化上一帧的地图数据
	
	// 初始化一些环境数据
	vector<int*> target_pos;
	vector<int*> box_pos;

	init_target_pos(&target_pos);
	init_box_pos(&box_pos);

	// 将环境添加到底图上
	init_env(map,&target_pos, &box_pos);
	
	int* cur_player_pos = new int[2] {1, 1};// y,x

	bool is_success = FALSE;// 是否成功通关

	wchar_t buf[512];
	while (true)
	{
		// Log 用来打印玩家当前位置的
		//wsprintf(buf, L"%d,%d，", cur_player_pos[0], cur_player_pos[1]);
		//OutputDebugString(buf);
		//wsprintf(buf, L"%s，", map[cur_player_pos[0]][cur_player_pos[1]]);
		//OutputDebugString(buf);
		//wsprintf(buf, L"%s\n", lastFrameMap[cur_player_pos[0]][cur_player_pos[1]]);
		//OutputDebugString(buf);

		draw(map,lastFrameMap,cur_player_pos); // 绘制

		if (is_success) // 如果游戏成功通关, 则退出
		{
			OutputDebugString(L"成功通关！");
			set_cursor_pos(0, ROW_NUM);
			cout << "恭喜您，成功通关!" << endl;
			break;
		}
			
		//lastFrameMap = map;
		copy_cur_map_to_last_frame_map(&map, &lastFrameMap);// 保存当前的帧 map --> lastFrameMap
		clear_cur_player_pos(&map, cur_player_pos);// 清理玩家当前的位置
		
		getUserInput(cur_player_pos,&box_pos);// 获取用户的输入

		is_success = updateGame(&map,cur_player_pos,&box_pos, &target_pos);// 更新游戏逻辑
	}
}
