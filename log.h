#pragma once

struct plane
{
	int x;
	int y;
	int hp;
	int visible = 1;
};

struct bullet
{
	int x;
	int y;
	int dam = 20;
	int n = 0;
	int visible = 1;
};
