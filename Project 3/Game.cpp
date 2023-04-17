#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    ~GameImpl(); // needed?
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
private:
    struct Ship
    {
        int m_length;
        char m_symbol;
        string m_name;
    };
    int m_rows;
    int m_columns;
    vector<Ship> m_ships;
    int mn_ships;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    m_rows = nRows;
    m_columns = nCols;
    mn_ships = 0;
}

GameImpl::~GameImpl()
{
    
}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_columns;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if (length > m_rows || length > m_columns || symbol == 'X' || symbol == 'o' || symbol == '.' || isprint(symbol) == 0)
        return false;
    
    Ship newShip;
    newShip.m_length = length;
    newShip.m_name = name;
    newShip.m_symbol = symbol;
    m_ships.push_back(newShip);
    mn_ships++;
    
    return true;
}

int GameImpl::nShips() const
{
    return mn_ships;
}

int GameImpl::shipLength(int shipId) const
{
    return m_ships[shipId].m_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return m_ships[shipId].m_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return m_ships[shipId].m_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{
    if(!p1->placeShips(b1) || !p2->placeShips(b2))
    {
        return nullptr;
    }
    
    if(p1->isHuman() || p2->isHuman())
        waitForEnter();
    
    Point p;
    bool shotHit;
    bool shipDestroyed;
    bool validShot;
    int shipId;
    
    while(!b1.allShipsDestroyed() || !b2.allShipsDestroyed())
    {
        cout << p1->name() << "'s turn." << " Board for " << p2->name() << ": " << endl;
        if(p1->isHuman())
            b2.display(true);
        else
            b2.display(false);
        
        cout << "BEFORE P1's recommend attack" << endl;

        p = p1->recommendAttack();
        
        cout << "AFTER P1's recommend attack" << endl;

        if(b2.attack(p, shotHit, shipDestroyed, shipId) && isValid(p))
        {
            cout << "HERE" << endl;
            validShot = true;
        }
        else
        {
            cout << "NOT HERE" << endl;
            validShot = false;
        }
        p1->recordAttackResult(p, validShot, shotHit, shipDestroyed, shipId);
        
        if(shotHit)
            cout << p1->name() << " attacked (" << p.r << "," << p.c << ")" << " and hit something, resulting in:" << endl;
        else if(!shotHit && !validShot)
            cout << p1->name() << " wasted a shot at (" << p.r << "," << p.c << ")."<< endl;
        else if(!shotHit && validShot)
            cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and missed," << " resulting in:" << endl;
    
        if(p1->isHuman() && validShot)
            b2.display(true);
        else if (p1->isHuman() && !validShot)
            ;
        else
            b2.display(false);
        
        if(p1->isHuman() || p2->isHuman())
        {
            waitForEnter();
        }
        
        if(shipDestroyed)
            cout << p2->name() << "'s " << p2->game().shipName(shipId) << " was destroyed!" << endl;
        
        if(b1.allShipsDestroyed() && p1->isHuman())
        {
            cout << p2->name() << "'s board was:" << endl;
            b2.display(false);
        }
        if(b2.allShipsDestroyed() && p2->isHuman())
        {
            cout << p1->name() << "'s board was:" << endl;
            b1.display(false);
        }
        
        if(b1.allShipsDestroyed())
        {
            cout << p2->name() << " wins!" << endl;
            return p2;
        }
        else if(b2.allShipsDestroyed())
        {
            cout << p1->name() << " wins!" << endl;
            return p1;
        }
        
        cout << p2->name() << "'s turn." << " Board for " << p1->name() << ": " << endl;
        if(p2->isHuman())
            b1.display(true);
        else
            b1.display(false);
        
        cout << "BEFORE P2's recommend attack" << endl;
        
        p = p2->recommendAttack();
        
        cout << "AFTER P2's recommend attack" << endl;

        if(b1.attack(p, shotHit, shipDestroyed, shipId))
            validShot = true;
        else
            validShot = false;
        p2->recordAttackResult(p, validShot, shotHit, shipDestroyed, shipId);
        
        if(shotHit)
            cout << p2->name() << " attacked (" << p.r << "," << p.c << ")" << " and hit something, resulting in:" << endl;
        else if(!shotHit && !validShot)
            cout << p2->name() << " wasted a shot at (" << p.r << "," << p.c << ")."<< endl;
        else if(!shotHit && validShot)
            cout << p2->name() << " attacked (" << p.r << "," << p.c << ") and missed," << " resulting in:" << endl;

        if(p2->isHuman() && validShot)
            b1.display(true);
        else if (p2->isHuman() && !validShot)
            ;
        else
            b1.display(false);
        
        if(p1->isHuman() || p2->isHuman())
        {
            waitForEnter();
        }
        
        if(shipDestroyed)
            cout << p1->name() << "'s " << p2->game().shipName(shipId) << " was destroyed!" << endl;
        
        if(b1.allShipsDestroyed() && p1->isHuman())
        {
            cout << p2->name() << "'s board was:" << endl;
            b2.display(false);
        }
        if(b2.allShipsDestroyed() && p2->isHuman())
        {
            cout << p1->name() << "'s board was:" << endl;
            b1.display(false);
        }
        if(b1.allShipsDestroyed())
        {
            cout << p2->name() << " wins!" << endl;
            return p2;
        }
        else if(b2.allShipsDestroyed())
        {
            cout << p1->name() << " wins!" << endl;
            return p1;
        }
    }

    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

