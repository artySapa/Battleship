#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
      // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
      // AwfulPlayer completely ignores what the opponent does
}






//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
  public:
    HumanPlayer(string name, const Game& g);
    //~HumanPlayer();
    
    virtual bool isHuman() const;
    
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    
};

HumanPlayer::HumanPlayer(string name, const Game& g): Player(name, g)
{

}

bool HumanPlayer::isHuman() const
{
    return true;
}

bool HumanPlayer::placeShips(Board& b)
{
    b.display(false);
    
    int r = -1;
    int c = -1;
    
    Direction dir;
    string dirs;
    
    for (int k = 0; k < game().nShips(); k++)
    {
        cout << "Enter h or v for direction of " << game().shipName(k) << " (length " << game().shipLength(k) << "):" << endl;
        cin >> dirs;
        
        if(dirs[0] == 'h')
            dir = HORIZONTAL;
        else if (dirs[0] == 'v')
            dir = VERTICAL;
        else
        {
            cout << "Direction must be h or v." << endl;
            placeShips(b);
            break;
        }
        
        cout << "Enter row and column of left most cell (e.g., 1 7): " << endl;
        getLineWithTwoIntegers(r, c);
        
        while(!b.placeShip(Point(r,c), k, dir))
        {
            cout << "Ship cannot be placed there." << endl;
            cout << "Enter row and column of left most cell (e.g., 1 7): " << endl;
            getLineWithTwoIntegers(r, c);
        }
        
        b.display(false);
        
    }
    
    return true;
}

Point HumanPlayer::recommendAttack()
{
    Point p;
    int r = -1;
    int c = -1;
    
    cout << "Enter the row and column to attack (e.g., 1 7): ";
    
    getLineWithTwoIntegers(r, c);
    
    p.r = r;
    p.c = c;
    
    return p;
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                            bool shipDestroyed, int shipId)
{
    
}

void HumanPlayer::recordAttackByOpponent(Point p)
{
    
}


//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer: public Player
{
public:
    MediocrePlayer(string name, const Game& g);
    // ~MediocrePlayer();
    
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    bool recursivePlace(Board& b, int placedShips);
    char attacks[MAXROWS][MAXCOLS];
    int m_state = 1;
    Point lastHit;
    int placedShips = 0;
    vector<Point> vp; 
};


MediocrePlayer::MediocrePlayer(string name, const Game& g): Player(name, g)
{
    
}

bool MediocrePlayer::placeShips(Board& b)
{
    
    int tries = 0;
    bool result = false;

    while(tries < 50)
    {
        b.block();
        result = recursivePlace(b, 0);
        if(!result)
        {
            b.unblock();
            tries++;
        }
        else
        {
            b.unblock();
            return result;
        }
        
    }
    
    b.unblock();

    return result;
    
}

Point MediocrePlayer::recommendAttack()
{

    if (vp.empty())
        m_state = 1;
    
    if (m_state == 1)
    {
        //cout << "m_state is 1!!!" << endl;
        Point p(randInt(game().rows()), randInt(game().cols()));
        //cout << "after random point" << endl;

        /*while(attacks[p.r][p.c] == '#')
        {
            if(p.r < game.rows() && p.c < game.cols())
            {
                p.r++;
                p.c++;
            }
            else if(p.r >= game.rows() && p.c < game.cols())
            {
                p.c++;
            }
            else
                p.r++;
            
        }*/
        
        attacks[p.r][p.c] = '#';
        //cout << "after random point to ###" << endl;
        return p;
    }
    else if (m_state == 2)
    {
        //cout << "m_state is 2!!!" << endl;
        int index = randInt(vp.size());
        Point temp = vp.at(index);
        attacks[temp.r][temp.c] = '#';
        vp.erase(vp.begin() + index);
        return temp;
        
    }
    
    return Point(-1,-1);
    
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if(!validShot)
    {
        return;
    }
    
    if(m_state == 1 && !shotHit)
    {
        m_state = 1;
    }
    else if(m_state == 1 && shotHit && shipDestroyed)
    {
        m_state = 1;
    }
    else if (m_state == 1 && shotHit && !shipDestroyed)
    {
        m_state = 2;
        lastHit.r = p.r;
        lastHit.c = p.c;
        vp.clear();
        for(int i = -4; i <= 4; i++)
        {
            if(i == 0 || !game().isValid(Point(p.r + i, p.c)) || attacks[p.r + i][p.c] == '#')
                continue;
            vp.push_back(Point(p.r + i, p.c));
        }
        for(int j = -4; j <= 4; j++)
        {
            if(j == 0 || !game().isValid(Point(p.r, p.c + j)) || attacks[p.r][p.c + j] == '#')
                continue;
            vp.push_back(Point(p.r, p.c + j));
        }
        if (vp.empty())
            m_state = 1;
    }
    else if(m_state == 2 && !shotHit)
    {
        m_state = 2;
    }
    else if(m_state == 2 && shotHit && shipDestroyed)
    {
        m_state = 1;
        vp.clear();
    }
    else if (m_state == 2 && shotHit && !shipDestroyed)
    {
        m_state = 2;
    }
    
}

void MediocrePlayer::recordAttackByOpponent(Point p)
{
    
}

bool MediocrePlayer::recursivePlace(Board& b, int placedShips)
{
    
    bool result = false;
    
    if(placedShips == game().nShips())
        return true;
    
    for(int i = 0; i < game().rows(); i++)
    {
        for(int j = 0; j < game().cols(); j++)
        {
            if(b.placeShip(Point(i, j), placedShips, HORIZONTAL))
            {
                result = recursivePlace(b, placedShips + 1);
                if(!result)
                {
                    b.unplaceShip(Point(i, j), placedShips, HORIZONTAL);
                }
                else
                    return result;
            }
            if(b.placeShip(Point(i, j), placedShips, VERTICAL))
            {
                result = recursivePlace(b, placedShips + 1);
                if(!result)
                {
                    b.unplaceShip(Point(i, j), placedShips, VERTICAL);
                }
                else
                    return result;
            }
        }
    }
    
    return false;
    
}



//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer: public Player
{
public:
    GoodPlayer(string name, const Game& g);
    // ~GoodPlayer();
    
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    bool recursivePlace(Board& b, int placedShips);
    char attacks[MAXROWS][MAXCOLS];
    char hits[MAXROWS][MAXCOLS];
    int m_state = 1;
    Point lastHit;
    int placedShips = 0;
    vector<Point> toShoot;
    vector<Point> toShootAfter;
    vector<Point> cross;
    vector<Point> line;
};

GoodPlayer::GoodPlayer(string name, const Game& g): Player(name, g)
{
    for(int i = 0; i < game().rows(); i++)
    {
        if(i % 2 == 0)
        {
            for(int j = 0; j < game().cols(); j++)
            {
                if(j % 2 == 0)
                    toShootAfter.push_back(Point(i,j));
                if(j % 2 != 0)
                    toShoot.push_back(Point(i,j));
            }
        }
        if(i % 2 != 0)
        {
            for(int j = 0; j < game().cols(); j++)
            {
                if(j % 2 != 0)
                    toShootAfter.push_back(Point(i,j));
                if(j % 2 == 0)
                    toShoot.push_back(Point(i,j));
            }
        }
    }
}

bool GoodPlayer::placeShips(Board &b)
{
    int tries = 0;
    bool result = false;

    while(tries < 50)
    {
        b.block();
        result = recursivePlace(b, 0);
        if(!result)
        {
            b.unblock();
            tries++;
        }
        else
        {
            b.unblock();
            return result;
        }
    }
    
    b.unblock();

    return result;
}

Point GoodPlayer::recommendAttack()
{
    
    if (cross.empty())
        m_state = 1;
    
    if (m_state == 1)
    {
        if(!toShoot.empty())
        {
            Point p = toShoot.at(0);
            
            while(!toShoot.empty() && toShoot.size() != 1 && attacks[p.r][p.c] == '#')
            {
                toShoot.erase(toShoot.begin());
                p = toShoot.at(0);
            }
            
            attacks[p.r][p.c] = '#';
            toShoot.erase(toShoot.begin());
            
            return p;
        }

        else if(!toShootAfter.empty())
        {
            Point p = toShootAfter.at(0);
            
            while(!toShootAfter.empty() && toShootAfter.size() != 1 && attacks[p.r][p.c] == '#')
            {
                toShootAfter.erase(toShootAfter.begin());
                p = toShootAfter.at(0);
            }
            
            attacks[p.r][p.c] = '#';
            toShootAfter.erase(toShootAfter.begin());
            
            return p;
        }
        
        else
        {
            Point p(randInt(game().rows()), randInt(game().cols()));
           /* while(attacks[p.r][p.c] == '#')
            {
                p.r = randInt(game().rows());
                p.c = randInt(game().cols());
            }*/
            attacks[p.r][p.c] = '#';
            
            return p;
        }
            
    }
    
    else if (m_state == 2)
    {
        
        int index = randInt(cross.size());
        Point temp = cross.at(index);
        attacks[temp.r][temp.c] = '#';
        cross.erase(cross.begin() + index);
        
        return temp;
        
    }
    
    else if (m_state == 3)
    {
        if(!line.empty())
        {
            Point temp = line.at(0);
            while(!line.empty() && line.size() != 1 && attacks[temp.r][temp.c] == '#')
            {
                line.erase(line.begin());
                temp = line.at(0);
            }
            
            attacks[temp.r][temp.c] = '#';
            line.erase(line.begin());
            
            return temp;
        }
        else
        {
            Point p(randInt(game().rows()), randInt(game().cols()));
            while(attacks[p.r][p.c] == '#')
            {
                p.r = randInt(game().rows());
                p.c = randInt(game().cols());
            }
            attacks[p.r][p.c] = '#';
            
            return p;
        }
    }
    
    
    return Point(-1,-1);
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    if(!validShot)
        return;
    
    if(m_state == 1 && !shotHit)
    {
        m_state = 1;
    }
    else if(m_state == 1 && shotHit && shipDestroyed)
    {
        m_state = 1;
        hits[p.r][p.c] = 'H';
    }

    else if (m_state == 1 && shotHit && !shipDestroyed)
    {
        hits[p.r][p.c] = 'H';
        m_state = 2;
        lastHit.r = p.r;
        lastHit.c = p.c;
        cross.clear();
        for(int i = -1; i <= 1; i++)
        {
            if(i == 0 || !game().isValid(Point(p.r + i, p.c)) || attacks[p.r + i][p.c] == '#' || hits[p.r + i][p.c] == 'H')
                continue;
            cross.push_back(Point(p.r + i, p.c));
        }
        for(int j = -1; j <= 1; j++)
        {
            if(j == 0 || !game().isValid(Point(p.r, p.c + j)) || attacks[p.r][p.c + j] == '#' || hits[p.r][p.c + j] == 'H')
                continue;
            cross.push_back(Point(p.r, p.c + j));
        }
        if (cross.empty())
            m_state = 1;
    }
    else if(m_state == 2 && !shotHit)
    {
        m_state = 2;
    }
    else if(m_state == 2 && shotHit && shipDestroyed)
    {
        m_state = 1;
        hits[p.r][p.c] = 'H';
        cross.clear();
    }
    else if (m_state == 2 && shotHit && !shipDestroyed)
    {
        m_state = 3;
        hits[p.r][p.c] = 'H';
        line.clear();
        if((lastHit.r > p.r || lastHit.r < p.r) && lastHit.c == p.c)
        {
            for(int i = -4; i <= 4; i++)
            {
                if(i == 0 || !game().isValid(Point(p.r + i, p.c)) || attacks[p.r + i][p.c] == '#')
                    continue;
                line.push_back(Point(p.r + i, p.c));
            }
        }
        if((lastHit.c > p.c || lastHit.c < p.c) && lastHit.r == p.r)
        {
            for(int j = -4; j <= 4; j++)
            {
                if(j == 0 || !game().isValid(Point(p.r, p.c + j)) || attacks[p.r][p.c + j] == '#')
                    continue;
                line.push_back(Point(p.r, p.c + j));
            }
        }
        
        
        
        if (line.empty())
            m_state = 1;
    }
    else if(m_state == 3 && !shotHit)
    {
        m_state = 3;
    }
    else if(m_state == 3 && shotHit && shipDestroyed)
    {
        m_state = 1;
        hits[p.r][p.c] = 'H';
        line.clear();
    }
}

void GoodPlayer::recordAttackByOpponent(Point p)
{
    
}

bool GoodPlayer::recursivePlace(Board& b, int placedShips)
{
    
    bool result = false;
    
    if(placedShips == game().nShips())
        return true;
    
    for(int i = 0; i < game().rows(); i++)
    {
        for(int j = 0; j < game().cols(); j++)
        {
            if(b.placeShip(Point(i, j), placedShips, HORIZONTAL))
            {
                result = recursivePlace(b, placedShips + 1);
                if(!result)
                {
                    b.unplaceShip(Point(i, j), placedShips, HORIZONTAL);
                }
                else
                    return result;
            }
            if(b.placeShip(Point(i, j), placedShips, VERTICAL))
            {
                result = recursivePlace(b, placedShips + 1);
                if(!result)
                {
                    b.unplaceShip(Point(i, j), placedShips, VERTICAL);
                }
                else
                    return result;
            }
        }
    }
    
    return false;
    
}

//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
