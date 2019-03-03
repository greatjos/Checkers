#include "Checkers.h"
using namespace std;

//Creates a dummy board and gives each position in the board a score
Checkers::Checkers(int depth)
{
	board = new std::string*[8];
	boardScore = new int*[8];
	jmp_repeat = false;
	switch(depth)
	{
		case(1): level = 1; break;
		case(2): level = 4; break;
		case(3): level = 8;
		cout << "Note: There will be a delay in this level" << endl;
	    break;
	}


	for (int i = 0; i < 8; ++i)
	{
		board[i] = new std::string[8];
		boardScore[i] = new int[8];
		for (int j = 0; j < 8; ++j)
		{
			board[i][j] = "";
			if ((i == 0 && (j % 2)) || (j == 0 && (i % 2)) ||
				(i == 7 && !(j % 2)) || (j == 7 && !(i % 2)))
				boardScore[i][j] = 4;

			else if ((i == 1 && !(j % 2)) || (j == 1 && !(i % 2)) ||
				(i == 6 && (j % 2)) || (j == 6 && (i % 2)))
				boardScore[i][j] = 3;

			else if ((i == 2 && (j % 2)) || (j == 2 && (i % 2)) ||
				(i == 5 && !(j % 2)) || (j == 5 && !(i % 2)))
				boardScore[i][j] = 2;
			else 
				continue;

		}
	}
		boardScore[4][3] = 1;
		boardScore[3][4] = 1;

	player1 = 12;
	cpu = 12;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Instructions for the game
void Checkers::instructions()
{
	cout << "Welcome to a game of Checkers" << endl;
	cout << "To play this game, simply enter the current row and colum of the current piece" << endl;
	cout << "Then enter the place you would want to move to" << endl;
	cout << "For example: 5 6 4 7" << endl;
	cout << "5 6 is the place of the current piece and 4 7 is the row the piece will move to" << endl;
	cout << "Make sure that the place you want to move to is an empty place with a '/' in that place" << endl;
	cout << "For jump moves, you will enter your current position and enter the place you want to jump to" << endl;
	cout << "P.S. This is a force jump checkers game" << endl;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//returns the player score
const int Checkers::return_player_score() const
{
	return player1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//returns the cpu score
const int Checkers::return_cpu_score() const
{
	return cpu;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//creates the checkers game board
void Checkers::createBoard()
{

	bool placePiece = true;
	for (int i = 0; i < 8; i++)
	{
		if (placePiece && i >= 1)
			placePiece = false;
		else if (!placePiece && i >= 1)
			placePiece = true;

		for (int j = 0; j < 8; j++)
		{
			if (placePiece)
			{
				board[i][j] = " ";
				placePiece = false;
			}

			else
			{
				if ( i >= 0 && i <= 2)
					board[i][j] = "W";
				else if (i >= 5 && i <= 7)
					board[i][j] = "B";
				else
					board[i][j] = "/";
				placePiece = true;
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//shows the game board
void Checkers::showBoard()
{
	int row = 0;
	for (int i = 0; i < 8; ++i)
	{
		std::cout << row << "| ";
		for (int j = 0; j < 8; ++j)
		{
			std::cout << board[i][j] << " | ";
		}
		row++;
		std::cout << std::endl << "---------------------------------" << std::endl;
	}
	for (int i = 0; i < 8; ++i)
	{
		std::cout << "   " << i;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//returns the possible jump moves for the player
std::map<int,std::vector<int> > Checkers::return_jumpMoves()
{
	return mp_jump;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//performs jump moves for the player, for more info => www.youngjoshua.com/projects/checkers
int Checkers::doJump(int prev_row, int prev_col, int new_row, int new_col)
{
	if (jmp_repeat)
	{
		if (save_row != prev_row && save_col != prev_col)
			return -1;
		mp_jump[save_row].push_back(save_col);
		jmp_repeat = false;
	}
	
	if ((new_row < 0 || new_row > 7) || (new_col < 0 || new_col > 7 ))
	{
		return -1;
	}
	if ((prev_row+2 != new_row && prev_row-2 != new_row) ||
		(prev_row < 0 || prev_row > 7))
	{
		return -1;
	}
	if ((prev_col+2 != new_col && prev_col-2 != new_col) || 
		(prev_col < 0 || prev_col > 7))
	{
		return -1;
	}

	std::map<int,std::vector<int> >::const_iterator it = mp_jump.find(prev_row);
	if (it != mp_jump.end())
	{
		for (int ind = 0; ind < it->second.size(); ++ind)
		{
			if (prev_col == it->second[ind])
			{
				int colmax;
				((prev_col > new_col) ? colmax = new_col: colmax = prev_col);
				colmax += 1;
				int i = new_row;
				int j = new_col;
				std::cout << prev_row-1 << std::endl;
				std::cout << (prev_row-1) << std::endl;
				std::cout << board[new_row][new_col] << std::endl;
				if (board[prev_row][prev_col] == "B" && 
						(board[prev_row - 1][colmax] == "W" || board[prev_row-1][colmax] == "C") 
							&& board[new_row][new_col] == "/")
				{
					std::string state = board[prev_row][prev_col];
					board[prev_row][prev_col] = "/";
					board[prev_row-1][colmax] = "/";
					if(new_row == 0)
						board[new_row][new_col] = "K";
					else
						board[new_row][new_col] = state;
					cpu -= 1;
					if ((j == 0 && i != 1 && i != 0) && (board[i-1][j+1] == "W" || board[i-1][j+1] == "C") 
						&& board[i-2][j+2] == "/")
					{
						jmp_repeat = true;
						save_row = new_row;
						save_col = new_col;
						return 0;
					}
					
					else if ((j == 7 && i !=0 && i != 1) && (board[i-1][j-1] == "W" || board[i-1][j-1] == "C")
					 && board[i-2][j-2] == "/")
					{
						jmp_repeat = true;
						save_row = new_row;
						save_col = new_col;
						return 0;

					}
					else
					{
						if ((j != 1 && j != 0 && i != 1 && i != 0) && (board[i-1][j-1] == "W" || board[i-1][j-1] == "C") 
							&& board[i-2][j-2] == "/")
						{
							jmp_repeat = true;
							save_row = new_row;
							save_col = new_col;
							return 0;

						}

						if ((j != 6 && j != 7 && i != 1 && i != 0) && (board[i-1][j+1] == "W" || board[i-1][j+1] == "C") 
							&& board[i-2][j+2] == "/")
						{
							jmp_repeat = true;
							save_row = new_row;
							save_col = new_col;
							return 0;

						}
					}
					mp_jump.clear();
					return 1;
					
				}
				else if (board[prev_row][prev_col] == "K" && 
					((prev_row != 0 && prev_row != 1 && (board[prev_row - 1][colmax] == "W" || board[prev_row - 1][colmax] == "C"))
						|| (prev_row != 7&& prev_row != 6 && (board[prev_row + 1][colmax] == "W" || board[prev_row + 1][colmax] == "C"))))
				{
					if (prev_row-2 == new_row)
					{
						board[prev_row-1][colmax] = "/";
						board[prev_row][prev_col] = "/";
					}
					else
					{
						board[prev_row+1][colmax] = "/";
						board[prev_row][prev_col] = "/";
					}
					board[new_row][new_col] = "K";
					cpu -= 1;
					if ((j == 0 && i == 7) && (board[i-1][j+1] == "C" ||board[i-1][j+1] == "W") && (board[i-2][j+2] == "/"))
					{
						jmp_repeat = true;
						save_row = new_row;
						save_col = new_col;
						return 0;

					}

					else if (((j == 0 || j == 1) && i != 7 && i != 6 && i != 1 && i != 0) && 
						(((board[i-1][j+1] == "C" ||board[i-1][j+1] == "W") && (board[i-2][j+2] == "/")) ||
							((board[i+1][j+1] == "C" ||board[i+1][j+1] == "W") && (board[i+2][j+2] == "/"))))
					{
						jmp_repeat = true;
						save_row = new_row;
						save_col = new_col;
						return 0;

					}

					else if ((j == 7 && i == 0) && (board[i+1][j-1] == "C" ||board[i+1][j-1] == "W") && (board[i+2][j-2] == "/"))
					{
						jmp_repeat = true;
						save_row = new_row;
						save_col = new_col;
						return 0;

					}

					else if (((j == 7 || j == 6)&& i != 0 && i != 7 && i != 6 && i != 1) && 
						(((board[i+1][j-1] == "C" ||board[i+1][j-1] == "W") && (board[i+2][j-2] == "/")) ||
							(board[i-1][j-1] == "C" ||board[i-1][j-1] == "W") && (board[i-2][j-2] == "/")))
					{
							jmp_repeat = true;
							save_row = new_row;
							save_col = new_col;
							return 0;

					}

					else if (((i == 7 || i == 6)&& j != 0 && j != 7 && j != 6 && j != 1) && 
						(((board[i-1][j+1] == "C" ||board[i-1][j+1] == "W") && (board[i-2][j+2] == "/")) ||
							(board[i-1][j-1] == "C" ||board[i-1][j-1] == "W") && (board[i-2][j-2] == "/")))
					{
							jmp_repeat = true;
							save_row = new_row;
							save_col = new_col;
							return 0;

					}

					else if (((i == 0 || i == 1)&& j != 0 && j != 7 && j != 6 && j != 1) && 
						(((board[i+1][j-1] == "C" || board[i+1][j-1] == "W") && (board[i+2][j-2] == "/")) ||
							(board[i+1][j+1] == "C" || board[i+1][j+1] == "W") && (board[i+2][j+2] == "/")))
					{
							jmp_repeat = true;
							save_row = new_row;
							save_col = new_col;
							return 0;

					}

					else if (((i != 0 && i != 1 && i != 6 && i != 7 && j != 1 && j != 0 && j != 6 && j != 7)) &&
						((((board[i+1][j-1] == "C" ||board[i+1][j-1] == "W") && (board[i+2][j-2] == "/")) ||
							(board[i+1][j+1] == "C" ||board[i+1][j+1] == "W") && (board[i+2][j+2] == "/")) ||
								((((board[i-1][j+1] == "C" ||board[i-1][j+1] == "W") && (board[i-2][j+2] == "/")) ||
									(board[i-1][j-1] == "C" ||board[i-1][j-1] == "W") && (board[i-2][j-2] == "/")))))
					{
							jmp_repeat = true;
							save_row = new_row;
							save_col = new_col;
							return 0;

					}
					mp_jump.clear();
					return 1;
				}
			}
		}
	}
	return -1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//checks for force jump
bool Checkers::P1_forcejump()
{
	return forceJump(board);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//checks for force jump
bool Checkers::forceJump(std::string**& c_board)
{
	std::string players = "B";
	std::string cpus = "W";

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (c_board[i][j] == players && i != 0)
			{
				if ((j == 0 && i != 1 && i != 0) && (c_board[i-1][j+1] == cpus || c_board[i-1][j+1] == "C") 
					&& c_board[i-2][j+2] == "/")
					mp_jump[i].push_back(j);
				
				else if ((j == 7 && i !=0 && i != 1) && (c_board[i-1][j-1] == cpus || c_board[i-1][j-1] == "C")
				 && c_board[i-2][j-2] == "/")
					mp_jump[i].push_back(j);
				else
				{
					if ((j != 1 && j != 0 && i != 1 && i != 0) && (c_board[i-1][j-1] == "W" || c_board[i-1][j-1] == "C") 
						&& c_board[i-2][j-2] == "/")
						mp_jump[i].push_back(j);

					if ((j != 6 && j != 7 && i != 1 && i != 0) && (c_board[i-1][j+1] == "W" || c_board[i-1][j+1] == "C") 
						&& c_board[i-2][j+2] == "/")
						mp_jump[i].push_back(j);
				}
		
			}

			else if (c_board[i][j] == "K")
			{
				if ((j == 0 && i == 7) && (c_board[i-1][j+1] == "C" ||c_board[i-1][j+1] == "W") && (c_board[i-2][j+2] == "/"))
					mp_jump[i].push_back(j);

				else if (((j == 0 || j == 1) && i != 7 && i != 6 && i != 1 && i != 0) && 
					(((c_board[i-1][j+1] == "C" ||c_board[i-1][j+1] == "W") && (c_board[i-2][j+2] == "/")) ||
						((c_board[i+1][j+1] == "C" ||c_board[i+1][j+1] == "W") && (c_board[i+2][j+2] == "/"))))
					mp_jump[i].push_back(j);

				else if ((j == 7 && i == 0) && (c_board[i+1][j-1] == "C" ||c_board[i+1][j-1] == "W") && (c_board[i+2][j-2] == "/"))
					mp_jump[i].push_back(j);

				else if (((j == 7 || j == 6)&& i != 0 && i != 7 && i != 6 && i != 1) && 
					(((c_board[i+1][j-1] == "C" ||c_board[i+1][j-1] == "W") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i-1][j-1] == "C" ||c_board[i-1][j-1] == "W") && (c_board[i-2][j-2] == "/")))
					mp_jump[i].push_back(j);

				else if (((i == 7 || i == 6)&& j != 0 && j != 7 && j != 6 && j != 1) && 
					(((c_board[i-1][j+1] == "C" ||c_board[i-1][j+1] == "W") && (c_board[i-2][j+2] == "/")) ||
						(c_board[i-1][j-1] == "C" ||c_board[i-1][j-1] == "W") && (c_board[i-2][j-2] == "/")))
					mp_jump[i].push_back(j);

				else if (((i == 0 || i == 1)&& j != 0 && j != 7 && j != 6 && j != 1) && 
					(((c_board[i+1][j-1] == "C" ||c_board[i+1][j-1] == "W") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i+1][j+1] == "C" ||c_board[i+1][j+1] == "W") && (c_board[i+2][j+2] == "/")))
					mp_jump[i].push_back(j);

				else if (((i != 0 && i != 1 && i != 6 && i != 7 && j != 1 && j != 0 && j != 6 && j != 7)) &&
					((((c_board[i+1][j-1] == "C" ||c_board[i+1][j-1] == "W") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i+1][j+1] == "C" ||c_board[i+1][j+1] == "W") && (c_board[i+2][j+2] == "/")) ||
							((((c_board[i-1][j+1] == "C" ||c_board[i-1][j+1] == "W") && (c_board[i-2][j+2] == "/")) ||
								(c_board[i-1][j-1] == "C" ||c_board[i-1][j-1] == "W") && (c_board[i-2][j-2] == "/")))))
					mp_jump[i].push_back(j);



			}
		}
	}

	if (mp_jump.size() > 0)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Performs a jump move for the computer, for more info => www.youngjoshua.com/projects/checkers
void Checkers::CPUdoJump(int& prevrow,int& prevcolum,std::map<int,std::vector<int> >& cpu_jump)
{
	int bestoutcome = -1000;
	int saveScore = player1;
	int save_score_cpu = cpu;
	int depth = level;
	bool isKing = false;
	
	std::vector<std::pair<int,int> > temp_jump;
	std::map<int,std::vector<int> >::const_iterator it;
	for (it = cpu_jump.begin();it != cpu_jump.end();it++)
	{
		int r = it->first;

		for (int i = 0; i < it->second.size(); ++i)
		{
			
			int c = it->second[i];
			string** save_board = copyBoard(board);
			if (save_board[r][c] == "C")
				isKing = true;
			keepJump(r,c,temp_jump,save_board,isKing);
			if (cpu_jump.size() > 1)
			{
				int val = minimaxAlg(true,save_board,depth);
				if (val > bestoutcome)
				{
					prevrow = r;
					prevcolum = c;
					save_jump = temp_jump;
				}
				
			}

			else
			{
				prevrow = r;
				prevcolum = c;
				save_jump = temp_jump;
			}
			isKing = false;
			temp_jump.clear();
			player1 = saveScore;
			cpu = save_score_cpu;
			destroy(save_board);
		}
	}
	cpu_jump.clear();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//performs a jump move for the computer and checks to see if another jump is possible, for more info => www.youngjoshua.com
void Checkers::keepJump(int r,int c,std::vector<std::pair<int,int> >& save_jmp,std::string**& c_board,bool isKing)
{
	if (!isKing)
	{
		int bestVal = -1000;
		bool leftSide = false;
		bool rightSide = false;
		string** tempBoard;
		string** new_tempBoard;
		std::vector<std::pair<int,int> > tempJump;
		std::vector<std::pair<int,int> > newtempJump;
		//delete both after
		int p1_save;
		int new_p1Save;
		if ((c == 0 && r != 7) && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
		{
		//	cout << "FAR LEFT JUMP" << endl;
			c_board[r+1][c+1] = "/";
			c_board[r][c] = "/";
			if ((r+2) == 7)
			{
				c_board[r+2][c+2] = "C";
				isKing = true;
			}
			else
			{
				c_board[r+2][c+2] = "W";
				isKing = false;
			}
			player1 -= 1;
			save_jmp.push_back(std::make_pair(r+2,c+2));
			keepJump(r+2,c+2,save_jmp,c_board,isKing);
		}

		else if ((c == 7 && r != 6)&& (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
		{
		//	cout << "FAR RIGHT JUMP" << endl;
			c_board[r+1][c-1] = "/";
			c_board[r][c] = "/";
			if ((r+2) == 7)
			{
				c_board[r+2][c-2] = "C";
				isKing = true;
			}
			else
			{
				c_board[r+2][c-2] = "W";
				isKing = false;
			}
			player1 -= 1;
			save_jmp.push_back(std::make_pair(r+2,c-2));
			keepJump(r+2,c-2,save_jmp,c_board,isKing);
		}

		else
		{
			if ((c != 1 && c != 0 && r != 7 && r != 6) && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
			{
		//		cout << "MIDDLE JUMP AND NOT 1" << endl;
				tempBoard = copyBoard(c_board);
				p1_save = player1;
				tempJump = save_jmp;
				c_board[r+1][c-1] = "/";
				c_board[r][c] = "/";
				if ((r+2) == 7)
				{
					c_board[r+2][c-2] = "C";
					isKing = true;
				}
				else
				{
					c_board[r+2][c-2] = "W";
					isKing = false;
				}
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r+2,c-2));
				keepJump(r+2,c-2,save_jmp,c_board,isKing);
				leftSide = true;
				int value = EvaluateBoard(c_board);
				if (value > bestVal)
					bestVal = value;
				c_board[r+1][c-1] = "B";
				c_board[r][c] = "W";
			}

			if ((c != 6 && c != 7 && r != 7 && r != 6) && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
		//		cout << "MIDDLE JUMP AND NOT 6" << endl;
				if (leftSide)
				{
					new_tempBoard = copyBoard(c_board);
					c_board = copyBoard(tempBoard);
					newtempJump = save_jmp;
					save_jmp = tempJump;
					new_p1Save = player1;
					player1 = p1_save;
					rightSide = true;
					c_board[r+1][c+1] = "/";
					c_board[r][c] = "/";
					if ((r+2) == 7)
					{
						c_board[r+2][c+2] = "C";
						isKing = true;
					}
					else
					{
						c_board[r+2][c+2] = "W";
						isKing = false;
					}
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r+2,c+2));
					keepJump(r+2,c+2,save_jmp,c_board,isKing);
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
					{
						c_board = copyBoard(new_tempBoard);
						save_jmp = newtempJump;
						player1 = new_p1Save;
					}

				}

				else
				{
					c_board[r+1][c+1] = "/";
					c_board[r][c] = "/";
					if ((r+2) == 7)
					{
						c_board[r+2][c+2] = "C";
						isKing = true;
					}
					else
					{
						c_board[r+2][c+2] = "W";
						isKing = false;
					}
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r+2,c+2));
					keepJump(r+2,c+2,save_jmp,c_board,isKing);

				}
			}
		}

		if (leftSide)
			destroy(tempBoard);
		if (rightSide)
			destroy(new_tempBoard);
	}

	else
	{
		int bestVal = -1000;
		bool leftside_upper = false;
		bool leftside_lower = false;
		string** leftside_upper_board;
		string** leftside_lower_board;
		std::vector<std::pair<int,int> > leftsideupperJump;
		std::vector<std::pair<int,int> > leftsidelowerJump;
		int p1_left_upper;
		int p1_left_lower;

		bool rightside_upper = false;
		bool rightside_lower = false;
		string** rightside_upper_board;
		string** rightside_lower_board;
		std::vector<std::pair<int,int> > rightsideupperJump;
		std::vector<std::pair<int,int> > rightsidelowerJump;
		int p1_right_upper;
		int p1_right_lower;

		bool top_right = false;
		bool top_left = false;
		string** top_right_board;
		string** top_left_board;
		std::vector<std::pair<int,int> > toprightJump;
		std::vector<std::pair<int,int> > topleftJump;
		int p1_top_right;
		int p1_top_left;

		bool bottom_right = false;
		bool bottom_left = false;
		string** bottom_right_board;
		string** bottom_left_board;
		std::vector<std::pair<int,int> > bottomrightJump;
		std::vector<std::pair<int,int> > bottomleftJump;
		int p1_bottom_right;
		int p1_bottom_left;

		bool middle = false;
		bool middle_temp = false;
		string** middle_board;
		string** middle_board_temp;
		std::vector<std::pair<int,int> > middleJump;
		std::vector<std::pair<int,int> > middletempJump;
		int p1_middle;
		int p1_middle_temp;


		if (c == 0 && r == 7 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
		{
			c_board[r][c] = "/";
			c_board[r-1][c+1] = "/";
			c_board[r-2][c+2] = "C";
			player1 -= 1;
			save_jmp.push_back(std::make_pair(r-2,c+2));
			keepJump(r-2,c+2,save_jmp,c_board,true);
		}

		else if (r == 0 && c == 7 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
		{
			c_board[r][c] = "/";
			c_board[r+1][c-1] = "/";
			c_board[r+2][c-2] = "C";
			player1 -= 1;
			save_jmp.push_back(std::make_pair(r+2,c-2));
			keepJump(r+2,c-2,save_jmp,c_board,true);
		}

		else if ((c == 0 || c == 1) && r != 7)
		{
			if (r == 1 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r+2,c+2));
				keepJump(r+2,c+2,save_jmp,c_board,true);
			}

			else if (r == 6 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c+1] = "/";
				c_board[r-2][c+2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r-2,c+2));
				keepJump(r-2,c+2,save_jmp,c_board,true);
			}

			else if (r == 0 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r+2,c+2));
				keepJump(r+2,c+2,save_jmp,c_board,true);
			}

			else
			{
				if (r != 1 && r != 0 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
				{
					leftside_upper_board = copyBoard(c_board);
					p1_left_upper = player1;
					leftsideupperJump = save_jmp;
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r-2,c+2));
					keepJump(r-2,c+2,save_jmp,c_board,true);
					leftside_upper = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
				if (r != 6 && r != 7 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
				{
					if (leftside_upper)
					{
						leftside_lower_board = copyBoard(c_board);
						c_board = copyBoard(leftside_upper_board);
						p1_left_lower = player1;
						player1 = p1_left_upper;
						leftsidelowerJump = save_jmp;
						save_jmp = leftsideupperJump;
						
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "C";
						player1 -= 1;
						save_jmp.push_back(std::make_pair(r+2,c+2));
						keepJump(r+2,c+2,save_jmp,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value < bestVal)
						{
							c_board = copyBoard(leftside_lower_board);
							player1 = p1_left_lower;
							save_jmp = leftsidelowerJump;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "C";
						player1 -= 1;
						save_jmp.push_back(std::make_pair(r+2,c+2));
						keepJump(r+2,c+2,save_jmp,c_board,true);
					}
				}
			}
		}
//fix the recursion to match the jump , the rows and columns

		else if ((c == 7 || c == 6) && r != 0)
		{
			if (r == 6 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r-2,c-2));
				keepJump(r-2,c-2,save_jmp,c_board,true);
			}

			else if (r == 7 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r-2,c-2));
				keepJump(r-2,c-2,save_jmp,c_board,true);
			}

			else if (r == 1 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c-1] = "/";
				c_board[r+2][c-2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r+2,c-2));
				keepJump(r+2,c-2,save_jmp,c_board,true);
			}

			else
			{
				if (r != 1 && r != 0 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
				{
					rightside_upper_board = copyBoard(c_board);
					p1_right_upper = player1;
					rightsideupperJump = save_jmp;
					c_board[r][c] = "/";
					c_board[r-1][c-1] = "/";
					c_board[r-2][c-2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r-2,c-2));
					keepJump(r-2,c-2,save_jmp,c_board,true);
					rightside_upper = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
				if (r != 6 && r != 7 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
				{
					if (rightside_upper)
					{
						rightside_lower_board = copyBoard(c_board);
						c_board = copyBoard(rightside_upper_board);
						p1_right_lower = player1;
						player1 = p1_right_upper;
						rightside_lower = true;
						rightsidelowerJump = save_jmp;
						save_jmp = rightsideupperJump;
						
						c_board[r][c] = "/";
						c_board[r+1][c-1] = "/";
						c_board[r+2][c-2] = "C";
						player1 -= 1;
						save_jmp.push_back(std::make_pair(r+2,c-2));
						keepJump(r+2,c-2,save_jmp,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value < bestVal)
						{
							c_board = copyBoard(rightside_lower_board);
							player1 = p1_right_lower;
							save_jmp = rightsidelowerJump;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c-1] = "/";
						c_board[r+2][c-2] = "C";
						player1 -= 1;
						save_jmp.push_back(std::make_pair(r+2,c-2));
						keepJump(r+2,c-2,save_jmp,c_board,true);
					}
				}
			}
		}

		else if ((r == 0 || r == 1) && c != 7)
		{
			if (c == 1 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r+2,c+2));
				keepJump(r+2,c+2,save_jmp,c_board,true);
			}

			else if (c == 0 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r+2,c+2));
				keepJump(r+2,c+2,save_jmp,c_board,true);
			}

			else if (c == 6 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c-1] = "/";
				c_board[r+2][c-2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r+2,c-2));
				keepJump(r+2,c-2,save_jmp,c_board,true);
			}

			else
			{
				if (c != 1 && c != 0 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
				{
					top_left_board = copyBoard(c_board);
					p1_top_left = player1;
					topleftJump = save_jmp;

					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r+2,c-2));
					keepJump(r+2,c-2,save_jmp,c_board,true);
					top_left = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
				if (c != 7 && c != 6 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
				{
					if (top_left)
					{
						top_right_board = copyBoard(c_board);
						c_board = copyBoard(top_left_board);
						p1_top_right = player1;
						player1 = p1_top_left;
						top_right = true;
						toprightJump = save_jmp;
						save_jmp = topleftJump;
						
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "C";
						player1 -= 1;
						save_jmp.push_back(std::make_pair(r+2,c+2));
						keepJump(r+2,c+2,save_jmp,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value < bestVal)
						{
							c_board = copyBoard(top_right_board);
							save_jmp = toprightJump;
							player1 = p1_top_right;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "C";
						player1 -= 1;
						save_jmp.push_back(std::make_pair(r+2,c+2));
						keepJump(r+2,c+2,save_jmp,c_board,true);
					}
				}
			}
		}

		else if ((r == 7 || r == 6) && c != 0)
		{
			if (c == 6 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r-2,c-2));
				keepJump(r-2,c-2,save_jmp,c_board,true);
			}

			else if (c == 1 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c+1] = "/";
				c_board[r-2][c+2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r-2,c+2));
				keepJump(r-2,c+2,save_jmp,c_board,true);
			}

			else if (c == 7 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r-2,c-2));
				keepJump(r-2,c-2,save_jmp,c_board,true);
			}

			else
			{
				if (c != 0 && c != 1 &&(c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
				{
					bottom_left_board = copyBoard(c_board);
					p1_bottom_left = player1;
					bottomleftJump = save_jmp;

					c_board[r][c] = "/";
					c_board[r-1][c-1] = "/";
					c_board[r-2][c-2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r-2,c-2));
					keepJump(r-2,c-2,save_jmp,c_board,true);
					bottom_left = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
				if (c != 6 && c != 7 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
				{
					if (bottom_left)
					{
						bottom_right_board = copyBoard(c_board);
						c_board = copyBoard(bottom_left_board);
						p1_bottom_right = player1;
						player1 = p1_bottom_left;
						bottom_right = true;
						bottomrightJump = save_jmp;
						save_jmp = bottomleftJump;
						
						c_board[r][c] = "/";
						c_board[r-1][c+1] = "/";
						c_board[r-2][c+2] = "C";
						player1 -= 1;
						save_jmp.push_back(std::make_pair(r-2,c+2));
						keepJump(r-2,c+2,save_jmp,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value < bestVal)
						{
							c_board = copyBoard(bottom_right_board);
							save_jmp = bottomrightJump;
							player1 = p1_bottom_right;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r-1][c+1] = "/";
						c_board[r-2][c+2] = "C";
						player1 -= 1;
						save_jmp.push_back(std::make_pair(r-2,c+2));
						keepJump(r-2,c+2,save_jmp,c_board,true);
					}
				}
			}
		}
		else if ((r != 0 && r != 1 && r != 6 && r != 7) && (c != 0 && c != 1 && c != 6 && c != 7))
		{
			if ((c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				middle_board = copyBoard(c_board);
				p1_middle = player1;
				middleJump = save_jmp;

				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				save_jmp.push_back(std::make_pair(r-2,c-2));
				keepJump(r-2,c-2,save_jmp,c_board,true);
				middle = true;
				int value = EvaluateBoard(c_board);
				if (value > bestVal)
					bestVal = value;
			}

			if ((c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					p1_middle_temp = player1;
					player1 = p1_middle;
					middle_temp = true;
					middletempJump = save_jmp;
					save_jmp = middleJump;
					
					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r+2,c-2));
					keepJump(r+2,c-2,save_jmp,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						player1 = p1_bottom_right;
						save_jmp = middletempJump;
					}
				}

				else
				{
					middle_board = copyBoard(c_board);
					p1_middle = player1;
					middleJump = save_jmp;


					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r+2,c-2));
					keepJump(r+2,c-2,save_jmp,c_board,true);
					middle = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
			}

			if ((c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					p1_middle_temp = player1;
					player1 = p1_middle;
					middle_temp = true;
					save_jmp = middleJump;
					middletempJump = save_jmp;
					
					c_board[r][c] = "/";
					c_board[r+1][c+1] = "/";
					c_board[r+2][c+2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r+2,c+2));
					keepJump(r+2,c+2,save_jmp,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						player1 = p1_bottom_right;
						save_jmp = middletempJump;
					}
				}

				else
				{
					middle_board = copyBoard(c_board);
					p1_middle = player1;
					middleJump = save_jmp;

					c_board[r][c] = "/";
					c_board[r+1][c+1] = "/";
					c_board[r+2][c+2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r+2,c+2));
					keepJump(r+2,c+2,save_jmp,c_board,true);
					middle = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
			}

			if ((c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					p1_middle_temp = player1;
					player1 = p1_middle;
					middle_temp = true;
					save_jmp = middleJump;
					middletempJump = save_jmp;
					
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r-2,c+2));
					keepJump(r-2,c+2,save_jmp,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						player1 = p1_bottom_right;
						save_jmp = middletempJump;
					}
				}

				else
				{
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "C";
					player1 -= 1;
					save_jmp.push_back(std::make_pair(r-2,c+2));
					keepJump(r-2,c+2,save_jmp,c_board,true);
				}
			}
		}

	if (leftside_upper)
		destroy(leftside_upper_board);
	if (leftside_lower)
		destroy(leftside_lower_board);
	if (rightside_upper)
		destroy(rightside_upper_board);
	if (rightside_lower)
		destroy(rightside_lower_board);
	if (top_right)
		destroy(top_right_board);
	if (top_left)
		destroy(top_left_board);
	if (bottom_right)
		destroy(bottom_right_board);
	if (bottom_left)
		destroy(bottom_left_board);
	if (middle)
		destroy(middle_board);
	if (middle_temp)
		destroy(middle_board_temp);

	}

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//checks for force jump for the computer
bool Checkers::CPUforceJump(std::string**& c_board,std::map<int,std::vector<int> >& cpu_jump)
{
	//change the names
	std::string players = "B";
	std::string cpus = "W";
	int counts = 0;


	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (c_board[i][j] == cpus && i != 7 && i != 6)
			{
				if (j == 0 && (c_board[i+1][j+1] == players || c_board[i+1][j+1] == "K") && c_board[i+2][j+2] == "/")
					cpu_jump[i].push_back(j);

				else if ((j == 7) && (c_board[i+1][j-1] == players || c_board[i+1][j-1] == "K") && c_board[i+2][j-2] == "/")
					cpu_jump[i].push_back(j);
				else
				{
					if ((j != 1 && j != 0 ) && (c_board[i+1][j-1] == players || c_board[i+1][j-1] == "K") && c_board[i+2][j-2] == "/")
						cpu_jump[i].push_back(j);

					if ((j != 6 && j != 7 ) && (c_board[i+1][j+1] == "B" || c_board[i+1][j+1] == "K") && c_board[i+2][j+2] == "/")
						cpu_jump[i].push_back(j);
				}
			}

			else if (c_board[i][j] == "C")
			{
				if ((j == 0 && i == 7) && (c_board[i-1][j+1] == "K" ||c_board[i-1][j+1] == "B") && (c_board[i-2][j+2] == "/"))
					cpu_jump[i].push_back(j);

				else if (((j == 0 || j == 1) && i != 7 && i != 6 && i != 1 && i != 0) && 
					(((c_board[i-1][j+1] == "K" ||c_board[i-1][j+1] == "B") && (c_board[i-2][j+2] == "/")) ||
						((c_board[i+1][j+1] == "K" ||c_board[i+1][j+1] == "B") && (c_board[i+2][j+2] == "/"))))
					cpu_jump[i].push_back(j);

				else if ((j == 7 && i == 0) && (c_board[i+1][j-1] == "K" ||c_board[i+1][j-1] == "B") && (c_board[i+2][j-2] == "/"))
					cpu_jump[i].push_back(j);

				else if (((j == 7 || j == 6)&& i != 0 && i != 7 && i != 6 && i != 1) && 
					(((c_board[i+1][j-1] == "K" ||c_board[i+1][j-1] == "B") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i-1][j-1] == "K" ||c_board[i-1][j-1] == "B") && (c_board[i-2][j-2] == "/")))
					cpu_jump[i].push_back(j);

				else if (((i == 7 || i == 6)&& j != 0 && j != 7 && j != 1) && 
					(((j != 6) && ((c_board[i-1][j+1] == "K" ||c_board[i-1][j+1] == "B") && (c_board[i-2][j+2] == "/"))) ||
						(c_board[i-1][j-1] == "K" ||c_board[i-1][j-1] == "B") && (c_board[i-2][j-2] == "/")))
					cpu_jump[i].push_back(j);

				else if (((i == 0 || i == 1)&& j != 0 && j != 7 && j != 6 && j != 1) && 
					(((c_board[i+1][j-1] == "K" ||c_board[i+1][j-1] == "B") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i+1][j+1] == "K" ||c_board[i+1][j+1] == "B") && (c_board[i+2][j+2] == "/")))
					cpu_jump[i].push_back(j);

				else if (((i != 0 && i != 1 && i != 6 && i != 7 && j != 1 && j != 0 && j != 6 && j != 7)) &&
					((((c_board[i+1][j-1] == "K" ||c_board[i+1][j-1] == "B") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i+1][j+1] == "K" ||c_board[i+1][j+1] == "B") && (c_board[i+2][j+2] == "/")) ||
							((((c_board[i-1][j+1] == "K" ||c_board[i-1][j+1] == "B") && (c_board[i-2][j+2] == "/")) ||
								(c_board[i-1][j-1] == "K" ||c_board[i-1][j-1] == "B") && (c_board[i-2][j-2] == "/")))))
					cpu_jump[i].push_back(j);



			}			
		}
	}


	if (cpu_jump.size() > 0)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//performs a player input
bool Checkers::playerMove(int rw,int col, int rwm, int colm)
{
	if (rw == rwm || col == colm)
		return false;
	//you cannot move
	//in the same row
	//or column



//KING
	if (board[rw][col] == "K")
	{
		if (board[rwm][colm] == "W" || board[rwm][colm] == " " || board[rwm][colm] == "C" ||board[rwm][colm] == "B" )
			return false;
		int colmax;
		((col > colm) ? colmax = colm: colmax = col);
		unsigned int attup = rw - 2;
		unsigned int attdown = rw + 2;
		unsigned int jumup = rw - 1;
		unsigned int jumdown = rw + 1;

		//moving one step forward for king
		if (rwm == jumup)
		{
			//normal checkers move
			int colnewlow = col - 1;
			int colnewmax = col + 1;
			if (col == 7 || col == 0)
			{
				if (col == 0)
				{
					if ( colnewmax != colm)
						return false;
						//trying to move beyond scope
					else
					{
						board[rwm][colm] = "K";
						board[rw][col] = "/";
					}
				}
				else
				{
					if (colnewlow != colm)
						return false;
						//trying to move beyond scope
					else
					{
						board[rwm][colm] = "K";
						board[rw][col] = "/";
					}
				}
			}

			else
			{
				if (colm != colnewmax && colm != colnewlow)
					return false;
				//out of scope
				board[rwm][colm] = "K";
				board[rw][col] = "/";
			}
		}

		//moving backwards for king
		else if (rwm == jumdown)
		{
			//normal checkers move
			int colnewlow = col - 1;
			int colnewmax = col + 1;
			if (col == 7 || col == 0)
			{
				if (col == 0)
				{
					if ( colnewmax != colm)
						return false;
						//trying to move beyond scope
					else
					{
						board[rwm][colm] = "K";
						board[rw][col] = "/";
					}
				}
				else
				{
					if (colnewlow != colm)
					{
						return false;
						//trying to move beyond scope
					}
					else
					{
						board[rwm][colm] = "K";
						board[rw][col] = "/";
					}
				}
			}

			else
			{
				if (colm != colnewmax && colm != colnewlow)
					return false;
				//out of scope
				board[rwm][colm] = "K";
				board[rw][col] = "/";
			}
		}

		else
			return false;




	}

//NORMAL PLAYER
	else if (board[rw][col] == "B")
	{
		if (rwm > rw)
			return false;
		//a regular chip cannot move backwards

		if (board[rwm][colm] == "W" || board[rwm][colm] == " ")
			return false;
			//to indicate that the place is taken or not available

		//the position is free
		//we need to check whether the move was an attack
		int colmax;
		((col > colm) ? colmax = colm: colmax = col);
		colmax = colmax + 1;
		unsigned int att = rw - 2;
		unsigned int jum = rw - 1;
		if (rwm == att)
		{
			//player has launched an attack
			if (board[rwm + 1][colmax] == "W")
			{
				board[rwm + 1][colmax] = "/";
				board[rw][col] = "/";
				if (rwm == 0)
					board[rwm][colm] = "K";
				else
					board[rwm][colm] = "B";
				cpu -= 1;
			}

			else
				return false;
				//player is trying to cheat
			
		}

		else if (rwm == jum)
		{
			//normal checkers move
			int colnewlow = col - 1;
			int colnewmax = col + 1;
			if (col == 7 || col == 0)
			{
				if (col == 0)
				{
					if ( colnewmax != colm)
					{
						return false;
						//trying to move beyond scope
					}
					else
					{
						if (rwm == 0)
							board[rwm][colm] = "K";
						else
							board[rwm][colm] = "B";
						board[rw][col] = "/";
					}
				}
				else
				{
					if (colnewlow != colm)
					{
						return false;
						//trying to move beyond scope
					}
					else
					{
						if (rwm == 0)
							board[rwm][colm] = "K";
						else
							board[rwm][colm] = "B";
						board[rw][col] = "/";
					}
				}
			}

			else
			{
				int colnewlow = col - 1;
				int colnewmax = col + 1;
				if (colm != colnewmax && colm != colnewlow)
					return false;
				//out of scope
				if (rwm == 0)
					board[rwm][colm] = "K";
				else
					board[rwm][colm] = "B";
				board[rw][col] = "/";



			}
		}
	}

	else
		return false;
		//trying to move a piece that isnt theirs
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//this is where the computer starts a move and performs a jump move if possible, for mor info => www.youngjoshua.com/projects/checkers
void Checkers::CpuMove(bool& game_over)
{
	int row = -1;
	int colum = -1;
	int prevrow = -1;
	int prevcolum = -1;
	bool isJump = false;
	findMovePosition(row,colum,prevrow,prevcolum,isJump,game_over);
	if (isJump)
	{
		int colmax;
		int r;
		int c;
		int disp_row = prevrow;
		int disp_col = prevcolum;
		for (int i = 0; i < save_jump.size(); ++i)
		{
			((prevcolum > save_jump[i].second) ? colmax = save_jump[i].second: colmax = prevcolum);
			colmax = colmax + 1;
			r = save_jump[i].first;
			c = save_jump[i].second;
			std::string state = board[prevrow][prevcolum];
			if (board[prevrow][prevcolum] == "C")
			{
				if (prevrow-2 == r)
				{
					board[prevrow-1][colmax] = "/";
					board[prevrow][prevcolum] = "/";
				}
				else
				{
					board[prevrow+1][colmax] = "/";
					board[prevrow][prevcolum] = "/";
				}
			}
			else
			{
				board[prevrow+1][colmax] = "/";
				board[prevrow][prevcolum] = "/";
			}
			if (r == 7)
				board[r][c] = "C";
			else
				board[r][c] = state;
			prevrow = save_jump[i].first;
			prevcolum = save_jump[i].second;
			player1 -= 1;
			
		}
		cout << "CPU moved from " << disp_row << " " << disp_col << " to " << r <<" " << c << endl;
		save_jump.clear();
		return;
	}
	if (game_over)
		return;



	std::string state = board[prevrow][prevcolum];
	if (row == 7)
		board[row][colum] = "C";
	else
		board[row][colum] = state;
	board[prevrow][prevcolum] = "/";
	cout << "CPU moved from " << prevrow << " " << prevcolum << " to "<< row <<" " << colum << endl;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//checks to see if there are any more moves left
std::map<int,std::vector<int> > Checkers::returnMoves()
{
	return getMoves(board,true);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//gets the possible available moves
std::map<int,std::vector<int> > Checkers::getMoves(std::string**& c_board,bool isplayer)
{
	std::map<int,std::vector<int> > Normal_move;
	if (!isplayer)
	{
		for(int i = 0; i < 8; i ++)
		{
			for (int j = 0; j < 8; j ++)
			{
				if (c_board[i][j] == "W")
				{
					if (j == 0 && i != 7 && c_board[i+1][j+1] == "/")
						Normal_move[i].push_back(j);

					else if (j == 7 && i != 7 && c_board[i+1][j-1] == "/")
						Normal_move[i].push_back(j);

					else if (i != 7)
					{
						//need to check both left and right side
						if ((j != 7 && c_board[i+1][j+1] == "/") || (j != 0 && c_board[i+1][j-1] == "/"))
							Normal_move[i].push_back(j);
					}
				}

				else if (c_board[i][j] == "C")
				{
					if ((j == 0 && i == 7 && c_board[i-1][j+1] == "/" )||
						(j == 0 && i != 7 && (c_board[i+1][j+1] == "/" || c_board[i-1][j+1] == "/")))
						Normal_move[i].push_back(j);

					else if ((j == 7 && i == 0 && c_board[i+1][j-1] == "/" )||
						(j == 7 && i != 0 && (c_board[i-1][j-1] == "/" || c_board[i+1][j-1] == "/")))
						Normal_move[i].push_back(j);

					else if (i == 7 && j != 0 && (c_board[i-1][j-1] == "/" || c_board[i-1][j+1] == "/"))
						Normal_move[i].push_back(j);

					else if (i == 0 && j != 7 && (c_board[i+1][j-1] == "/" || c_board[i+1][j+1] == "/"))
						Normal_move[i].push_back(j);
					else if (i != 0 && i != 7 && j != 0 && j != 7)
					{
						if(c_board[i+1][j+1] == "/" || c_board[i-1][j-1] == "/" || c_board[i+1][j-1] == "/"
							|| c_board[i-1][j+1] == "/")
							Normal_move[i].push_back(j);
					}
				}
				
			}
		}
		return Normal_move;
	}

	else
	{
		for(int i = 0; i < 8; i ++)
		{
			for (int j = 0; j < 8; j ++)
			{
				if (c_board[i][j] == "B")
				{
					if (j == 0 && i != 0 && c_board[i-1][j+1] == "/")
						Normal_move[i].push_back(j);

					else if (j == 7 && i != 0 && c_board[i-1][j-1] == "/")
						Normal_move[i].push_back(j);

					else if (i != 0)
					{
						//need to check both left and right side
						if ((j != 7 && c_board[i-1][j+1] == "/") || (j != 0 && c_board[i-1][j-1] == "/"))
							Normal_move[i].push_back(j);
					}
				}


				else if (c_board[i][j] == "K")
				{
					if ((j == 0 && i == 7 && c_board[i-1][j+1] == "/" )||
						(j == 0 && i != 7 && (c_board[i+1][j+1] == "/" || c_board[i-1][j+1] == "/")))
						Normal_move[i].push_back(j);

					else if ((j == 7 && i == 0 && c_board[i+1][j-1] == "/" )||
						(j == 7 && i != 0 && (c_board[i-1][j-1] == "/" || c_board[i+1][j-1] == "/")))
						Normal_move[i].push_back(j);

					else if (i == 7 && j != 0 && (c_board[i-1][j-1] == "/" || c_board[i-1][j+1] == "/"))
						Normal_move[i].push_back(j);

					else if (i == 0 && j != 7 && (c_board[i+1][j-1] == "/" || c_board[i+1][j+1] == "/"))
						Normal_move[i].push_back(j);
					else if (i != 0 && i != 7 && j != 0 && j != 7)
					{
						if(c_board[i+1][j+1] == "/" || c_board[i-1][j-1] == "/" || c_board[i+1][j-1] == "/"
							|| c_board[i-1][j+1] == "/")
							Normal_move[i].push_back(j);
					}
				}
				
			}
		}
		return Normal_move;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//copies the state of the board into a different array
std::string** Checkers::copyBoard(std::string**& orig_board)
{
	std::string** c_board;
	c_board = new std::string*[8];
	for (int i = 0; i < 8; ++i)
	{
		c_board[i] = new std::string[8];
		for (int j = 0; j < 8; ++j)
		{
			c_board[i][j] = orig_board[i][j];
		}
	}
	return c_board;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//finds a position for the cpu to move one of its pieces for more info => www.youngjoshua.com/projects/checkers
void Checkers::findMovePosition(int& row,int& colmun,int& prevrow,int& prevcolum,bool& isJump,bool& game_over)
{

	std::map<int,std::vector<int> > cpu_jump;
	if(CPUforceJump(board, cpu_jump))
	{
		CPUdoJump(prevrow,prevcolum,cpu_jump);
		isJump = true;
		return;
	}
	
	
	std::map<int,std::vector<int> > cpuNormal_move = getMoves(board,false);
	if (cpuNormal_move.empty())
	{
		std::cout << "EMPTY MAP" << std::endl;
		game_over = true;
		return;
	}
	int depth = level;
	std::map<int,std::vector<int> >::const_iterator it;
	int bestoutcome = -1000;
	for (it = cpuNormal_move.begin(); it != cpuNormal_move.end(); it++)
	{
		int r = it->first;
		for (int i = 0; i < it->second.size();i++)
		{
			string** temp_board = copyBoard(board);
			//delete after
			int c = it->second[i];
			if (temp_board[r][c] == "W")
			{
				if ( r != 7 && c == 0 && temp_board[r+1][c+1] == "/")
				{
					temp_board[r][c] = "/";
					if (r+1 == 7)
						temp_board[r+1][c+1] = "C";
					else
						temp_board[r+1][c+1] = "W";
					int value = minimaxAlg(true,temp_board,depth);
					temp_board[r][c] = "W";
					temp_board[r+1][c+1] = "/";
					if (value > bestoutcome)
					{
						row = r+1;
						colmun = c+1;
						prevrow = r;
						prevcolum = c;
						bestoutcome = value;
					}
				}
				else if (r != 7 && c == 7 && temp_board[r+1][c-1] == "/")
				{
					temp_board[r][c] = "/";
					if (r+1 == 7)
						temp_board[r+1][c-1] = "C";
					else
						temp_board[r+1][c-1] = "W";
					int value = minimaxAlg(true,temp_board,depth);
					temp_board[r][c] = "W";
					temp_board[r+1][c-1] = "/";
					if (value > bestoutcome)
					{
						row = r+1;
						colmun = c-1;
						prevrow = r;
						prevcolum = c;
						bestoutcome = value;
					}
				}
				else if (r != 7 && c != 0)
				{
					if (temp_board[r+1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						if (r+1 == 7)
							temp_board[r+1][c+1] = "C";
						else
							temp_board[r+1][c+1] = "W";
					//	show(temp_board);
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "W";
						temp_board[r+1][c+1] = "/";
						if (value > bestoutcome)
						{
							row = r+1;
							colmun = c+1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}

					if (temp_board[r+1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						if (r+1 == 7)
							temp_board[r+1][c-1] = "C";
						else
							temp_board[r+1][c-1] = "W";
					//	show(temp_board);
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "W";
						temp_board[r+1][c-1] = "/";
						if (value > bestoutcome)
						{
							row = r+1;
							colmun = c-1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}
				}
			}

			else
			{
				if (c == 0 && r == 7 && temp_board[r-1][c+1] == "/")
				{
					temp_board[r][c] = "/";
					temp_board[r-1][c+1] = "C";
					int value = minimaxAlg(true,temp_board,depth);
					temp_board[r][c] = "C";
					temp_board[r-1][c+1] = "/";
					if (value > bestoutcome)
					{
						row = r-1;
						colmun = c+1;
						prevrow = r;
						prevcolum = c;
						bestoutcome = value;
					}
				}

				else if (c == 0 && r != 7)
				{
					if(temp_board[r-1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r-1][c+1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r-1][c+1] = "/";
						if (value > bestoutcome)
						{
							row = r-1;
							colmun = c+1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}

					if (temp_board[r+1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r+1][c+1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r+1][c+1] = "/";
						if (value > bestoutcome)
						{
							row = r+1;
							colmun = c+1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}
				}

				else if (c == 7 && r == 0 && temp_board[r+1][c-1] == "/")
				{
					temp_board[r][c] = "/";
					temp_board[r+1][c-1] = "C";
					int value = minimaxAlg(true,temp_board,depth);
					temp_board[r][c] = "C";
					temp_board[r+1][c-1] = "/";
					if (value > bestoutcome)
					{
						row = r+1;
						colmun = c-1;
						prevrow = r;
						prevcolum = c;
						bestoutcome = value;
					}
				}

				else if (c == 7 && r != 0)
				{
					if (temp_board[r+1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r+1][c-1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r+1][c-1] = "/";
						if (value > bestoutcome)
						{
							row = r+1;
							colmun = c-1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}

					if (temp_board[r-1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r-1][c-1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r-1][c-1] = "/";
						if (value > bestoutcome)
						{
							row = r-1;
							colmun = c-1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}
				}

				else if (r == 7 && c != 0)
				{
					if(temp_board[r-1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r-1][c-1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r-1][c-1] = "/";
						if (value > bestoutcome)
						{
							row = r-1;
							colmun = c-1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}

					if(temp_board[r-1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r-1][c+1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r-1][c+1] = "/";
						if (value > bestoutcome)
						{
							row = r-1;
							colmun = c+1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}
				}

				else if (r == 0 && c != 7)
				{
					if(temp_board[r+1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r+1][c-1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r+1][c-1] = "/";
						if (value > bestoutcome)
						{
							row = r+1;
							colmun = c-1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}

					if(temp_board[r+1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r+1][c+1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r+1][c+1] = "/";
						if (value > bestoutcome)
						{
							row = r+1;
							colmun = c+1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}
				}

				else
				{
					if(temp_board[r+1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r+1][c-1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r+1][c-1] = "/";
						if (value > bestoutcome)
						{
							row = r+1;
							colmun = c-1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}

					if(temp_board[r+1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r+1][c+1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r+1][c+1] = "/";
						if (value > bestoutcome)
						{
							row = r+1;
							colmun = c+1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}

					if(temp_board[r-1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r-1][c-1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r-1][c-1] = "/";
						if (value > bestoutcome)
						{
							row = r-1;
							colmun = c-1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}

					if(temp_board[r-1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r-1][c+1] = "C";
						int value = minimaxAlg(true,temp_board,depth);
						temp_board[r][c] = "C";
						temp_board[r-1][c+1] = "/";
						if (value > bestoutcome)
						{
							row = r-1;
							colmun = c+1;
							prevrow = r;
							prevcolum = c;
							bestoutcome = value;
						}
					}
				}
			}
		destroy(temp_board);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//evaluates the current state of the board
int Checkers::EvaluateBoard(std::string**& temp_board)
{
	int p1_score = 0;
	int cpu_score = 0;
	if (player1 == cpu)
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (temp_board[i][j] == "W")
					cpu_score += boardScore[i][j];
				else if (temp_board[i][j] == "B")
					p1_score += boardScore[i][j];
				else if (temp_board[i][j] == "K")
					p1_score += boardScore[i][j] + 2;
				else if (temp_board[i][j] == "C")
					p1_score += boardScore[i][j] + 2;
			}
		}
		return (cpu_score - p1_score);
	}

	else
		return (cpu - player1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//performs jump moves for the computer
int Checkers::MinMaxCPUdoJump(std::string**& temp_board,int depth,std::map<int,std::vector<int> >& cpu_jump)
{
	int bestoutcome = -1000;
	int saveScore = player1;
	int save_score_cpu = cpu;
	bool isKing = false;
	std::map<int,std::vector<int> >::const_iterator it;
	for (it = cpu_jump.begin();it != cpu_jump.end();it++)
	{
		int r = it->first;
		for (int i = 0; i < it->second.size(); ++i)
		{
			string** tempB = copyBoard(temp_board);
			int c = it->second[i];
			if (tempB[r][c] == "C")
				isKing = true;
			CPUminMaxJump(r,c,tempB,isKing);
		//	show(tempB);
			int value = minimaxAlg(true,tempB,depth-1);
			if (value > bestoutcome)
				bestoutcome = value;
			destroy(tempB);
			player1 = saveScore;
			cpu = save_score_cpu;
			isKing = false;
		}
	}
	cpu_jump.clear();
	return bestoutcome;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//performs jump moves for the computer and checks if more jumps are possible for more info => www.youngjoshua.com/projects/checkers
void Checkers::CPUminMaxJump(int r,int c,std::string**& c_board,bool isKing)
{
	if (!isKing)
	{
		int bestVal = -1000;
		bool leftSide = false;
		bool rightSide = false;
		string** tempBoard;
		string** new_tempBoard;
		int p1_save;
		int new_p1Save;
		if ((c == 0 && r != 7) && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
		{
			c_board[r+1][c+1] = "/";
			c_board[r][c] = "/";
			if ((r+2) == 7)
			{
				c_board[r+2][c+2] = "C";
				isKing = true;
			}
			else
			{
				c_board[r+2][c+2] = "W";
				isKing = false;
			}
			player1 -= 1;
			CPUminMaxJump(r+2,c+2,c_board,isKing);
		}

		else if ((c == 7 && r != 6)&& (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
		{
			c_board[r+1][c-1] = "/";
			c_board[r][c] = "/";
			if ((r+2) == 7)
			{
				c_board[r+2][c-2] = "C";
				isKing = true;
			}
			else
			{
				c_board[r+2][c-2] = "W";
				isKing = false;
			}
			player1 -= 1;
			CPUminMaxJump(r+2,c-2,c_board,isKing);
		}

		else
		{
			if ((c != 1 && c != 0 && r != 7 && r != 6) && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
			{
				tempBoard = copyBoard(c_board);
				p1_save = player1;
				c_board[r+1][c-1] = "/";
				c_board[r][c] = "/";
				if ((r+2) == 7)
				{
					c_board[r+2][c-2] = "C";
					isKing = true;
				}
				else
				{
					c_board[r+2][c-2] = "W";
					isKing = false;
				}
				player1 -= 1;
				CPUminMaxJump(r+2,c-2,c_board,isKing);
				leftSide = true;
				int value = EvaluateBoard(c_board);
				if (value > bestVal)
					bestVal = value;
			}

			if ((c != 6 && c != 7 && r != 7 && r != 6) && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				if (leftSide)
				{
					new_tempBoard = copyBoard(c_board);
					c_board = copyBoard(tempBoard);

					new_p1Save = player1;
					player1 = p1_save;
					rightSide = true;
					c_board[r+1][c+1] = "/";
					c_board[r][c] = "/";
					if ((r+2) == 7)
					{
						c_board[r+2][c+2] = "C";
						isKing = true;
					}
					else
					{
						c_board[r+2][c+2] = "W";
						isKing = false;
					}
					player1 -= 1;
					CPUminMaxJump(r+2,c+2,c_board,isKing);
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
					{
						c_board = copyBoard(new_tempBoard);
						player1 = new_p1Save;
					}

				}

				else
				{
					c_board[r+1][c+1] = "/";
					c_board[r][c] = "/";
					if ((r+2) == 7)
					{
						c_board[r+2][c+2] = "C";
						isKing = true;
					}
					else
					{
						c_board[r+2][c+2] = "W";
						isKing = false;
					}
					player1 -= 1;
					CPUminMaxJump(r+2,c+2,c_board,isKing);
				}
			}
		}

		if (leftSide)
			destroy(tempBoard);
		if (rightSide)
			destroy(new_tempBoard);
	}

	else
	{
		int bestVal = -1000;
		bool leftside_upper = false;
		bool leftside_lower = false;
		string** leftside_upper_board;
		string** leftside_lower_board;
		int p1_left_upper;
		int p1_left_lower;

		bool rightside_upper = false;
		bool rightside_lower = false;
		string** rightside_upper_board;
		string** rightside_lower_board;
		int p1_right_upper;
		int p1_right_lower;

		bool top_right = false;
		bool top_left = false;
		string** top_right_board;
		string** top_left_board;
		int p1_top_right;
		int p1_top_left;

		bool bottom_right = false;
		bool bottom_left = false;
		string** bottom_right_board;
		string** bottom_left_board;
		int p1_bottom_right;
		int p1_bottom_left;

		bool middle = false;
		bool middle_temp = false;
		string** middle_board;
		string** middle_board_temp;
		int p1_middle;
		int p1_middle_temp;


		if (c == 0 && r == 7 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
		{
			c_board[r][c] = "/";
			c_board[r-1][c+1] = "/";
			c_board[r-2][c+2] = "C";
			player1 -= 1;
			CPUminMaxJump(r-2,c+2,c_board,true);
		}

		else if (r == 0 && c == 7 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
		{
			c_board[r][c] = "/";
			c_board[r+1][c-1] = "/";
			c_board[r+2][c-2] = "C";
			player1 -= 1;
			CPUminMaxJump(r+2,c-2,c_board,true);
		}

		else if ((c == 0 || c == 1) && r != 7)
		{
			if (r == 1 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "C";
				player1 -= 1;
				CPUminMaxJump(r+2,c+2,c_board,true);
			}

			else if (r == 6 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c+1] = "/";
				c_board[r-2][c+2] = "C";
				player1 -= 1;
				CPUminMaxJump(r-2,c+2,c_board,true);
			}

			else if (r == 0 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "C";
				player1 -= 1;
				CPUminMaxJump(r+2,c+2,c_board,true);
			}

			else
			{
				if (r != 1 && r != 0 && (c_board[r-1][c+1] == "B" 
					|| c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
				{
					leftside_upper_board = copyBoard(c_board);
					p1_left_upper = player1;
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "C";
					player1 -= 1;
					CPUminMaxJump(r-2,c+2,c_board,true);
					leftside_upper = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
				if (r != 6 && r != 7 && (c_board[r+1][c+1] == "B" 
					|| c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
				{
					if (leftside_upper)
					{
						leftside_lower_board = copyBoard(c_board);
						c_board = copyBoard(leftside_upper_board);
						p1_left_lower = player1;
						player1 = p1_left_upper;
						
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "C";
						player1 -= 1;
						CPUminMaxJump(r+2,c+2,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value < bestVal)
						{
							c_board = copyBoard(leftside_lower_board);
							player1 = p1_left_lower;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "C";
						player1 -= 1;
						CPUminMaxJump(r+2,c+2,c_board,true);
					}
				}
			}
		}

		else if ((c == 7 || c == 6) && r != 0)
		{
			if (r == 6 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				CPUminMaxJump(r-2,c-2,c_board,true);
			}

			else if (r == 7 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				CPUminMaxJump(r-2,c-2,c_board,true);
			}

			else if (r == 1 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c-1] = "/";
				c_board[r+2][c-2] = "C";
				player1 -= 1;
				CPUminMaxJump(r+2,c-2,c_board,true);
			}

			else
			{
				if (r != 1 && r != 0 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
				{
					rightside_upper_board = copyBoard(c_board);
					p1_right_upper = player1;
					c_board[r][c] = "/";
					c_board[r-1][c-1] = "/";
					c_board[r-2][c-2] = "C";
					player1 -= 1;
					CPUminMaxJump(r-2,c-2,c_board,true);
					rightside_upper = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
				if (r != 7 && r != 6 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
				{
					if (rightside_upper)
					{
						rightside_lower_board = copyBoard(c_board);
						c_board = copyBoard(rightside_upper_board);
						p1_right_lower = player1;
						player1 = p1_right_upper;
						rightside_lower = true;
						
						c_board[r][c] = "/";
						c_board[r+1][c-1] = "/";
						c_board[r+2][c-2] = "C";
						player1 -= 1;
						CPUminMaxJump(r+2,c-2,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value < bestVal)
						{
							c_board = copyBoard(rightside_lower_board);
							player1 = p1_right_lower;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c-1] = "/";
						c_board[r+2][c-2] = "C";
						player1 -= 1;
						CPUminMaxJump(r+2,c-2,c_board,true);
					}
				}
			}
		}

		else if ((r == 0 || r == 1) && c != 7)
		{
			if (c == 1 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "C";
				player1 -= 1;
				CPUminMaxJump(r+2,c+2,c_board,true);
			}

			else if (c == 0 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "C";
				player1 -= 1;
				CPUminMaxJump(r+2,c+2,c_board,true);
			}

			else if (c == 6 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c-1] = "/";
				c_board[r+2][c-2] = "C";
				player1 -= 1;
				CPUminMaxJump(r+2,c-2,c_board,true);
			}

			else
			{
				if (c != 1 && c != 0 && (c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
				{
					top_left_board = copyBoard(c_board);
					p1_top_left = player1;

					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "C";
					player1 -= 1;
					CPUminMaxJump(r+2,c-2,c_board,true);
					top_left = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
				if (c != 7 && c != 6 && (c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
				{
					if (top_left)
					{
						top_right_board = copyBoard(c_board);
						c_board = copyBoard(top_left_board);
						p1_top_right = player1;
						player1 = p1_top_left;
						top_right = true;
						
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "C";
						player1 -= 1;
						CPUminMaxJump(r+2,c+2,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value < bestVal)
						{
							c_board = copyBoard(top_right_board);
							player1 = p1_top_right;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "C";
						player1 -= 1;
						CPUminMaxJump(r+2,c+2,c_board,true);
					}
				}
			}
		}

		else if ((r == 7 || r == 6) && c != 0)
		{
			if (c == 6 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				CPUminMaxJump(r-2,c-2,c_board,true);
			}

			else if (c == 1 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c+1] = "/";
				c_board[r-2][c+2] = "C";
				player1 -= 1;
				CPUminMaxJump(r-2,c+2,c_board,true);
			}

			else if (c == 7 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				CPUminMaxJump(r-2,c-2,c_board,true);
			}

			else
			{
				if (c != 1 && c != 0 && (c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
				{
					bottom_left_board = copyBoard(c_board);
					p1_bottom_left = player1;

					c_board[r][c] = "/";
					c_board[r-1][c-1] = "/";
					c_board[r-2][c-2] = "C";
					player1 -= 1;
					CPUminMaxJump(r-2,c-2,c_board,true);
					bottom_left = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
				if (c != 6 && c != 7 && (c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
				{
					if (bottom_left)
					{
						bottom_right_board = copyBoard(c_board);
						c_board = copyBoard(bottom_left_board);
						p1_bottom_right = player1;
						player1 = p1_bottom_left;
						bottom_right = true;
						
						c_board[r][c] = "/";
						c_board[r-1][c+1] = "/";
						c_board[r-2][c+2] = "C";
						player1 -= 1;
						CPUminMaxJump(r-2,c+2,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value < bestVal)
						{
							c_board = copyBoard(bottom_right_board);
							player1 = p1_bottom_right;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r-1][c+1] = "/";
						c_board[r-2][c+2] = "C";
						player1 -= 1;
						CPUminMaxJump(r-2,c+2,c_board,true);
					}
				}
			}
		}

		else if ((r != 0 && r != 1 && r != 6 && r != 7) && (c != 0 && c != 1 && c != 6 && c != 7))
		{
			if ((c_board[r-1][c-1] == "B" || c_board[r-1][c-1] == "K") && c_board[r-2][c-2] == "/")
			{
				middle_board = copyBoard(c_board);
				p1_middle = player1;

				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "C";
				player1 -= 1;
				CPUminMaxJump(r-2,c-2,c_board,true);
				middle = true;
				int value = EvaluateBoard(c_board);
				if (value > bestVal)
					bestVal = value;
			}

			if ((c_board[r+1][c-1] == "B" || c_board[r+1][c-1] == "K") && c_board[r+2][c-2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					p1_middle_temp = player1;
					player1 = p1_middle;
					middle_temp = true;
					
					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "C";
					player1 -= 1;
					CPUminMaxJump(r+2,c-2,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						player1 = p1_bottom_right;
					}
				}

				else
				{
					middle_board = copyBoard(c_board);
					p1_middle = player1;

					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "C";
					player1 -= 1;
					CPUminMaxJump(r+2,c-2,c_board,true);
					middle = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
			}

			if ((c_board[r+1][c+1] == "B" || c_board[r+1][c+1] == "K") && c_board[r+2][c+2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					p1_middle_temp = player1;
					player1 = p1_middle;
					middle_temp = true;
					
					c_board[r][c] = "/";
					c_board[r+1][c+1] = "/";
					c_board[r+2][c+2] = "C";
					player1 -= 1;
					CPUminMaxJump(r+2,c+2,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						player1 = p1_bottom_right;
					}
				}

				else
				{
					middle_board = copyBoard(c_board);
					p1_middle = player1;

					c_board[r][c] = "/";
					c_board[r+1][c+1] = "/";
					c_board[r+2][c+2] = "C";
					player1 -= 1;
					CPUminMaxJump(r+2,c+2,c_board,true);
					middle = true;
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
						bestVal = value;
				}
			}

			if ((c_board[r-1][c+1] == "B" || c_board[r-1][c+1] == "K") && c_board[r-2][c+2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					p1_middle_temp = player1;
					player1 = p1_middle;
					middle_temp = true;
					
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "C";
					player1 -= 1;
					CPUminMaxJump(r-2,c+2,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						player1 = p1_bottom_right;
					}
				}

				else
				{
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "C";
					player1 -= 1;
					CPUminMaxJump(r-2,c+2,c_board,true);
				}
			}
		}

	if (leftside_upper)
		destroy(leftside_upper_board);
	if (leftside_lower)
		destroy(leftside_lower_board);
	if (rightside_upper)
		destroy(rightside_upper_board);
	if (rightside_lower)
		destroy(rightside_lower_board);
	if (top_right)
		destroy(top_right_board);
	if (top_left)
		destroy(top_left_board);
	if (bottom_right)
		destroy(bottom_right_board);
	if (bottom_left)
		destroy(bottom_left_board);
	if (middle)
		destroy(middle_board);
	if (middle_temp)
		destroy(middle_board_temp);
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//checks if there's force jump for the player
bool Checkers::P1_minMaxforceJump(std::string**& c_board,std::map<int,std::vector<int> >& p1_jump)
{
	std::string players = "B";
	std::string cpus = "W";

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (c_board[i][j] == players && i != 0)
			{
				if ((j == 0 && i != 1) && (c_board[i-1][j+1] == cpus 
					|| c_board[i-1][j+1] == "C") && c_board[i-2][j+2] == "/")
					p1_jump[i].push_back(j);
				
				else if (j == 7 && (c_board[i-1][j-1] == cpus 
					|| c_board[i-1][j-1] == "C") && c_board[i-2][j-2] == "/")
					p1_jump[i].push_back(j);
				else
				{
					if ((j != 1 && j != 0 && i != 1) && (c_board[i-1][j-1] == "W" 
						|| c_board[i-1][j-1] == "C") && c_board[i-2][j-2] == "/")
						p1_jump[i].push_back(j);

					if ((j != 6 && j != 7 && i != 1) && (c_board[i-1][j+1] == "W" 
						|| c_board[i-1][j+1] == "C") && c_board[i-2][j+2] == "/")
						p1_jump[i].push_back(j);
				}
		
			}

			else if (c_board[i][j] == "K")
			{
				if ((j == 0 && i == 7) && (c_board[i-1][j+1] == "C" ||c_board[i-1][j+1] == "W") && (c_board[i-2][j+2] == "/"))
					p1_jump[i].push_back(j);

				else if (((j == 0 || j == 1) && i != 7 && i != 6 && i != 1 && i != 0) && 
					(((c_board[i-1][j+1] == "C" ||c_board[i-1][j+1] == "W") && (c_board[i-2][j+2] == "/")) ||
						((c_board[i+1][j+1] == "C" ||c_board[i+1][j+1] == "W") && (c_board[i+2][j+2] == "/"))))
					p1_jump[i].push_back(j);

				else if ((j == 7 && i == 0) && (c_board[i+1][j-1] == "C" ||c_board[i+1][j-1] == "W") && (c_board[i+2][j-2] == "/"))
					p1_jump[i].push_back(j);

				else if (((j == 7 || j == 6)&& i != 0 && i != 7 && i != 6 && i != 1) && 
					(((c_board[i+1][j-1] == "C" ||c_board[i+1][j-1] == "W") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i-1][j-1] == "C" ||c_board[i-1][j-1] == "W") && (c_board[i-2][j-2] == "/")))
					p1_jump[i].push_back(j);

				else if (((i == 7 || i == 6)&& j != 0 && j != 7 && j != 6 && j != 1) && 
					(((c_board[i-1][j+1] == "C" ||c_board[i-1][j+1] == "W") && (c_board[i-2][j+2] == "/")) ||
						(c_board[i-1][j-1] == "C" ||c_board[i-1][j-1] == "W") && (c_board[i-2][j-2] == "/")))
					p1_jump[i].push_back(j);

				else if (((i == 0 || i == 1)&& j != 0 && j != 7 && j != 6 && j != 1) && 
					(((c_board[i+1][j-1] == "C" ||c_board[i+1][j-1] == "W") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i+1][j+1] == "C" ||c_board[i+1][j+1] == "W") && (c_board[i+2][j+2] == "/")))
					p1_jump[i].push_back(j);

				else if (((i != 0 && i != 1 && i != 6 && i != 7 && j != 1 && j != 0 && j != 6 && j != 7)) &&
					((((c_board[i+1][j-1] == "C" ||c_board[i+1][j-1] == "W") && (c_board[i+2][j-2] == "/")) ||
						(c_board[i+1][j+1] == "C" ||c_board[i+1][j+1] == "W") && (c_board[i+2][j+2] == "/")) ||
							((((c_board[i-1][j+1] == "C" ||c_board[i-1][j+1] == "W") && (c_board[i-2][j+2] == "/")) ||
								(c_board[i-1][j-1] == "C" ||c_board[i-1][j-1] == "W") && (c_board[i-2][j-2] == "/")))))
					p1_jump[i].push_back(j);



			}
		}
	}

	if (p1_jump.size() > 0)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//simulates jump moves for the player
int Checkers::P1MinMaxdoJump(std::string**& temp_board,int depth,std::map<int,std::vector<int> >& p1_jump)
{
	int leastoutcome = 1000;
	int saveScore = player1;
	int save_score_cpu = cpu;
	bool isKing = false;
	std::map<int,std::vector<int> >::const_iterator it;
	for (it = p1_jump.begin();it != p1_jump.end();it++)
	{
		int r = it->first;
		for (int i = 0; i < it->second.size(); i++)
		{
			string** tempB = copyBoard(temp_board);
			int c = it->second[i];
			if (tempB[r][c] == "K")
				isKing = true;
			P1minMaxJump(r,c,tempB,isKing);
			int value = minimaxAlg(false,tempB,depth-1);
			if (value < leastoutcome)
				leastoutcome = value;
			destroy(tempB);
			player1 = saveScore;
			cpu = save_score_cpu;
			isKing = false;
		}
	}
	p1_jump.clear();
	return leastoutcome;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//simulates jump moves for the player and checks if a jump move is possible for more info => www.youngjoshua.com/projects/checkers
void Checkers::P1minMaxJump(int r,int c,std::string**& c_board,bool isKing)
{
	if (!isKing)
	{
		string** tempBoard;
		string** new_tempBoard;
		int bestVal = 1000;
		bool leftSide = false;
		bool rightSide = false;
		int cpu_save;
		int new_cpuSave;
		if ((c == 0 && r != 1)  && (c_board[r-1][c+1] == "W" || c_board[r-1][c+1] == "C") && c_board[r-2][c+2] == "/")
		{
			c_board[r-1][c+1] = "/";
			c_board[r][c] = "/";
			if ((r-2) == 0)
			{
				c_board[r-2][c+2] = "K";
				isKing = true;
			}
			else
			{
				c_board[r-2][c+2] = "B";
				isKing = false;
			}
			cpu -= 1;
			//after the jumping is done
			P1minMaxJump(r-2,c+2,c_board,isKing);
		}

		else if ((c == 7 && r != 0) && (c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") && c_board[r-2][c-2] == "/")
		{
			c_board[r-1][c-1] = "/";
			c_board[r][c] = "/";
			if ((r-2) == 0)
			{
				c_board[r-2][c-2] = "K";
				isKing = true;
			}
			else
			{
				c_board[r-2][c-2] = "B";
				isKing = false;
			}
			cpu -= 1;
			//after the jumping is done
			P1minMaxJump(r-2,c-2,c_board,isKing);
		}

		else
		{
			if ((c != 1 && c != 0 && r != 0 && r != 1) && (c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") && c_board[r-2][c-2] == "/")
			{
				tempBoard = copyBoard(c_board);
				cpu_save = cpu;
				c_board[r-1][c-1] = "/";
				c_board[r][c] = "/";
				if ((r-2) == 0)
				{
					c_board[r-2][c-2] = "K";
					isKing = true;
				}
				else
				{
					c_board[r-2][c-2] = "B";
					isKing = false;
				}
				cpu -= 1;
				P1minMaxJump(r-2,c-2,c_board,isKing);
				leftSide = true;
				int value = EvaluateBoard(c_board);
				if (value < bestVal)
					bestVal = value;
			}

			if ((c != 6 && c != 7 && r != 0 && r != 1) && (c_board[r-1][c+1] == "W" || c_board[r-1][c+1] == "C") && c_board[r-2][c+2] == "/")
			{
				if (leftSide)
				{
					new_tempBoard = copyBoard(c_board);
					c_board = copyBoard(tempBoard);
					rightSide = true;
					new_cpuSave = cpu;
					cpu = cpu_save;

					c_board[r-1][c+1] = "/";
					c_board[r][c] = "/";
					if ((r-2) == 0)
					{
						c_board[r-2][c+2] = "K";
						isKing = true;
					}
					else
					{
						c_board[r-2][c+2] = "B";
						isKing = false;
					}
					cpu -= 1;
					P1minMaxJump(r-2,c+2,c_board,isKing);
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
					{
						c_board = copyBoard(new_tempBoard);
						cpu = new_cpuSave;
					}

				}
				else
				{
					c_board[r-1][c+1] = "/";
					c_board[r][c] = "/";
					if ((r-2) == 0)
					{
						c_board[r-2][c+2] = "K";
						isKing = true;
					}
					else
					{
						c_board[r-2][c+2] = "B";
						isKing = false;
					}
					cpu -= 1;
					P1minMaxJump(r-2,c+2,c_board,isKing);
				}
			}
		}
		
		if (leftSide)
			destroy(tempBoard);
		if (rightSide)
			destroy(new_tempBoard);
	}

	else
	{
		int bestVal = 1000;
		bool leftside_upper = false;
		bool leftside_lower = false;
		string** leftside_upper_board;
		string** leftside_lower_board;
		int cpu_left_upper;
		int cpu_left_lower;

		bool rightside_upper = false;
		bool rightside_lower = false;
		string** rightside_upper_board;
		string** rightside_lower_board;
		int cpu_right_upper;
		int cpu_right_lower;

		bool top_right = false;
		bool top_left = false;
		string** top_right_board;
		string** top_left_board;
		int cpu_top_right;
		int cpu_top_left;

		bool bottom_right = false;
		bool bottom_left = false;
		string** bottom_right_board;
		string** bottom_left_board;
		int cpu_bottom_right;
		int cpu_bottom_left;

		bool middle = false;
		bool middle_temp = false;
		string** middle_board;
		string** middle_board_temp;
		int cpu_middle;
		int cpu_middle_temp;


		if (c == 0 && r == 7 && (c_board[r-1][c+1] == "W" || c_board[r-1][c+1] == "C") && c_board[r-2][c+2] == "/")
		{
			c_board[r][c] = "/";
			c_board[r-1][c+1] = "/";
			c_board[r-2][c+2] = "K";
			cpu -= 1;
			P1minMaxJump(r-2,c+2,c_board,true);
		}

		else if (r == 0 && c == 7 && (c_board[r+1][c-1] == "W" || c_board[r+1][c-1] == "C") && c_board[r+2][c-2] == "/")
		{
			c_board[r][c] = "/";
			c_board[r+1][c-1] = "/";
			c_board[r+2][c-2] = "K";
			cpu -= 1;
			P1minMaxJump(r+2,c-2,c_board,true);
		}

		else if ((c == 0 || c == 1) && r != 7)
		{
			if (r == 1 && (c_board[r+1][c+1] == "W" || c_board[r+1][c+1] == "C") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "K";
				cpu -= 1;
				P1minMaxJump(r+2,c+2,c_board,true);
			}

			else if (r == 6 && (c_board[r-1][c+1] == "W" || c_board[r-1][c+1] == "C") && c_board[r-2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c+1] = "/";
				c_board[r-2][c+2] = "K";
				cpu -= 1;
				P1minMaxJump(r-2,c+2,c_board,true);
			}

			else if (r == 0 && (c_board[r+1][c+1] == "W" || c_board[r+1][c+1] == "C") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "K";
				cpu -= 1;
				P1minMaxJump(r+2,c+2,c_board,true);
			}

			else
			{
				if (r != 1 && r != 0 && r != 6 && (c_board[r-1][c+1] == "W" || c_board[r-1][c+1] == "C") && c_board[r-2][c+2] == "/")
				{
					leftside_upper_board = copyBoard(c_board);
					cpu_left_upper = cpu;
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "K";
					cpu -= 1;
					P1minMaxJump(r-2,c+2,c_board,true);
					leftside_upper = true;
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
						bestVal = value;
				}
				if (c != 0 && c != 1 && r != 6 && (c_board[r+1][c-1] == "W" || c_board[r+1][c-1] == "C") && c_board[r+2][c-2] == "/")
				{
					if (leftside_upper)
					{
						leftside_lower_board = copyBoard(c_board);
						c_board = copyBoard(leftside_upper_board);
						cpu_left_lower = cpu;
						cpu = cpu_left_upper;
						leftside_lower = true;
						
						c_board[r][c] = "/";
						c_board[r+1][c-1] = "/";
						c_board[r+2][c-2] = "K";
						cpu -= 1;
						P1minMaxJump(r+2,c-2,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value > bestVal)
						{
							c_board = copyBoard(leftside_lower_board);
							cpu = cpu_left_lower;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c-1] = "/";
						c_board[r+2][c-2] = "K";
						cpu -= 1;
						P1minMaxJump(r+2,c-2,c_board,true);
					}
				}
			}
		}

		else if ((c == 7 || c == 6) && r != 0)
		{
			if (r == 6 && (c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "K";
				cpu -= 1;
				P1minMaxJump(r-2,c-2,c_board,true);
			}

			else if (r == 7 && (c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "K";
				cpu -= 1;
				P1minMaxJump(r-2,c-2,c_board,true);
			}

			else if (r == 1 && (c_board[r+1][c-1] == "W" || c_board[r+1][c-1] == "C") && c_board[r+2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c-1] = "/";
				c_board[r+2][c-2] = "K";
				cpu -= 1;
				P1minMaxJump(r+2,c-2,c_board,true);
			}

			else
			{
				if (r != 0 && r != 1 && (c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") && c_board[r-2][c-2] == "/")
				{
					rightside_upper_board = copyBoard(c_board);
					cpu_right_upper = cpu;
					c_board[r][c] = "/";
					c_board[r-1][c-1] = "/";
					c_board[r-2][c-2] = "K";
					cpu -= 1;
					P1minMaxJump(r-2,c-2,c_board,true);
					rightside_upper = true;
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
						bestVal = value;
				}
				if (r != 6 && r != 7 && (c_board[r+1][c-1] == "W" || c_board[r+1][c-1] == "C") && c_board[r+2][c-2] == "/")
				{
					if (rightside_upper)
					{
						rightside_lower_board = copyBoard(c_board);
						c_board = copyBoard(rightside_upper_board);
						cpu_right_lower = cpu;
						cpu = cpu_right_upper;
						rightside_lower = true;
						
						c_board[r][c] = "/";
						c_board[r+1][c-1] = "/";
						c_board[r+2][c-2] = "K";
						cpu -= 1;
						P1minMaxJump(r+2,c-2,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value > bestVal)
						{
							c_board = copyBoard(rightside_lower_board);
							cpu = cpu_right_lower;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c-1] = "/";
						c_board[r+2][c-2] = "K";
						cpu -= 1;
						P1minMaxJump(r+2,c-2,c_board,true);
					}
				}
			}
		}

		else if ((r == 0 || r == 1) && c != 7)
		{
			if (c == 1 && (c_board[r+1][c+1] == "W" || c_board[r+1][c+1] == "C") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "K";
				cpu -= 1;
				P1minMaxJump(r+2,c+2,c_board,true);
			}

			else if (c == 0 && (c_board[r+1][c+1] == "W" || c_board[r+1][c+1] == "C") && c_board[r+2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c+1] = "/";
				c_board[r+2][c+2] = "K";
				cpu -= 1;
				P1minMaxJump(r+2,c+2,c_board,true);
			}

			else if (c == 6 && (c_board[r+1][c-1] == "W" || c_board[r+1][c-1] == "C") && c_board[r+2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r+1][c-1] = "/";
				c_board[r+2][c-2] = "K";
				cpu -= 1;
				P1minMaxJump(r+2,c-2,c_board,true);
			}

			else
			{
				if ( c != 1 && c != 0 && (c_board[r+1][c-1] == "W" || c_board[r+1][c-1] == "C") && c_board[r+2][c-2] == "/")
				{
					top_left_board = copyBoard(c_board);
					cpu_top_left = cpu;

					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "K";
					cpu -= 1;
					P1minMaxJump(r+2,c-2,c_board,true);
					top_left = true;
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
						bestVal = value;
				}
				if (c != 6 && (c_board[r+1][c+1] == "W" || c_board[r+1][c+1] == "C") && c_board[r+2][c+2] == "/")
				{
					if (top_left)
					{
						top_right_board = copyBoard(c_board);
						c_board = copyBoard(top_left_board);
						cpu_top_right = cpu;
						cpu = cpu_top_left;
						top_right = true;
						
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "K";
						cpu -= 1;
						P1minMaxJump(r+2,c+2,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value > bestVal)
						{
							c_board = copyBoard(top_right_board);
							cpu = cpu_top_right;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r+1][c+1] = "/";
						c_board[r+2][c+2] = "K";
						cpu -= 1;
						P1minMaxJump(r+2,c+2,c_board,true);
					}
				}
			}
		}

		else if ((r == 7 || r == 6) && c != 0)
		{
			if (c == 6 && (c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") 
				&& c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "K";
				cpu -= 1;
				P1minMaxJump(r-2,c-2,c_board,true);
			}

			else if (c == 1 && (c_board[r-1][c+1] == "W" || c_board[r-1][c+1] == "C") && c_board[r-2][c+2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c+1] = "/";
				c_board[r-2][c+2] = "K";
				cpu -= 1;
				P1minMaxJump(r-2,c+2,c_board,true);
			}

			else if (c == 7 && (c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") && c_board[r-2][c-2] == "/")
			{
				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "K";
				cpu -= 1;
				P1minMaxJump(r-2,c-2,c_board,true);
			}

			else
			{
				if (c != 1 && (c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") && c_board[r-2][c-2] == "/")
				{
					bottom_left_board = copyBoard(c_board);
					cpu_bottom_left = cpu;

					c_board[r][c] = "/";
					c_board[r-1][c-1] = "/";
					c_board[r-2][c-2] = "K";
					cpu -= 1;
					P1minMaxJump(r-2,c-2,c_board,true);
					bottom_left = true;
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
						bestVal = value;
				}
				if (c != 6 && c != 7 && (c_board[r-1][c+1] == "W" || c_board[r-1][c+1] == "C") && c_board[r-2][c+2] == "/")
				{
					if (bottom_left)
					{
						bottom_right_board = copyBoard(c_board);
						c_board = copyBoard(bottom_left_board);
						cpu_bottom_right = cpu;
						cpu = cpu_bottom_left;
						bottom_right = true;
						
						c_board[r][c] = "/";
						c_board[r-1][c+1] = "/";
						c_board[r-2][c+2] = "K";
						cpu -= 1;
						P1minMaxJump(r-2,c+2,c_board,true);
						int value = EvaluateBoard(c_board);
						if (value > bestVal)
						{
							c_board = copyBoard(bottom_right_board);
							cpu = cpu_bottom_right;
						}
					}
					else
					{
						c_board[r][c] = "/";
						c_board[r-1][c+1] = "/";
						c_board[r-2][c+2] = "K";
						cpu -= 1;
						P1minMaxJump(r-2,c+2,c_board,true);
					}
				}
			}
		}

		else if ((r != 0 && r != 1 && r != 6 && r != 7) && (c != 0 && c != 1 && c != 6 && c != 7))
		{
			if ((c_board[r-1][c-1] == "W" || c_board[r-1][c-1] == "C") && c_board[r-2][c-2] == "/")
			{
				middle_board = copyBoard(c_board);
				cpu_middle = cpu;

				c_board[r][c] = "/";
				c_board[r-1][c-1] = "/";
				c_board[r-2][c-2] = "K";
				cpu -= 1;
				P1minMaxJump(r-2,c-2,c_board,true);
				middle = true;
				int value = EvaluateBoard(c_board);
				if (value < bestVal)
					bestVal = value;
			}

			if ((c_board[r+1][c-1] == "W" || c_board[r+1][c-1] == "C") && c_board[r+2][c-2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					cpu_middle_temp = cpu;
					cpu = cpu_middle;
					middle_temp = true;
					
					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "K";
					cpu -= 1;
					P1minMaxJump(r+2,c-2,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						cpu = cpu_bottom_right;
					}
				}

				else
				{
					middle_board = copyBoard(c_board);
					cpu_middle = cpu;

					c_board[r][c] = "/";
					c_board[r+1][c-1] = "/";
					c_board[r+2][c-2] = "K";
					cpu -= 1;
					P1minMaxJump(r+2,c-2,c_board,true);
					middle = true;
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
						bestVal = value;
				}
			}

			if ((c_board[r+1][c+1] == "W" || c_board[r+1][c+1] == "C") && c_board[r+2][c+2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					cpu_middle_temp = cpu;
					cpu = cpu_middle;
					middle_temp = true;
					
					c_board[r][c] = "/";
					c_board[r+1][c+1] = "/";
					c_board[r+2][c+2] = "K";
					cpu -= 1;
					P1minMaxJump(r+2,c+2,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						cpu = cpu_bottom_right;
					}
				}

				else
				{
					middle_board = copyBoard(c_board);
					cpu_middle = cpu;

					c_board[r][c] = "/";
					c_board[r+1][c+1] = "/";
					c_board[r+2][c+2] = "K";
					cpu -= 1;
					P1minMaxJump(r+2,c+2,c_board,true);
					middle = true;
					int value = EvaluateBoard(c_board);
					if (value < bestVal)
						bestVal = value;
				}
			}

			if ((c_board[r-1][c+1] == "W" || c_board[r-1][c+1] == "C") && c_board[r-2][c+2] == "/")
			{
				if (middle)
				{
					middle_board_temp = copyBoard(c_board);
					c_board = copyBoard(middle_board);
					cpu_middle_temp = cpu;
					cpu = cpu_middle;
					middle_temp = true;
					
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "K";
					cpu -= 1;
					P1minMaxJump(r-2,c+2,c_board,true);
					int value = EvaluateBoard(c_board);
					if (value > bestVal)
					{
						c_board = copyBoard(middle_board_temp);
						cpu = cpu_bottom_right;
					}
				}

				else
				{
					c_board[r][c] = "/";
					c_board[r-1][c+1] = "/";
					c_board[r-2][c+2] = "K";
					cpu -= 1;
					P1minMaxJump(r-2,c+2,c_board,true);
				}
			}
		}
		
	if (leftside_upper)
		destroy(leftside_upper_board);
	if (leftside_lower)
		destroy(leftside_lower_board);
	if (rightside_upper)
		destroy(rightside_upper_board);
	if (rightside_lower)
		destroy(rightside_lower_board);
	if (top_right)
		destroy(top_right_board);
	if (top_left)
		destroy(top_left_board);
	if (bottom_right)
		destroy(bottom_right_board);
	if (bottom_left)
		destroy(bottom_left_board);
	if (middle)
		destroy(middle_board);
	if (middle_temp)
		destroy(middle_board_temp);

	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//the minimax algorithm to find the best possible for the computer to move to - for more info => www.youngjoshua.com/projects/checkers
int Checkers::minimaxAlg(bool isplayer, std::string**& temp_board,int depth)
{
	if (depth <= 0)
		return EvaluateBoard(temp_board);

	
	if (!isplayer)
	{
		int maxnum = -1000;
		std::map<int,std::vector<int> > cpu_jump;
		if (CPUforceJump(temp_board,cpu_jump))
		{
			maxnum = MinMaxCPUdoJump(temp_board,depth,cpu_jump);
			return maxnum;
		}
		std::map<int,std::vector<int> > cpuNormal_move = getMoves(temp_board,false);
		if(cpuNormal_move.empty())
			return EvaluateBoard(temp_board);

		std::map<int,std::vector<int> >::const_iterator it;
		for (it = cpuNormal_move.begin(); it != cpuNormal_move.end(); it++)
		{
			int r = it->first;
			for (int i = 0; i < it->second.size();i++)
			{
				int c = it->second[i];
				if(temp_board[r][c] == "W")
				{
					if ( r != 7 && c == 0 && temp_board[r+1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						if (r+1 == 7)
							temp_board[r+1][c+1] = "C";
						else
							temp_board[r+1][c+1] = "W";
						maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
						temp_board[r][c] = "W";
						temp_board[r+1][c+1] = "/";
					}
					else if (r != 7 && c == 7 && temp_board[r+1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						if (r+1 == 7)
							temp_board[r+1][c-1] = "C";
						else
							temp_board[r+1][c-1] = "W";
						maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
						temp_board[r][c] = "W";
						temp_board[r+1][c-1] = "/";
					}
					else if (r != 7 && c != 0 && c != 7)
					{
						if (temp_board[r+1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							if (r+1 == 7)
								temp_board[r+1][c+1] = "C";
							else
								temp_board[r+1][c+1] = "W";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "W";
							temp_board[r+1][c+1] = "/";
						}

						if (temp_board[r+1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							if (r+1 == 7)
								temp_board[r+1][c-1] = "C";
							else
								temp_board[r+1][c-1] = "W";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "W";
							temp_board[r+1][c-1] = "/";
						}
					}
				}

				else
				{
					if (c == 0 && r == 7 && temp_board[r-1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r-1][c+1] = "C";
						maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
						temp_board[r][c] = "C";
						temp_board[r-1][c+1] = "/";
					}

					else if (c == 0 && r != 7)
					{
						if(temp_board[r-1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c+1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r-1][c+1] = "/";
						}

						if (temp_board[r+1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c+1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r+1][c+1] = "/";
						}
					}

					else if (c == 7 && r == 0 && temp_board[r+1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						temp_board[r+1][c-1] = "C";
						maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
						temp_board[r][c] = "C";
						temp_board[r+1][c-1] = "/";
					}

					else if (c == 7 && r != 0)
					{
						if (temp_board[r+1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c-1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r+1][c-1] = "/";
						}

						if (temp_board[r-1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c-1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r-1][c-1] = "/";
						}
					}

					else if (r == 7 && c != 0)
					{
						if(temp_board[r-1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c-1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r-1][c-1] = "/";
						}

						if(temp_board[r-1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c+1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r-1][c+1] = "/";
						}
					}

					else if (r == 0 && c != 7)
					{
						if(temp_board[r+1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c-1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r+1][c-1] = "/";
						}

						if(temp_board[r+1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c+1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r+1][c+1] = "/";
						}
					}

					else
					{
						if(temp_board[r+1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c-1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r+1][c-1] = "/";
						}

						if(temp_board[r+1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c+1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r+1][c+1] = "/";
						}

						if(temp_board[r-1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c-1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r-1][c-1] = "/";
						}

						if(temp_board[r-1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c+1] = "C";
							maxnum = std::max(maxnum,minimaxAlg(true,temp_board,depth-1));
							temp_board[r][c] = "C";
							temp_board[r-1][c+1] = "/";
						}
					}
				}
			}
		}
		return maxnum;
	}


	else
	{
		std::map<int,std::vector<int> > p1_jump;
		if (P1_minMaxforceJump(temp_board,p1_jump))
		{
			int min = P1MinMaxdoJump(temp_board,depth,p1_jump);
			return min;
		}
		std::map<int,std::vector<int> > P1Normal_move = getMoves(temp_board,true);
		if (P1Normal_move.empty())
			return EvaluateBoard(temp_board);

		int minnum = 1000;
		std::map<int,std::vector<int> >::const_iterator it;
		for (it = P1Normal_move.begin(); it != P1Normal_move.end(); it++)
		{
			int r = it->first;
			for (int i = 0; i < it->second.size();i++)
			{
				int c = it->second[i];
				if(temp_board[r][c] == "B")
				{
					if ( r != 0 && c == 0 && temp_board[r-1][c+1] == "/")
					{
						temp_board[r][c] = "/";
						if (r-1 == 0)
							temp_board[r-1][c+1] = "K";
						else
							temp_board[r-1][c+1] = "B";
			//			show(temp_board);
						minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
						temp_board[r][c] = "B";
						temp_board[r-1][c+1] = "/";
					}

					else if (r != 0 && c == 7 && temp_board[r-1][c-1] == "/")
					{
						temp_board[r][c] = "/";
						if (r-1 == 0)
							temp_board[r-1][c-1] = "K";
						else
							temp_board[r-1][c-1] = "B";
						minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
						temp_board[r][c] = "B";
						temp_board[r-1][c-1] = "/";
					}
					
					else if (c != 7 && r != 0 && c != 0)
					{
						if (temp_board[r-1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							if (r-1 == 0)
								temp_board[r-1][c+1] = "K";
							else
								temp_board[r-1][c+1] = "B";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "B";
							temp_board[r-1][c+1] = "/";
						}

						if (temp_board[r-1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							if (r-1 == 0)
								temp_board[r-1][c-1] = "K";
							else
								temp_board[r-1][c-1] = "B";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "B";
							temp_board[r-1][c-1] = "/";
						}
					}
				}

				else
				{
					if (c == 0 && r == 7)
					{
						temp_board[r][c] = "/";
						temp_board[r-1][c+1] = "K";
						minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
						temp_board[r][c] = "K";
						temp_board[r-1][c+1] = "/";
					}

					else if (c == 0 && r != 7)
					{
						if(temp_board[r-1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c+1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r-1][c+1] = "/";
						}

						if (temp_board[r+1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c+1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r+1][c+1] = "/";
						}
					}

					else if (c == 7 && r == 0 )
					{
						temp_board[r][c] = "/";
						temp_board[r+1][c-1] = "K";
						minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
						temp_board[r][c] = "K";
						temp_board[r+1][c-1] = "/";
					}

					else if (c == 7 && r != 0)
					{
						if (temp_board[r+1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c-1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r+1][c-1] = "/";
						}

						if (temp_board[r-1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c-1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r-1][c-1] = "/";
						}
					}

					else if (r == 7 && c != 0)
					{
						if(temp_board[r-1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c-1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r-1][c-1] = "/";
						}

						if(temp_board[r-1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c+1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r-1][c+1] = "/";
						}
					}

					else if (r == 0 && c != 7)
					{
						if(temp_board[r+1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c-1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r+1][c-1] = "/";
						}

						if(temp_board[r+1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c+1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r+1][c+1] = "/";
						}
					}

					else
					{
						if(temp_board[r+1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c-1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r+1][c-1] = "/";
						}

						if(temp_board[r+1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r+1][c+1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r+1][c+1] = "/";
						}

						if(temp_board[r-1][c-1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c-1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r-1][c-1] = "/";
						}

						if(temp_board[r-1][c+1] == "/")
						{
							temp_board[r][c] = "/";
							temp_board[r-1][c+1] = "K";
							minnum = std::min(minnum,minimaxAlg(false,temp_board,depth-1));
							temp_board[r][c] = "K";
							temp_board[r-1][c+1] = "/";
						}
					}
				}

			}
		}
		return minnum;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//deallocates the boards that are on the heap
void Checkers::destroy(std::string**& des_board)
{
	if(des_board == NULL)
		return;
	for (int i = 0; i < 8; ++i)
	{
		delete[] des_board[i];
	}
	delete[] des_board;
	des_board = NULL;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//deconstructor
Checkers::~Checkers()
{
	destroy(board);	
}