#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
    const Game& m_game;
    int m_rows;
    int m_cols;
    char m_board[MAXROWS][MAXCOLS];
    int m_nShips;
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    m_rows = g.rows();
    m_cols = g.cols();
    m_nShips = g.nShips();
    
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
                m_board[r][c] = '.';
        }
    }
}

void BoardImpl::clear()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            m_board[r][c] = '.';
        }
    }
}

void BoardImpl::block()
{
    int nCells = (m_rows * m_cols) / 2;
    int blockedCells = 0;
      // Block cells with 50% probability
    while(blockedCells != nCells)
    {
        int randRow = randInt(m_game.rows());
        int randCol = randInt(m_game.cols());
        if(m_board[randRow][randCol] != '#')
        {
            m_board[randRow][randCol] = '#';
            blockedCells++;
        }
    }
}

void BoardImpl::unblock()
{
    for (int r = 0; r < m_game.rows(); r++)
    {
        for (int c = 0; c < m_game.cols(); c++)
        {
            if(m_board[r][c] == '#')
                m_board[r][c] = '.';
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    char symbol = m_game.shipSymbol(shipId);
    const int length = m_game.shipLength(shipId);
    
    if(!m_game.isValid(topOrLeft) || shipId < 0 || shipId > m_game.nShips())
        return false;
    
    for (int r = 0; r < m_game.rows(); r++)
        for (int c = 0; c < m_game.cols(); c++)
        {
            if(symbol == m_board[r][c])
                return false;
        }
    
    int row = topOrLeft.r;
    int col = topOrLeft.c;
    
    if (dir == VERTICAL && row + length - 1 < m_game.rows())
    {
        for(int i = 0; i < length; i++)
        {
            if (m_board[row + i][col] != '.')
                return false;
        }
        
        for (int r = 0; r < length; r++)
        {
            m_board[row + r][col] = symbol;
        }
        return true;
    }
    else if (dir == HORIZONTAL && col + length - 1 < m_game.rows())
    {
        for(int i = 0; i < length; i++)
        {
            if (m_board[row][col + i] != '.')
                return false;
        }
        
        for (int c = 0; c < length; c++)
        {
            m_board[row][col + c] = symbol;
        }
        return true;
    }
    else
    {
        return false;
    }
    
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    if(!m_game.isValid(topOrLeft) || shipId < 0 || shipId > m_game.nShips())
        return false;
    
    int length = m_game.shipLength(shipId);
    char symbol = m_game.shipSymbol(shipId);
    
    int row = topOrLeft.r;
    int col = topOrLeft.c;
    
    if (dir == VERTICAL)
    {
        if(m_board[row + length - 1][col] != symbol)
            return false;
        
        for(int i = row; i < row + length; i++)
        {
            if (m_board[i][col] == symbol)
                m_board[i][col] = '.';
        }
        
        return true;
    }
    else if (dir == HORIZONTAL)
    {
        if(m_board[row][col + length - 1] != symbol)
            return false;
        
        for(int i = col; i < col + length; i++)
        {
            if (m_board[row][i] == symbol)
                m_board[row][i] = '.';
        }
        
        return true;
    }
    
    else
    {
        return false;
    }
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for(int c = 0; c < m_cols; c ++)
        cout << c;
    
    cout << endl;
    
    if(!shotsOnly)
    {
        for(int r = 0; r < m_rows; r++)
        {
            cout << r << " ";
            for(int c = 0; c < m_cols; c++)
            {
                cout << m_board[r][c];
            }
            cout << endl;
        }
    }
    else
    {
        for(int r = 0; r < m_rows; r++)
        {
            cout << r << " ";
            for(int c = 0; c < m_cols; c++)
            {
                if (m_board[r][c] == 'X' || m_board[r][c] == 'o' || m_board[r][c] == '.')
                    cout << m_board[r][c];
                else
                    cout << '.';
            }
            cout << endl;
        }
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    int row = p.r;
    int col = p.c;
    int temp = -1;
    char symbol = m_board[row][col];
    
    if(!m_game.isValid(p) || symbol == 'X' || symbol == 'o')
        return false;
    
    if (symbol != '.' && symbol != 'o' && symbol != 'X')
    {
        m_board[row][col] = 'X';
        shotHit = true;
        
        for(int i = 0; i < m_game.nShips(); i++)
        {
            if (symbol == m_game.shipSymbol(i))
            {
                temp = i;
                break;
            }
        }
        
        for (int r = 0; r < m_game.rows(); r++)
            for (int c = 0; c < m_game.cols(); c++)
            {
                if (m_board[r][c] == symbol)
                {
                    shipDestroyed = false;
                    return true;
                }
                else
                {
                    shipId = temp;
                    shipDestroyed = true;
                }
            }
    
    }
    
    else if(symbol == '.')
    {
        m_board[row][col] = 'o';
        shotHit = false;
        shipDestroyed = false;
        return true;
    }
    
    if(shipDestroyed == true)
    {
        m_nShips--;
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    return (m_nShips == 0);
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
