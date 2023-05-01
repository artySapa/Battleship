# Battleship

https://user-images.githubusercontent.com/118086930/235388734-0e9c1e71-02ef-4c9e-ba34-00e69fca2487.mp4

## **General Description** 
Battleship game is a very famous game where there are two players trying to explode each other's battleship crew. Whoever destroy's all the ships first wins. 
In my particular implementation, player can choose 3 options: 
1.  A mini-game between two mediocre players
2.  A mediocre player against a human player
3.  A 100-game match between a mediocre and an awful player, with no pauses
Human player will be promted to set up the desk of ships and then play against a mediocre player, which has a secret implementation of the actions. 
Good player is my own implementation of an algorithm that is attacking its opponents' ships.


## **Data structures design**

Board. 
I represented the board in an array of characters(m_board[MAXROWS][MAXCOLS]). In the constructor, I initialized it using 2 loops and set each character on the board to ‘.’ My board object also had a track of the game’s rows, columns, and numbers of ships. 
When a shot is made, I check if the cell was already attacked(or if the board’s value at the attacked cell is either ‘X’ or ‘o’), and, then if the cell is valid and all the requirements are met, I change the attacked cell’s character to ‘X’ if the attack hit a ship and ‘o’ if the attack missed. 
For placing ships on the board, I check if the ship is not going to go out of the board(looking at the direction, length, and rows/columns of the board). Then, if all the requirements are met, I fill in the points with the given symbol of the ship that was requested to be placed.

Game.
Mostly, the game class takes track of the ships in the game. I created its own struct Ship, where I put the ship’s length, symbol, and name. To take track of all the ships in the game, I used a vector m_ships. I also took track of the number of ships in the game(mn_ships), as well as of the rows and columns in the game. 
When adding a ship to the game, I created a Ship object, updated all of its elements,
and pushed it to the vector m_ships. Finally, I increased the number of ships in the game by 1.

Players.
Human Player.
My human Player did not have any private members, as in the recommendAttack() function it simply passed all the values as soon as it was asked by the game’s play function to recommend an attack. In other words, after human cins the coordinates, they are simply returned and then used by the game’s play function. In the placeShips() function, the function also took the values from the human, checked if they are all right, placed them on the board, and displayed the board.

Mediocre Player.
The mediocre player has several members in its structure. It has the recursive place function, which is the recursive placing of the ships on the board and a track of placed ships to allow recursion to happen(I will explain that in the pseudocode for this function better). Then, the Mediocre player has its own track of the attacks that were made, implemented by the board of the same size as the game’s board, but only indicating the places that were attacked by the player in any way( I used the character ‘#’ to indicate that). I also used the variable m_state initialized to 1 to track the state the player is in. Finally, I tracked the last point that was hit by the attack and only updated it when a completely new hit was made(or, when switching from state 1 to state 2). After we switch to the 2nd state, we use the last hit point to create a cross of attacking, and add it to the vp vector of points(also checking if the cross parts are on the board and were not attacked before(using attacks array)for each direction). In state 2, we randomly choose a point from the vector, record that we attacked it in the attacks array, erase the point from the cross, and return the point. After we attacked all the points from the cross(), we switch back to state 1 and randomly attack valid points.

Good Player.
For the Good player, I used the same approach to place the ships as in the Mediocre Player, but I implemented the attacking differently. So, my idea was to attack the diagonals of the board when we are in state 1, and after we hit any part of the ship, switch to state 2. In the 2nd state, we do the cross procedure as in Mediocre player, but now only in 1 point far from the last hit. Then, if we hit any part of a ship, we switch to state 3, where we attack the entire line of the ship until the ship is destroyed. That was implemented with the vector line. If the cross or the line is empty, we switch back to the first state and attack diagonals as follows:
>		.#.#.#.#.#
>		#.#.#.#.#.
>		.#.#.#.#.#
>		#.#.#.#.#.
>		.#.#.#.#.#
>		#.#.#.#.#.
‘#’ cells go first, after we are done with them, we attack ‘.’ cells. That is implemented with two vectors, from which we take the coordinates of diagonals to attack. The toShoot vector contains only ‘#’ points, and the toShootAfter vector contains only ‘.’ points. If all the vectors are empty, we return the random unattacked point to get rid of any problems with the shooting at the later stages.

GoodPlayer’s strategies.
bool placeShips()
Tries 50 times to randomly block half of the cells of the board and call recursivePlace(Board& b, 0) helper function on the board. If succeded, returns the point, if not, it tries one more time.

bool recursivePlace(Board& b, int placedShips)
Base case - if the number of placed ships is equal to the number of the ships on the board – returns true. If not, for each cell on the board, the function tries to place a ship horizontally or vertically and calls itself after placing a ship, increasing the number of ships placed. If the recursive call returns false, we unplace the ship from the cell and try a different direction. If nothing worked, the function returns false.

void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
Does nothing if the shot was invalid. Initially, in state 1. After the player hits a ship, it switches to state 2 and records the last hit cell for later use. Then, in the process of switching to state 2, it also clears the cross from previous actions made and fulfills it with a small cross(1 to each direction). If the player is in state 2 and does not hit anything, it stays in the state. If it hits anything while being in state 2, it switches to state 3 and records the entire line created by two points hit. If in state 3, it misses, it stays in state 3. If in state 3, the player hits something, it continues hitting the line. If, in state 3, the player destroyed a ship, the player switches to the  state 1.

Point recommendAttack()
Return points to attack with respect to the state given. If the player is in state 1, the function will return a point from the toShoot or toShootAfter if it’s valid and was not attacked before and remove the point after the attack was made, after which return the point, or, a random unattacked point if the cross is empty. If the player is in state 2, it randomly picks a point from the cross and returns it. In state 3, it returns a point from the line and erases it, or, a random unattacked point if the line is empty.

## **Pseudocode for non-trivial functions**

### BoardImpl

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)

	if the point or shipId is not valid
		return false

	For each point on the board:
		If the symbol is at the point
			Return false

	If the direction is vertical and there are enough rows to fit in the ship
	For each row under the top point:
		If the point is not ‘.’ 
			Return false
	For each row under the top point
		Change the character of the point to the symbol
	Return True

	If the direction is horizontal and there are enough columns to fit in the ship
	For each column to the right of the left point:
		If the point is not ‘.’ 
			Return false
	For each column to the right of the left point:
		Change the character of the point to the symbol
	Return True
	Else
		Return False;


> Remark: bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir) acts the same, but removes the symbol.



void BoardImpl::display(bool shotsOnly) const

	Print the space
	Print the number of each column(“0123….n”)

	Print newline

	If asked to show everything
		For each row:
			Print the row value and a space
			For each column:
				Print the value of the board at the point
			Print newline
	Else
		For each row:
			Print the row value and a space
			For each column:
				If the value of the board is not ‘.’
					Print the value of the board
				Else
					Print ‘.’
			Print newline


bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)

	If the point is not valid or was already attacked
		Return false

	If the symbol is a symbol of a ship
		Update it to ‘X’
		Update shotHit to true
		Update temporary variable to the attacked ship’s id
		For each cell on the board
			If there is any part of the ship left
				Update shipDestroyed to false
				Return true
			Else
				Update shipId to the temporary variable
				Update shipDestroyed to true
				Return true
	Else if the symbol is ‘.’
		Update the cell value to ‘o’
		Update needed variables to false
	If the ship was destroyed, reduce the number of ships by 1

###  GameImpl

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)

	If players’ ships cannot be placed
		Return nullptr

	If anyone is a human wait to enter

	While there are any ships left
		Player 1’s turn
		Show Player 2’s board(if p1 is a human only show shot)
		Take the first player’s recommend attack
		Attack the second player’s board
		Record attack result for the 1st player
		Print out the result of the attack
		If the player is human
			Wait for enter

		If anyone wins and the losing player is human
			Show the winner’s board

		Player 2’s turn
		Show Player 1’s board(if p2 is a human only show shot)
		Take the second player’s recommend attack
		Attack the first player’s board
		Record attack result for the 2nd player
		Print out the result of the attack
		If the player is human
			Wait for enter

	If anyone wins and the losing player is human
			Show the winner’s board

	Return nullptr(as there are no ships initially)


###  HumanPlayer

bool HumanPlayer::placeShips(Board& b)

	Display the board
	For each ship in the game
	{
		Ask Human for 2 coordinates to place a ship at
		If the direction is valid
			Set direction to the given direction
		Else
			Ask again using recursion

		Ask for two coordinates to place ships
		While the coordinates are not valid
			State that the coordinates are not valid
			Ask again

		Display board

	}
	Return true, as there are 0 ships in the game and all of them are placed(or none of them are placed);

Point HumanPlayer::recommendAttack()

	Ask the humanPlayer for two numbers that represent row and column
	Return the point with the row and column

###  MediocrePlayer

bool MediocrePlayer::placeShips(Board& b)

	For 50 times
	{
		Randomly block the board
		If the recursivePlace function for the board return true
			Unblock the board
			Return true
		Else
			Try again
	}
	Unblock the board
	Return result(false if 50 times was not enough)

bool MediocrePlayer::recursivePlace(Board& b, int placedShips)

	If placedShips equal to the number of ships in the game
		Return true

	For each row on the board:
		For each column on the board:
			If board’s placeShips of the ship with index placedShips for Horizontal direction              returns true
				If recursive place for the next ship return false
					Unplace current ship
				Else
					Return the recursive place for the next ship
			If board’s placeShips of the ship with index placedShips for Vertical direction              returns true
				If recursive place for the next ship return false
					Unplace current ship
				Else
					Return the recursive place for the next ship

	Return false if nothing worked


Point MediocrePlayer::recommendAttack()

	If the cross is empty
		Switch to the state 1

	If we are in the state 1
		Create a random point
		While the point is not valid or already attacked
			Create another random point
		Attack the point on the private attacks board
		Return the point
	If we are in the state 2
		Randomly take a point from the cross
		Attack the point on the private attacks board
		Erase the point from the vector
		Return the point

	Return any point (does not matter, the program won’t go here)

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)


	If the shot was not valid
		Do nothing

	If we are in the state 1 and we did not hit anything
		Stay in the state 1

	Else If we are in the state 1 and we hit something and destroyed a ship
		Stay in the state 1

	Else If we are in the state 1 and we did hit something but did not destroy a ship
		Switch to state 2
		Record the last hit point
		Clear the cross
		For each horizontal line(+- 4 cells)
			Add a point to the vector if it is not the point we already hit
	For each vertical line(+- 4 cells)
			Add a point to the vector if it is not the point we already hit
		If the process did not change the cross because everything was hit before
			Change to state 1

	Else If we are in the state 2 and we did not hit anything
		Stay in the state 2

	Else If we are in the state 2 and we hit something and destroyed a ship
		Switch to the state 1
		Clear the cross

	Else If we are in the state 2 and we did hit something but did not destroy a ship
	Stay in state 2

###  GoodPlayer

GoodPlayer::GoodPlayer(string name, const Game& g): Player(name, g)

	For each row in the board
		If the row number is odd
			If the column is odd
				Add the point to toShootAfter
			Else
				Add the point to toShoot
		If the row number is even
			If the column is odd
				Add the point to toShoot
			Else
				Add the point to toShootAfter



bool GoodPlayer::placeShips(Board &b) (the same as Mediocre player)

	For 50 times
	{
		Randomly block the board
		If the recursivePlace function for the board return true
			Unblock the board
			Return true
		Else
			Try again
	}

	Unblock the board
	Return result(false if 50 times was not enough)


bool GoodPlayer::recursivePlace(Board& b, int placedShips)  (the same as for Mediocre player)

	{
	If placedShips equal to the number of ships in the game
		Return true

	For each row on the board:
		For each column on the board:
			If board’s placeShips of the ship with index placedShips for Horizontal direction              returns true
				If recursive place for the next ship return false
					Unplace current ship
				Else
					Return the recursive place for the next ship
			If board’s placeShips of the ship with index placedShips for Vertical direction              returns true
				If recursive place for the next ship return false
					Unplace current ship
				Else
					Return the recursive place for the next ship

	Return false if nothing worked
	}


Point GoodPlayer::recommendAttack()

	If the cross is empty
		Switch to the state 1

	If we are in the state 1
		If toShoot is not empty
			Set a point to the first point from toShoot
			While toShoot is not empty and its size is not 1 and we have already attacked the point
				Erase the point from the vector
				Set the point to the first point from toShoot
			Attack the point
			Erase the point from the vector
			Return the point
		Else If toShootAfter is not empty
			Set a point to the first point from toShootAfter
			While toShootAfter is not empty, its size is not 1 and we have already attacked the point
				Erase the point from the vector
				Set the point to the first point from toShootAfter
			Attack the point
			Erase the point from the vector
			Return the point
		Else
			Return random not attacked point

	Else If we are in the state 2
		Randomly take a point from the cross
		Attack the point on the private attacks board
		Erase the point from the vector
		Return the point

	Else if we are in the state 3
		If line is not empty
			Set a point to the first point in the line
			While the line is not empty and its size is not 1 and we have already attacked the point
				Erase the point from line
				Set the point to the first element in line vector
			Attack the point
			Erase the point from line vector
			Return point
		Else
			Return random not attacked point

	Return any point (does not matter, the program won’t go here)

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)

	If the shot was not valid
		Do nothing

	If we are in the state 1 and we did not hit anything
		Stay in the state 1

	Else If we are in the state 1 and we hit something and destroyed a ship
		Stay in the state 1

	Else If we are in the state 1 and we did hit something but did not destroy a ship
		Switch to state 2
		Record the last hit point
		Clear the cross
		For each horizontal line(+- 1 cell)
			Add a point to the vector if it is not the point we already hit
	For each vertical line(+- 1 cell)
			Add a point to the vector if it is not the point we already hit
		If the process did not change the cross because everything was hit before
			Change to state 1

	Else If we are in the state 2 and we did not hit anything
		Stay in the state 2

	Else If we are in the state 2 and we hit something and destroyed a ship
		Switch to the state 1
		Clear the cross

	Else If we are in the state 2 and we did hit something but did not destroy a ship
	Change to state 3
	Clear the line
	If the hit was on the horizontal line with the last hit
		For each horizontal line(+- 4 cells)
			Add a point to the line vector if it is not the point we already hit
	For each vertical line(+- 4 cells)
			Add a point to the line vector if it is not the point we already hit

	Else If we are in the state 3 and we did not hit anything
		Stay in state 3

	Else If we are in the state 3 and we did hit something and destroyed the ship
		Change to state 1
		Clear the line
