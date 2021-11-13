#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pthread.h>
#include <string.h>

// x = -1
// o = 1

struct Move {
	int x;
	int y;
};

/*
	make_move_x: makes optimal move for player x
	args:
		int board[3][3]: array to store board
	returns: Move struct that contains the optimal move
*/
Move make_move_x(int board[3][3]);
/*
	make_move_o: makes optimal move for player o
	args:
		int board[3][3]: array to store board
	returns: Move struct that contains the optimal move
*/
Move make_move_o(int board[3][3]);
/*
	minimax: performs the minimax algorithm to find an optimal move
	args:
		int board[3][3]: array to store board
		int depth: keeps track of how many recursive calls have been made until a terminal state has been reached
		bool isMax: boolean determining whether or not it is max's turn
	returns: integer that contains score of terminal state
*/
int minimax(int board[3][3], int depth, bool isMax);
/*
	check_terminal_state: checks whether board is in terminal state
	args:
		int board[3][3]: array to store board
	returns: integer that indicates which terminal state (if any) the board is in
*/
int check_terminal_state(int board[3][3]);
/*
	max: finds the maximum between two integers
	args:
		int a: the first integer
		int b: the second integer
	returns: the integer that is greater
*/
int max(int a, int b);
/*
	max: finds the maximum between two integers
	args:
		int a: the first integer
		int b: the second integer
	returns: the integer that is greater
*/
int min(int a, int b);

/*
	min: finds the minimum between two integers
	args:
		int a: the first integer
		int b: the second integer
	returns: the integer that is lesser
*/
int read_from_file( char* inputfilename, int board[3][3] );

/*
	write_to_file: writes the board from a file
	args:
		char* outputfilename: filename to be written
		int board[4][4][4]: array to store board
	returns: 0 if all is well; -2 if the file cannot be opened for writing
*/
//int write_to_file( char* outputfilename, int board[3][3] );
/*
	NOTE: this function is a modified version of write_to_file which has been removed from this program
	write_board_to_file: writes the board to a file
	args:
		FILE *ofile: file that will be written to
		int board[3][3]: array to store board
	returns: void
*/
void write_board_to_file( FILE *ofile, int board[3][3] );
/*
	write_move_to_file: writes the move to file
	args:
		FILE *ofile: file that will be written to
		int board[3][3]: array used for the board
	returns: void
*/
void write_move_to_file(FILE *ofile, Move m);
/*
	print_board_to_screen: prints the board to the screen
	args:
		int board[3][3]: array used for the board
	returns: void
*/
void print_board_to_screen( int board[3][3] );


/*
	print_move_to_screen: prints the move chosen to the screen
	args:
		Move m: struct storing the x,y coordinate of the square to move to
	returns: void
*/
void print_move_to_screen( Move m);


int main( int argc, char* argv[] )
{
	char* inputfilename = NULL, *outputfilename = NULL;
	int board[3][3];
	Move m = {0,0};

	// parse command-line arguments
	for( int i = 1; i < argc; i++ )
	{
		// input file
		if( !strcmp(argv[i], "-i" ) )
		{
			inputfilename = argv[i+1];
			i++;
		}
		// output file
		else if( !strcmp( argv[i], "-o") )
		{
			outputfilename = argv[i+1];
			i++;
		}
	}

	// check to make sure command-line arguments have been specified
	if( inputfilename == NULL || outputfilename == NULL )
	{
		printf( "input and output filenames need to be specified on the command line (-i <filename> -o <filename>\n");
		return -1;
	}

	// debug info
	//printf( "input file: [%s]\n", inputfilename);
	//printf( "output file: [%s]\n", outputfilename);

	// read from file
	read_from_file( inputfilename, board );

	// debug
	//printf( "starting tictactoe\n");

	//TODO: MAKE MOVE HERE
	//sleep(5);

	// file now opens here instead of in write_to_file
	FILE *ofile = fopen( outputfilename, "w" );
	if( !ofile )
	{
		printf( "could not open output file [%s] for writing\n", outputfilename );
		return -2;
	}

	// 0 = .
	//-1 = x
	// 1 = o
	
	bool Xturn = false; // o goes first	

	// loop while a terminal state is not reached
	while(check_terminal_state(board)==0)
	{
		// if its player x's turn perform make_move_x and write move to board
		if(Xturn == true)
		{
			m = make_move_x(board);
			board[m.x][m.y]=-1;
		}
		// if its player o's turn perform make_move_o and write move to board
		else
		{
			m = make_move_o(board);
			board[m.x][m.y]=1;
		}

		//debug into
		//printf( "finishing tictactoe\n");
		
		// write move to screen and output file
		print_move_to_screen( m );
		write_move_to_file(ofile, m);
		//fprintf( ofile, "move chosen: %d %d\n\n", m.x, m.y);
		print_board_to_screen( board);
		write_board_to_file(ofile, board);

		Xturn = !Xturn; // next players turn
	}
	// output to screen and output file based on terminal state
	switch(check_terminal_state(board))
	{
		case 10: 
			printf("Player x wins!\n"); 
			fprintf( ofile, "Player x wins!\n");
			break;
		case -10: 
			printf("Player o wins!\n"); 
			fprintf( ofile, "Player o wins!\n");
			break;
		case 11: 
			printf("Game is a draw!\n"); 
			fprintf( ofile, "Game is a draw!\n");
			break;
	}	

	// file now closes here instead of in write_to_file
	fclose (ofile);

	return 0;
}

Move make_move_x(int board[3][3])
{
	int bestValue = -1000; // I have made x max, so set initial best value to an arbitrary large negative number
	Move optimalMove;
	optimalMove.x = -1;
	optimalMove.y = -1;

	// check for winning move at each space
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(board[i][j] == 0)
			{
				board[i][j] = -1;
				if(check_terminal_state(board) == 10)
				{
					optimalMove.x = i;
					optimalMove.y = j;
					return optimalMove;
				}
				board[i][j] = 0;
			}
		}
	}

	// if no winning move perform minimax
	// use minimax algorithm for each empty space
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(board[i][j] == 0)
			{
				board[i][j] = -1; // mark current space with x
				int moveValue = minimax(board, 0, false); // mini's turn with newly updated board
				board[i][j] = 0; // unmark current space
				// if resulting value is more than current best value
				// set optimal move to current space and set best value
				// to resulting value
				if (moveValue > bestValue) 
				{ 
					optimalMove.x = i; 
					optimalMove.y = j; 
					bestValue = moveValue; 
				} 
			}
		}
	}

	return optimalMove; 
}

Move make_move_o(int board[3][3])
{
	int bestValue = 1000; // I have made o mini, so set initial best value to an arbitrary large positive number
	Move optimalMove;
	optimalMove.x = -1;
	optimalMove.y = -1;

	// check for winning move at each space
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(board[i][j] == 0)
			{
				board[i][j] = 1;
				if(check_terminal_state(board) == -10)
				{
					optimalMove.x = i;
					optimalMove.y = j;
					return optimalMove;
				}
				board[i][j] = 0;
			}
		}
	}

	// if no winning move perform minimax
	// use minimax algorithm for each empty space
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(board[i][j] == 0)
			{
				board[i][j] = 1; // mark current space with o
				int moveValue = minimax(board, 0, true); // max's turn with newly updated board
				board[i][j] = 0; // unmark current space
				// if resulting value is less than current best value
				// set optimal move to current space and set best value
				// to resulting value
				if (moveValue < bestValue) 
				{ 
					optimalMove.x = i; 
					optimalMove.y = j; 
					bestValue = moveValue; 
				} 
			}
		}
	}

	return optimalMove; 
}

int minimax(int board[3][3], int depth, bool isMax) 
{
	// utility
	int score = check_terminal_state(board);
	if(isMax) // for max
	{
		if(score == 10)
			return score - depth; // if max wins we want shortest path to a win
		if(score == -10)
			return score + depth; // if min wins we want longest path to a win
		if(score == 11)
			return 0; // tie so no points to mini or max
	}
	else // for mini
	{
		if(score == 10)
			return score - depth; // if max wins we want longest path to a win
		if(score == -10)
			return score + depth; // if min wins we want shortest path to a win
		if(score == 11)// tie so no points to mini or max
			return 0;
	}

	// Max's turn
	if(isMax)
	{
		int best = -1000; // since max, set current best score to large negative number
		// use minimax algorithm for each empty space
		for (int i=0;i<3;i++)
		{
			for (int j=0;j<3;j++)
			{
				if(board[i][j]==0)
				{
					board[i][j] = -1; // mark current space with x
					best = max(best, minimax(board, depth+1, !isMax));// mini's turn with newly updated board
					board[i][j] = 0; // unmark current space
				}
			}
		}
		return best;
	}
	// Mini's turn
	else
	{
		int best = 1000; // since min, set current best score to large positive number
		// use minimax algorithm for each empty space
		for (int i=0;i<3;i++)
		{
			for (int j=0;j<3;j++)
			{
				if(board[i][j]==0)
				{
					board[i][j] = 1; // mark current space with o
					best = min(best, minimax(board, depth+1, !isMax));// max's turn with newly updated board
					board[i][j] = 0; // unmark current space
				}
			}
		}
		return best;
	}
}

int check_terminal_state(int board[3][3])
{
	// Check for x and o wins
	// return -10 for o win
	// return 10 for x win
	// return 11 for tie
	// return 0 if not in terminal state (board is not filled)
	// rows
	for (int row = 0; row<3; row++) 
	{ 
		if (board[row][0]==board[row][1] && board[row][1]==board[row][2]) 
		 { 
   			if (board[row][0]==-1) 
				return 10; 
			else if (board[row][0]==1) 
 				return -10; 
		} 
	}
	// cols
	for (int col = 0; col<3; col++) 
	{ 
		if (board[0][col]==board[1][col] && board[1][col]==board[2][col]) 
		{ 
			if (board[0][col]==-1) 
				return 10; 
  
			else if (board[0][col]==1) 
				return -10; 
		} 
	}
	// diagonals
	if (board[0][0]==board[1][1] && board[1][1]==board[2][2]) 
	{ 
		if (board[0][0]==-1) 
			return 10; 
		else if (board[0][0]==1) 
			return -10; 
	} 
  
	if (board[0][2]==board[1][1] && board[1][1]==board[2][0]) 
	{ 
		if (board[0][2]==-1) 
			return 10; 
		else if (board[0][2]==1) 
			return -10; 
	} 

	//check if board is not filled
	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			if (board[i][j]==0) 
				return 0; // board is not filled
		}
	}

	return 11; // only other option is that board is filled, 11 will represent a tie
}

int max(int a, int b)
{
	// if a>b then its the max, else max is b
	if (a>b)
		return a;
	else
		return b;
}

int min(int a, int b)
{
	// if a<b then its the min, else min is b
	if (a<b)
		return a;
	else
		return b;
}

int read_from_file( char* inputfilename, int board[3][3] )
{
	FILE *ifile = fopen( inputfilename, "r" );
	if( !ifile )
	{
		printf( "could not open input file [%s] for reading\n", inputfilename );
		return -2;
	}

	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			char c = '.';
			fscanf(ifile, " %c", &c );
			switch( c )
			{
				case '.': board[i][j] = 0; break;
				case 'x': board[i][j] = -1; break;
				case 'o': board[i][j] = 1; break;
				default: board[i][j] = 0; break;
			}

		}
	}

	fclose( ifile );
	return 0;
}

void write_board_to_file( FILE *ofile, int board[3][3] )
{
	//FILE *ofile = fopen( outputfilename, "w" );
	/*if( !ofile )
	{
		printf( "could not open output file [%s] for writing\n", outputfilename );
		return -2;
	}*/

	// iterate through all squares on the board
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			// write the appropriate character to the file
			switch( board[i][j])
			{
				case 0: fprintf( ofile, "%c", '.'); break;
				case 1: fprintf( ofile, "%c", 'o'); break;
				case -1: fprintf( ofile, "%c", 'x'); break;
				default: fprintf( ofile, "%c", '.'); break;
			}
		}
		fprintf( ofile, "\n");
	}

	// close the output file
	//fclose (ofile);
	//return 0;
}

void write_move_to_file(FILE *ofile, Move m)
{
	// print move chosen to ofile
	fprintf( ofile, "move chosen: %d %d\n\n", m.x, m.y);
}

void print_board_to_screen( int board[3][3] )
{
	// iterate through all squares on the board
	for( int i = 0; i < 3; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			// print the appropriate character to the screen
			switch( board[i][j])
			{
				case 0: printf( "%c", '.'); break;
				case 1: printf( "%c", 'o'); break;
				case -1: printf( "%c", 'x'); break;
				default: printf( "%c", '.'); break;
			}
		}
		printf( "\n");
	}

}

void print_move_to_screen( Move m)
{
	printf( "move chosen: %d %d\n\n", m.x, m.y);
}
