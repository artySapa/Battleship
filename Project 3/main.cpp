
// Project 3
// CS 32
// Sapa Artyom

#include "Game.h"
#include "Player.h"
#include "Board.h"
#include <iostream>
#include <string>

using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

int main()
{
    const int NTRIALS = 100;

    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A mediocre player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
         << "-game match between a mediocre and an awful player, with no pauses"
         << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    else if (line[0] == '1')
    {
        Game g(2, 3);
        g.addShip(2, 'R', "rowboat");
        Player* p1 = createPlayer("mediocre", "Popeye", g);
        Player* p2 = createPlayer("mediocre", "Bluto", g);
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '3')
    {
        int nMediocreWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("good", "Good Audrey", g);
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p2)
                nMediocreWins++;
            delete p1;
            delete p2;
        }
        cout << "The mediocre player won " << nMediocreWins << " out of "
             << NTRIALS << " games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
    else
    {
       cout << "That's not one of the choices." << endl;
    }
}


/*int main()
{
    cout << "Start" << endl;
    Game game(10, 10);
    // game.addShip(4, 'A', "submarine");
    //game.addShip(3, 'B', "battleship");
    //game.addShip(2, 'P', "patrol boat");
    game.addShip(5, 'A', "aircraft carrier");
    game.addShip(4, 'B', "battleship");
    game.addShip(3, 'D', "destroyer");
    game.addShip(3, 'S', "submarine");
    game.addShip(2, 'P', "patrol boat");
    /*assert(game.rows() == 10);
    assert(game.cols() == 10);
    assert(game.nShips() == 2);
    assert(game.shipLength(0) == 3);
    assert(game.shipLength(1) == 4);
    assert(game.shipSymbol(0) == 'A');
    assert(game.shipSymbol(1) == 'B');
    assert(game.shipName(1) == "mirage");
    
    Point p(0,0);
    Point p2(0,1);
    Point p3(0,2);
    Point p4(5, 5);
    Board board(game);
    board.placeShip(p, 0, HORIZONTAL);
    //board.placeShip(p4, 1, HORIZONTAL);
    
    bool shotHit = false;
    bool shipDestroyed = false;
    int shipId = -1;
    
    // board.display(false);
    
    flflfl assert(board.unplaceShip(p, 0, HORIZONTAL) == true);
    assert(board.unplaceShip(p, 0, VERTICAL) == false);
    
    assert(board.unplaceShip(p4, 1, HORIZONTAL) == true);
    assert(board.unplaceShip(p4, 1, VERTICAL) == false); flflfl


    
    // board.display(false);
    //cout << "======================" << endl;
    board.attack(p, shotHit, shipDestroyed, shipId);
    assert(shipDestroyed == false);
    board.attack(p2, shotHit, shipDestroyed, shipId);
    board.attack(p3, shotHit, shipDestroyed, shipId);
    assert(shipDestroyed == true);
    board.attack(p4, shotHit, shipDestroyed, shipId);
    // board.display(true);
    // cout << "======================" << endl;
    
    board.clear();
    //board.display(true);
    
    board.block();
    // board.display(false);
    
    //cout << "======================" << endl;
    
    board.unblock();
    
    Player* Arty = createPlayer("human", "Arty", game);
    Player* Awful = createPlayer("awful", "Maia", game);
    //Arty->placeShips(board);
    //board.display(false);
    game.play(Arty, Awful);
    //cout << "======================" << endl;
    

    Board board(game);
    Player* Mediocre = createPlayer("mediocre", "TEST", game);
    Player* Awful = createPlayer("awful", "Maia", game);
    
    game.play(Mediocre, Awful);
    //Mediocre->placeShips(board);
    //board.display(false);
    =======
    Board board(game);
    Player* Mediocre = createPlayer("mediocre", "TEST", game);
    int count = 0;
    
    for(int i = 0; i < 100; i++)
    {
        if(Mediocre->placeShips(board))
            count++;
        board.clear();
    }
    cout << "Placed " << count << " times" << endl;
    board.display(false);
    cerr << "This game is cool bro."<< endl;
}
*/
/*
int main()
{
    /*Game g(3, 3);
    g.addShip(2, 'R', "rowboat");
    Player* p1 = createPlayer("mediocre", "Popeye", g);
    
    int k = 0;
    
    Board b(g);
    for(int i = 0; i < 100; i++)
    {
        if(p1->placeShips(b))
            k++;
    }
    
    cout << k << " times placed" << endl;*/
    
 /*
    Game game(10,10);
    
    game.addShip(5, 'A', "aircraft carrier");
    game.addShip(4, 'B', "battleship");
    game.addShip(3, 'D', "destroyer");
    game.addShip(3, 'S', "submarine");
    game.addShip(2, 'P', "patrol boat");
    
    Board board(game);
    Player* Mediocre = createPlayer("good", "TEST", game);
    int count = 0;
    
    for(int i = 0; i < 100; i++)
    {
        if(Mediocre->placeShips(board))
            count++;
        board.clear();
    }
    cout << "Placed " << count << " times" << endl;
    
    Point k = Mediocre->recommendAttack();
    // board.display(false);
}
*/


