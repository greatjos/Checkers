#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>


class Checkers
{

public:
	Checkers(int depth);
	~Checkers();
	void instructions();
	bool playerMove(int rw,int col, int rwm, int colm);
	void createBoard();
	void showBoard();
	void CpuMove(bool&);
	int doJump(int prev_row, int prev_col, int new_row, int new_col);
	bool P1_forcejump();
	const int return_player_score() const;
	const int  return_cpu_score() const;
	std::map<int,std::vector<int> > returnMoves();
	std::map<int,std::vector<int> > return_jumpMoves();





private:
	bool jmp_repeat;
	int save_row;
	int save_col;
	int** boardScore;
	int player1;
	int cpu;
	int level;
	std::map<int,std::vector<int> > mp_jump;
	std::vector<std::pair<int,int> > save_jump;
	std::string** board;
	void findMovePosition(int& row,int& colmun,int&,int&,bool&,bool&);
	void P1minMaxJump(int r,int c,std::string**& c_board,bool isKing);
	void keepJump(int r,int c,std::vector<std::pair<int,int> >& save_jmp,std::string**& c_board,bool isKing);
	void CPUdoJump(int& prevrow,int& prevcolum,std::map<int,std::vector<int> >& cpu_jump);
	void destroy(std::string**& des_board);
	void CPUminMaxJump(int r,int c,std::string**& c_board,bool isKing);
	void show(std::string**& cb);
	int EvaluateBoard(std::string**& c_board);
	int minimaxAlg(bool isplayer,std::string**& c_board,int depth);
	int MinMaxCPUdoJump(std::string**& c_board,int depth,std::map<int,std::vector<int> >& cpu_jump);
	int P1MinMaxdoJump(std::string**&,int depth,std::map<int,std::vector<int> >& p1_jump);
	bool forceJump(std::string**& c_board);
	bool CPUforceJump(std::string**& c_board,std::map<int,std::vector<int> >& cpu_jump);
	bool P1_minMaxforceJump(std::string**& c_board,std::map<int,std::vector<int> >& p1_jump);
	std::string** copyBoard(std::string**& orig_board);
	std::map<int,std::vector<int> > getMoves(std::string**& c_board,bool isplayer);

};