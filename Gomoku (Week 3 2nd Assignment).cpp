#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

#define capacity 100
#define MaxP 10

struct board
{
    char G[capacity][capacity];
    int dim;
    char Pname[MaxP][30];
    char Psymb[MaxP];
    int turn, ri, ci, WinCount;
    bool GameOver = false;
    char WinnerName[30];
    char WinnerSymb;
    int PlayerWon[MaxP] = { 0 };
    int DrawTimes = 0;
    int MatchesPlayed = 0;
    int draw = -1;
    int NOP, HP;
    int WinOrder[MaxP];
    int WC = 0;
    bool IsCpu[MaxP] = {false};
    char CPUName[MaxP][20] = { "Bashir[CPU]","Sameer[CPU]","Lateef[CPU]",
        "Mareez[CPU]","Rafeeq[CPU]","Kamzoor[CPU]","Bilawal[CPU]",
        "Marium[CPU]","Reham[CPU]","Kanjoos[CPU]" };
    char CPUsym[MaxP]={'*','+','^','%','#','$','@','!','&','?'};
};

void ResetBoard(board &b)
{
    for (int ri = 0; ri < b.dim; ri++)
    {
        for (int ci = 0; ci < b.dim; ci++)
        {
            b.G[ri][ci] = '-';
        }
    }
    for (int i = 0; i < b.NOP; i++)
    {
        b.WinOrder[i] = -1;
    }
    b.GameOver = false;
    b.draw = -1;
    b.WC = 0;
}

void init(board &b)
{
    cout << "Enter Dimensions: ";
    cin >> b.dim;
    cout << "Enter Win Count: ";
    cin >> b.WinCount;
    cout << "Enter Number of Total Players: ";
    cin >> b.NOP;
    do
    {
        cout << "Enter Number of Human Players: ";
        cin >> b.HP;
        if (b.HP > b.NOP)
        {
            cout << "Human Players Should Be Less than Total Player!" << endl;
        }
    } while (b.HP>b.NOP);
    for (int i = 0; i < b.NOP-b.HP; i++)
    {
        strcpy_s(b.Pname[b.HP + i], b.CPUName[i]);
        b.Psymb[b.HP + i] = b.CPUsym[i];
        b.IsCpu[b.HP+i] = true;
    }
    for (int i = 0; i < b.HP; i++)
    {
        cin.ignore();
        cout << "Enter Player" << i + 1 << "'s Name: ";
        cin.getline(b.Pname[i],30);
        cout << "Enter Player" << i + 1 << "'s Symbol: ";
        cin>>b.Psymb[i];
    }
    ResetBoard(b);

    b.turn = rand() % b.NOP;
}

void DisplayBoard(char G[capacity][capacity],int dim)
{
    system("cls");
    cout << setw(5) << "";
    for (int i = 1; i <= dim; i++)
    {
        cout << setw(5) << left << i;
    }
    cout << endl;
    for (int ri = 0; ri < dim; ri++)
    {
        cout << setw(5) << left << ri + 1;
        for (int ci = 0; ci < dim; ci++)
        {
            cout << setw(5) << left << G[ri][ci];
        }
        cout << endl;
    }
}

void PlayerMessage(board b)
{
    cout << b.Pname[b.turn] << "'s Turn (" << b.Psymb[b.turn] << "): " << endl;
    cout << "[ri ci]: ";
}

void PlayerTurn(int &ri,int &ci)
{
    cin >> ri >> ci;
    ri--; ci--;
}

bool IsNotValidTurn(board b)
{
    return (b.ri < b.dim && b.ci < b.dim && b.G[b.ri][b.ci] == '-');
}

void UpdateBoard(board &b)
{
    b.G[b.ri][b.ci] = b.Psymb[b.turn];
}

bool IsHorizontalWin(board b,int ri,int ci)
{
    if ((ci + b.WinCount) > b.dim)
    {
        return false;
    }
    for (int i = 0; i < b.WinCount; i++)
    {
        if (b.G[ri][ci+i]!=b.Psymb[b.turn])
        {
            return false;
        }
    }
    return true;
}

bool IsVerticalWin(board b, int ri, int ci)
{
    if ((ri + b.WinCount) > b.dim)
    {
        return false;
    }
    for (int i = 0; i < b.WinCount; i++)
    {
        if (b.G[ri + i][ci] != b.Psymb[b.turn])
        {
            return false;
        }
    }
    return true;
}

bool IsRDiagonalWin(board b, int ri, int ci)
{
    if ((ri + b.WinCount) > b.dim && (ci + b.WinCount) > b.dim)
    {
        return false;
    }
    for (int i = 0; i < b.WinCount; i++)
    {
        if (b.G[ri + i][ci + i] != b.Psymb[b.turn])
        {
            return false;
        }
    }
    return true;
}

bool IsLDiagonalWin(board b, int ri, int ci)
{
    if ((ri + b.WinCount) > b.dim && (ci - b.WinCount - 1) < 0)
    {
        return false;
    }
    for (int i = 0; i < b.WinCount; i++)
    {
        if (b.G[ri + i][ci - i] != b.Psymb[b.turn])
        {
            return false;
        }
    }
    return true;
}

bool IsWin(board b)
{
    for (int i = 0; i < b.dim; i++)
    {
        for (int j = 0; j < b.dim; j++)
        {
            if (IsHorizontalWin(b, i, j) || IsVerticalWin(b, i, j) || IsRDiagonalWin(b, i, j) || IsLDiagonalWin(b, i, j))
            {
                return true;
            }
        }
    }
    return false;
}

bool IsDraw(board b)
{
    for (int i = 0; i < b.dim; i++)
    {
        for (int j = 0; j < b.dim; j++)
        {
            if (b.G[i][j] == '-')
                return false;
        }
    }
    return true;
}

void DisplayScoreBoard(board b)
{
    cout << "==============================================" << endl;
    cout << "Positions For This Match:" << endl;
    for (int i = 0; i < b.WC; i++)
    {
        cout << i + 1 << ". " << b.Pname[b.WinOrder[i]] << " (" << b.Psymb[b.WinOrder[i]] << ") Won " << endl;
    }
    cout << endl << b.Pname[b.WinOrder[b.WC]] << " is a LOSER! *Everybody Laughs at Him*";
    cout << "\n----------------------------------------------" << endl;
    cout << "Top Scorers:" << endl;
    for (int i = 0; i < b.NOP; i++)
    {
        cout << b.Pname[i] << " Came First " << b.PlayerWon[i] << " Times." << endl;
    }
    cout << "\n----------------------------------------------" << endl;
    cout << "Games Played: " << b.MatchesPlayed << endl;
    cout << "Draw Times: " << b.DrawTimes << endl;
    cout << "\n----------------------------------------------" << endl << endl;
}

void TurnChange(board& b)
{
    b.turn = (b.turn + 1) % b.NOP;
    for (int i = 0; i < b.WC; i++)
    {
        if (b.WinOrder[i]==b.turn)
        {
            b.turn = (b.turn + 1) % b.NOP;
        }
    }
}

void ComputerMove(board& b)
{
    int TempWinCount = b.WinCount;
    int TempTurn = b.turn;
    for (; b.WinCount >=1; b.WinCount--)
    {
        for (int ri = 0; ri < b.dim; ri++)
        {
            for (int ci = 0; ci < b.dim; ci++)
            {
                b.ri = ri; b.ci = ci;
                if (IsNotValidTurn(b))
                {
                    b.G[b.ri][b.ci] = b.Psymb[b.turn];
                    if (IsWin(b))
                    {
                        b.G[b.ri][b.ci] = '-';
                        b.WinCount = TempWinCount;
                        return;
                    }
                    b.G[b.ri][b.ci] = '-';
                }
            }
        }
        for (int i = 0; i < b.NOP; i++)
        {
            for (int ri = 0; ri < b.dim; ri++)
            {
                for (int ci = 0; ci < b.dim; ci++)
                {
                    b.ri = ri; b.ci = ci;
                    if (IsNotValidTurn(b))
                    {
                        b.turn = i;
                        b.G[b.ri][b.ci] = b.Psymb[b.turn];
                        if (IsWin(b))
                        {
                            b.G[b.ri][b.ci] = '-';
                            b.WinCount = TempWinCount;
                            b.turn = TempTurn;
                            return;
                        }
                        b.G[b.ri][b.ci] = '-';
                    }
                }
            }
        }
    }
    do
    {
        b.ri = rand() % b.dim;
        b.ci = rand() % b.dim;
    } while (IsNotValidTurn(b));
    b.WinCount = TempWinCount;
    b.turn = TempTurn;
}

int main()
{
    board b;
    init(b);
    char option;
    do
    {
        do
        {
            DisplayBoard(b.G, b.dim);
            PlayerMessage(b);
            if (b.IsCpu[b.turn])
            {
                ComputerMove(b);
            }
            else
            {
                do
                {
                    PlayerTurn(b.ri, b.ci);
                    if (!IsNotValidTurn(b))
                    {
                        cout << "Invalid Turn!" << endl;
                        PlayerMessage(b);
                    }
                } while (!IsNotValidTurn(b));
            }
            UpdateBoard(b);
            if (IsWin(b))
            {
                b.WinOrder[b.WC] = b.turn;
                b.WC++;
                if (b.WC == b.NOP - 1)
                {
                    b.GameOver = true;
                    TurnChange(b);
                    b.WinOrder[b.WC] = b.turn;
                }
            }
            if (b.GameOver)
            {
                strcpy_s(b.WinnerName, b.Pname[b.WinOrder[0]]);
                b.WinnerSymb = b.Psymb[b.WinOrder[0]];
                b.PlayerWon[b.WinOrder[0]]++;
                DisplayBoard(b.G, b.dim);
            }
            if (IsDraw(b))
            {
                b.draw = 1;
                DisplayBoard(b.G, b.dim);
                b.DrawTimes++;
                b.GameOver = true;
            }
            if (!b.GameOver)
            {
                TurnChange(b);
            }
        } while (!b.GameOver);
        b.MatchesPlayed++;
        if (b.draw == 1)
        {
            cout << "Game is Draw..." << endl;
        }
        else
        {
            cout << "Game Over....\n"<<b.WinnerName <<"("<<b.WinnerSymb<<") Secured First Position."<< endl;
        }
        DisplayScoreBoard(b);
        cout << "Do you want to play again? (Y/N)... ";
        cin >> option;
        if (option == 'Y' || option == 'y')
            ResetBoard(b);
    } while (option == 'Y' || option == 'y');
    return 0;
}
