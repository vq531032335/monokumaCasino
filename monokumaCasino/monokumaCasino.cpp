// monokumaCasino.cpp: 定义控制台应用程序的入口点。
//1白 2粉 3黄 4蓝

#include "stdafx.h"
#include<iostream>
#include<string>
#include<vector>
using namespace std;
struct Node
{
	int value;
	int able;
};

struct NodeInfo
{
	int row;
	int column;
};
Node** initiateBlocks(int m,int n);
void displayBlocks(Node** block, int m,int n,int s1,int s2,int e1,int e2);
int getClickable(Node** block, int m, int n, vector<NodeInfo>& result);
bool calCasino(Node** block, int m, int n, vector<NodeInfo> process, int s1, int s2, int e1, int e2);
Node** click(Node** block, int m, int n, int c1, int c2);
vector<NodeInfo> getNeighboor(Node** block, int m,int n,int c1,int c2);
void pushVector(vector<NodeInfo>& result, int c1, int c2);


int main()
{
	while (true)
	{

		int m;
		int n;
		cout << "行数 竖排数\n";
		cin >> m;
		cin >> n;
		//initiate blocks
		Node** block = initiateBlocks(m, n);

		int s1;
		int s2;
		cout << "初始点坐标\n";
		cin >> s2;
		cin >> s1;

		int e1;
		int e2;
		cout << "结束点坐标\n";
		cin >> e2;
		cin >> e1;


		displayBlocks(block, m, n, s1, s2, e1, e2);

		vector<NodeInfo> process;
		calCasino(block, m, n, process, s1, s2, e1, e2);

		cout << "\n";

	}
    return 0;
}

Node** initiateBlocks(int m, int n)
{
	Node** block = new Node*[m + 2];
	for (int i = 0; i < m + 2; i++)
	{
		block[i] = new Node[n + 2];
	}

	for (int i = 0; i < n + 2; i++)
	{
		block[0][i].value = block[0][i].able = 0;
		block[m + 1][i].value = block[m + 1][i].able = 0;
	}
	for (int i = 1; i <= m; i++)
	{
		block[i][0].value = block[i][0].able = 0;
		block[i][n + 1].value = block[i][n + 1].able = 0;
		cout << "第" << i << "行\n";
		for (int j = 1; j <= n; j++)
		{
			cin >> block[i][j].value;
			block[i][j].able = 0;
		}
	}
	return block;
}

void displayBlocks(Node** block,int m, int n, int s1, int s2, int e1, int e2)
{
	for (int i = 0; i < m + 2; i++)
	{
		for (int j = 0; j < n + 2; j++)
		{
			if (i == s1&&j == s2)
			{
				cout << "(" << block[i][j].value << ")";
			}
			else
			{
				if (i == e1&&j == e2)
				{
					cout << "{" << block[i][j].value << "}";
				}
				else
				{
					cout << " " << block[i][j].value << " ";
				}
			}
		}
		cout << "\n";
	}

}

//get the clickable number of sets of blocks and change the parameter "result" who presents all the clickable blocks
int getClickable(Node** block,int m,int n, vector<NodeInfo>& result)
{
	int num = 0;
	//
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (block[i][j].value != 0 && block[i][j].able == 0)
			{
				vector<NodeInfo> temp = getNeighboor(block, m, n, i, j);
				if (temp.size() > 0)
				{
					num++;
					pushVector(result, i, j);
					for (auto iter = temp.cbegin(); iter != temp.cend(); iter++)
					{
						block[(*iter).row][(*iter).column].able = 1;
					}
				}
			}
		}
	}
	return num;
}

//do all the available clicks in a loop and check whether it meets the success
bool calCasino(Node** block, int m, int n, vector<NodeInfo> process, int s1, int s2, int e1, int e2)
{
	vector<NodeInfo> ClickableResult;
	int temp = getClickable(block, m, n, ClickableResult);

	if (temp == 0)
	{
		bool success=true;
		//fit the goal?
		for (int i = s1; i <= e1; i++)
		{
			for (int j = s2; j <= e2; j++)
			{
				if (block[i][j].value != 0)
				{
					success = false;
				}
			}
		}
		if (success)
		{
			//print process
			for (auto iter = process.cbegin(); iter != process.cend(); iter++)
			{
				cout << "(" << (*iter).column<<"," << (*iter).row<<")" << endl;
			}
			displayBlocks(block, m, n, s1, s2, e1, e2);
			return true;
		}
	}
	else
	{
		for (int i = 0; i < temp; i++)
		{
			Node** nowblock=click(block,m,n, ClickableResult[i].row, ClickableResult[i].column);
			process.push_back(ClickableResult[i]);
			if (calCasino(nowblock, m, n, process, s1, s2, e1, e2) == true)
			{
				return true;
			}
			else
			{
				process.pop_back();
			}
		}
	}
	return false;
}

//click on a block and return the result of the blocks
Node** click(Node** block,int m,int n,int c1,int c2)
{
	//copy
	Node** result = new Node*[m + 2];
	for (int i = 0; i < m + 2; i++)
	{
		result[i] = new Node[n + 2];
	}

	for (int i = 0; i < n + 2; i++)
	{
		result[0][i].value = result[0][i].able= 0;
		result[m + 1][i].value= result[m + 1][i].able = 0;
	}
	for (int i = 1; i <= m; i++)
	{
		result[i][0].value= result[i][0].able = 0;
		result[i][n + 1].value= result[i][n + 1].able = 0;
		for (int j = 1; j <= n; j++)
		{
			result[i][j].value = block[i][j].value;
			result[i][j].able = 0;
		}
	}
	vector<NodeInfo> neighboors = getNeighboor(result, m, n, c1, c2);
	for (auto iter = neighboors.cbegin(); iter != neighboors.cend(); iter++)
	{
		Node* tempBlock = &result[(*iter).row - 1][(*iter).column];
		if (tempBlock->value != 0 && tempBlock->able == 0) { tempBlock->able = 2; }
		tempBlock = &result[(*iter).row + 1][(*iter).column];
		if (tempBlock->value != 0 && tempBlock->able == 0) { tempBlock->able = 2; }
		tempBlock = &result[(*iter).row][(*iter).column - 1];
		if (tempBlock->value != 0 && tempBlock->able == 0) { tempBlock->able = 2; }
		tempBlock = &result[(*iter).row][(*iter).column + 1];
		if (tempBlock->value != 0 && tempBlock->able == 0) { tempBlock->able = 2; }
	}
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (result[i][j].able == 1)
			{
				result[i][j].value = 0;
			}
			if (result[i][j].able == 2)
			{
				result[i][j].value = result[i][j].value % 4 + 1;//1 2 3 4 => 2 3 4 1
			}
			result[i][j].able = 0;
		}
	}
	return result;
}

//given blocks and the click point, return all the neighboor points
vector<NodeInfo> getNeighboor(Node** block, int m, int n, int c1, int c2)
{
	vector<NodeInfo> result;	
	if (block[c1][c2].value!=0)//排除没格子的
	{
		block[c1][c2].able = 1;
		pushVector(result, c1, c2);
		while (true)
		{
			bool ifchange = false;
			vector<NodeInfo> tempresult=result;
			for (auto iter = result.cbegin(); iter != result.cend(); iter++)
			{
				int tempValue = block[(*iter).row][(*iter).column].value;
				Node* tempBlock = &block[(*iter).row - 1][(*iter).column];//上
				if (tempBlock->able == 0 && tempBlock->value == tempValue)
				{
					tempBlock->able = 1;
					ifchange =true;
					pushVector(tempresult, (*iter).row - 1, (*iter).column);
				}
				tempBlock = &block[(*iter).row + 1][(*iter).column];//下
				if (tempBlock->able == 0 && tempBlock->value == tempValue)
				{
					tempBlock->able = 1;
					ifchange =true;
					pushVector(tempresult, (*iter).row + 1, (*iter).column);
				}
				tempBlock = &block[(*iter).row][(*iter).column - 1];//左
				if (tempBlock->able == 0 && tempBlock->value == tempValue)
				{
					tempBlock->able = 1;
					ifchange =true;
					pushVector(tempresult, (*iter).row , (*iter).column-1);
				}
				tempBlock = &block[(*iter).row][(*iter).column + 1];//右
				if (tempBlock->able == 0 && tempBlock->value == tempValue)
				{
					tempBlock->able = 1;
					ifchange =true;
					pushVector(tempresult, (*iter).row , (*iter).column+1);
				}
			}
			result.swap(tempresult);
			if (ifchange == false)
			{
				break;
			}
		}
		if (result.size()==1)//排除单个的
		{
			result.clear();
		}
	}
	return result;
}

void pushVector(vector<NodeInfo>& result,int c1,int c2)
{
	NodeInfo temp;
	temp.row = c1;
	temp.column = c2;
	result.push_back(temp);
}