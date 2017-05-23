#pragma once
#include "Pos.h"
class chess
{
public:
	chess();
	~chess();
	void welcome();
	double dis(int x1, int y1, int x2, int y2);
	bool JudgeChess(int x, int y);
	void PlayGame1();
	void GameOver1(int n);
	void PlayGame2();
	void GameOver2(int n);
	Pos GetAddress();
};

