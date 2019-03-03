#include "Checkers.h"

using namespace std;
void checkValid(int& rwplayer,int& colplayer,int& rwmove,int& colmove);
void checkValid(int& difficulty);


int main(int argc, char const *argv[])
{
	int rwplayer, colplayer;
	int rwmove, colmove;
	int difficulty;
	int player_score, pc_score;
	bool game_over;
	bool jump = false;
	bool gameStart;
	std::map<int,std::vector<int> > mp_jump;
	std::map<int,std::vector<int> > mp_currentMoves;
	cout << "Choose the difficulty: " << endl;
	cout << "1 = Easy" << endl;
	cout << "2 = Medium" << endl;
	cout << "3 = Hard" << endl;
	cin >> difficulty;
	while(cin.fail())
	{
		cout << "Please enter a number (1-3)" << endl;
		cin.clear();
		cin.ignore(256,'\n');
		cin >> difficulty;
	}
	checkValid(difficulty);
	Checkers game(difficulty);
	game.instructions();
	game.createBoard();
	game.showBoard();
	cin >> rwplayer >> colplayer >> rwmove >> colmove;
	while (cin.fail())
	{
		cout << "Please enter a number (0-7)" << endl;
		cin.clear();
		cin.ignore(256,'\n');
		cin >> rwplayer >> colplayer >> rwmove >> colmove;
	}
	checkValid(rwplayer,colplayer,rwmove,colmove);
	gameStart = game.playerMove(rwplayer,colplayer,rwmove,colmove);
	while ( true)
	{
		if (jump)
		{
			cin >> rwplayer >> colplayer >> rwmove >> colmove;
			while (cin.fail())
			{
				cout << "Please enter a number (0-7)" << endl;
				cin.clear();
				cin.ignore(256,'\n');
				cin >> rwplayer >> colplayer >> rwmove >> colmove;
			}
			checkValid(rwplayer,colplayer,rwmove,colmove);
			int go = game.doJump(rwplayer,colplayer,rwmove,colmove);
			while (go == 0 || go == -1)
			{
				if (go == 0)
				{
					game.showBoard();
					cout << "Keep jumping" << endl;
				}
				else
				{
					cout << "You entered an invalid move" << endl;
					mp_jump = game.return_jumpMoves();
					std::map<int,std::vector<int> >::iterator it;
					cout << "There's a force jump at:" << endl;
					for (it = mp_jump.begin(); it != mp_jump.end();it++)
					{
						cout << "row: " << it->first << endl;
						cout << "col: ";
						for (int i = 0; i < it->second.size(); i ++)
						{
							cout << it->second[i] << " ";
						}
						cout << endl;
					}

				}
				cin >> rwplayer >> colplayer >> rwmove >> colmove;
				while (cin.fail())
				{
					cout << "Please enter a number (0-7)" << endl;
					cin.clear();
					cin.ignore(256,'\n');
					cin >> rwplayer >> colplayer >> rwmove >> colmove;
				}
				checkValid(rwplayer,colplayer,rwmove,colmove);
			    go = game.doJump(rwplayer,colplayer,rwmove,colmove);
			}
		}
		while (!gameStart && !jump)
		{
			cout << "Enter a different place" << endl;
			cin >> rwplayer >> colplayer >> rwmove >> colmove;
			while (cin.fail())
			{
				cout << "Please enter a number (0-7)" << endl;
				cin.clear();
				cin.ignore(256,'\n');
				cin >> rwplayer >> colplayer >> rwmove >> colmove;
			}
			checkValid(rwplayer,colplayer,rwmove,colmove);
			gameStart = game.playerMove(rwplayer,colplayer,rwmove,colmove);
		}
		game.showBoard();
		pc_score = game.return_cpu_score();
		if(pc_score == 0)
		{
			cout << "Game Over! You Win!" << endl;
			return 0;
		}
		cout << "Waiting for CPU to make a move..." << endl;
		game.CpuMove(game_over);
		if (game_over)
		{
			cout << "Game Over! You Win!" << endl;
			return 0;	
		}
		game.showBoard();
		player_score = game.return_player_score();
		if(player_score == 0)
		{
			cout << "Game Over! CPU Wins!" << endl;
			return 0;
		}
		mp_currentMoves = game.returnMoves();
		if (mp_currentMoves.empty())
		{
			cout << "NO MORE MOVES! CPU Wins!" << endl;
			return 0;
		}
		cout << "Your turn..." << endl;
		jump = game.P1_forcejump();
		if (!jump)
		{
			cin >> rwplayer >> colplayer >> rwmove >> colmove;
			while (cin.fail())
			{
				cout << "Please enter a number (0-7)" << endl;
				cin.clear();
				cin.ignore(256,'\n');
				cin >> rwplayer >> colplayer >> rwmove >> colmove;
			}
			checkValid(rwplayer,colplayer,rwmove,colmove);
			gameStart = game.playerMove(rwplayer,colplayer,rwmove,colmove);
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//checks the make sure the inputted values are valid
void checkValid(int& rwplayer,int& colplayer,int& rwmove,int& colmove)
{
	while ((rwplayer < 0 || rwplayer > 7) || (colplayer < 0 || colplayer > 7) || (rwmove < 0 || rwmove > 7)
		|| (colmove < 0 || colmove > 7))
	{
		cout << "Please enter a number between (0-7) inclusive for all" << endl;
		cin >> rwplayer >> colplayer >> rwmove >> colmove;
		while (cin.fail())
		{
			cout << "Please enter a number (0-7)" << endl;
			cin.clear();
			cin.ignore(256,'\n');
			cin >> rwplayer >> colplayer >> rwmove >> colmove;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//same as above
void checkValid(int& difficulty)
{
	while (difficulty != 1 && difficulty != 2 && difficulty != 3)
	{
		cout << "Please enter 1, 2, or 3" << endl;
		cin >> difficulty;
		while(cin.fail())
		{
			cout << "Please enter a number (1-3)" << endl;
			cin.clear();
			cin.ignore(256,'\n');
			cin >> difficulty;
		}
	}
}
