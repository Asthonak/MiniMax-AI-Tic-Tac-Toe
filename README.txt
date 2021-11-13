Ethan Park
CS 482
Fall 2019
Homework Assignment 2

This directory contains the following files:
+-- MiniMax-AI-Tic-Tac-Toe
|	+--	board.in
|	+--	board.out
|	+--	board2.in
|	+--	board3.in
|	+--	makefile
|	+--	minimax_tictactoe.cpp
|	+--	README.txt
|	+--	tictactoe

This directory contains a program, minimax_tictactoe.cpp, to conduct a tictactoe game which in which both players are AI using the minimax algorithm. The program's ability to conduct a full game was made by an undergraduate student and as such should count as extra credit for this assignment (I have competed all other problems for this assignment). The algorithm currently gives -10 points to mini for winning states and 10 points for max for winning states. Depth of the winning state is added or subtracted depending on whose turn it is and whether that state results in a win for mini or max. The purpose of adding and subtracting depth is to ensure that mini and max try and win the game as fast as possible (the win state with the shallowest depth) or if a senario is unwinable for mini or max, they will still attempt to play as long as possible (the longest depth for opponents win state). To test the ladder senario, I have included board2.in and board3.in. board2.in results in a state that o will always win if played optimally, but demonstrates that x will play for as long as possible. board3.in results in a state that x will always win if played optimally, but demonstrates that o will play for as long as possible. 

To compile this code change to the directory to HW2, and simply type make. To run this code enter ./tictactoe -i <infile> -o <outfile> where infile is the file containing the initial board state and where outfile outputs the result of thetictac toe match.

No additional libraries apart from the ones already included in the minimax_tictactoe.cpp template file were used.

It was assumed that o would always go first when conducting a full game of tictactoe. It was also assumed that player x is max and player o is mini, although this assumpition should not affect output. Finally, it was assumed that if a newly found score has the same value as the current best score, the move with current best score will be used.

Additional functions used besides make_move and minimax were functions were check_terminal_state, max, min, and write_move_to_file. It should be noted that make_move was split into 2 functions make_move_x and make_move_o which makes the optimal moves for x and o respectively. check_terminal_state is used to analyze the board to check 4 possible conditions: player x wins, player o wins, the game is tied (no more moves), or there is still moves to be done (terminal state fails). The min and max functions finds the minimum between two integers and maximum between two integers respectively.

It should be noted that write_to_file was modified and renamed to write_board_to_file. The function now takes an output file rather than the name of an output file with the file being opened and closed in main rather than in write_board_to_file. This was done so that write_board_to_file could be called every turn to write a new board to the file rather than open, rewrite the exisiting board, and close the file.
