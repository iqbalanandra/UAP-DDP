#include <iostream>
#include <fstream>
#include <conio.h>
#include <dos.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define PURPLE 5
#define DARKYELLOW 6
#define WHITE 7
#define GREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define PINK 13
#define YELLOW 14
#define LIGHTWHITE 15

#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int nyawa;
int telorY[3];
int telorX[3];
bool telorFlag[3];
char keranjang[4][4] = {
    '.',
    ' ',
    ' ',
    '.',
    '|',
    ' ',
    ' ',
    '|',
    '\\',
    ' ',
    ' ',
    '/',
    ' ',
    '.',
    '.',
    ' ',
};

// posisi awal keranjang di tengah secara horizontal layar
int keranjangPos = WIN_WIDTH / 2;// agar ketengah
// score awal
int score = 0;

void textcolor(int color)
{
    SetConsoleTextAttribute(console, color);
}

void gotoxy(int x, int y)
{
    /*
		pindahkan kursor ke koordinat x horizontal atau kolom
		dan koordinat y vertikal atau baris
	*/
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) //
{
    if (size == 0)
        size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder()
{
    textcolor(YELLOW);
    // bikin border kiri
    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            gotoxy(0 + j, i);
            cout << "±";
            gotoxy(WIN_WIDTH - j, i);
            cout << "±";
        }
    }
    // lebar arena 36
    // bikin border kanan
    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        gotoxy(SCREEN_WIDTH, i);
        cout << "±";
    }
    textcolor(WHITE);
}

void genTelor(int ind)
{
    /*
		bikin posisi Telor di koordinat horizontal x
		minimal di x = 17, karena lebar border kiri adalah 17
		maksimal 49, karena 49 adalah posisi bagian kiri telor
	*/
    telorX[ind] = 17 + rand() % (33); //Random spawn telor di x
}

void drawTelor(int ind)
{
    /*
		gambar Telor pada horizontal telorX[ind]
		dan vertikal telorY[ind]
	*/
    if (telorFlag[ind] == true)// cek apakah telor ada
    {
        textcolor(WHITE);
        gotoxy(telorX[ind], telorY[ind]);
        cout << " .. ";
        gotoxy(telorX[ind], telorY[ind] + 1);
        cout << "....";
        gotoxy(telorX[ind], telorY[ind] + 2);
        cout << "....";
        gotoxy(telorX[ind], telorY[ind] + 3);
        cout << " .. ";
        textcolor(WHITE);
    }
}

void eraseTelor(int ind)
{
    if (telorFlag[ind] == true)
    {
        gotoxy(telorX[ind], telorY[ind]);
        cout << "    ";
        gotoxy(telorX[ind], telorY[ind] + 1);
        cout << "    ";
        gotoxy(telorX[ind], telorY[ind] + 2);
        cout << "    ";
        gotoxy(telorX[ind], telorY[ind] + 3);
        cout << "    ";
    }
}

void resetTelor(int ind)
{
    eraseTelor(ind);
    telorY[ind] = 1;
    genTelor(ind);
}

void drawKeranjang()
{
    textcolor(GREEN);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // posisi keranjang selalu pada baris 22
            gotoxy(j + keranjangPos, i + 22);
            cout << keranjang[i][j];
        }
    }
    textcolor(WHITE);
}

void eraseKeranjang()
{
    /*
		hapus bekas pergeseran keranjang
	*/
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(j + keranjangPos, i + 22);
            cout << " ";
        }
    }
}

bool collision(int ind)
{
    /*
		kalo terjadi tabrakan return true, else false
	*/

    // klo posisi Telor ke-0, berada pada baris tempat keranjang
    // berada, terdapat kemungkinan tabrakan terjadi
    if (telorY[ind] + 4 >= 23)
    {
        if (telorX[ind] + 4 - keranjangPos >= 0 && telorX[ind] + 4 - keranjangPos < 9)
        {
            return true;
        }
    }
    return false;
}

void gameover()
{
    string text;

    ofstream HallOfFameFile("game.dat", ios_base::app);

    system("cls");
    textcolor(CYAN);
    cout << endl;
    cout << "\t\t--------------------------" << endl;
    cout << "\t\t-------- Game Over -------" << endl;
    cout << "\t\t--------------------------" << endl
         << endl;
    cout << "\t\tYour name: ";
    cin >> text;
    HallOfFameFile << text << " " << score << endl;
    cout << "\t\tPress any key to go back to menu.";
    textcolor(WHITE);
    getch();

    HallOfFameFile.close();
}

void updateScore()
{
    textcolor(CYAN);
    gotoxy(WIN_WIDTH + 7, 5);
    cout << "Telor: " << score << endl;
    gotoxy(WIN_WIDTH + 7, 6);
    cout << "Nyawa: " << nyawa << endl;
    textcolor(WHITE);
}

void hall_of_fame(void)
{
    string text;

    ifstream HallOfFameFile("game.dat");

    system("cls");
    textcolor(DARKYELLOW);
    cout << "Hall of Fame";
    cout << "\n----------------";
    while (getline(HallOfFameFile, text))
    {
        cout << "\n " << text;
    }
    cout << "\n\nPress any key to go back to menu";
    textcolor(WHITE);
    getch();

    HallOfFameFile.close();
}

void instructions()
{
    system("cls");
    textcolor(DARKYELLOW);
    cout << "Instructions";
    cout << "\n----------------";
    cout << "\n Avoid Cars by moving left or right. ";
    cout << "\n\n Press 'a' to move left";
    cout << "\n Press 'd' to move right";
    cout << "\n Press 'escape' to exit";
    cout << "\n\nPress any key to go back to menu";
    textcolor(WHITE);
    getch();
}

void play()
{
    nyawa = 3;
    keranjangPos = -1 + WIN_WIDTH / 2;
    score = 0;
    telorFlag[0] = true;
    telorFlag[1] = false;
    telorY[0] = telorY[1] = 1;

    system("cls");
    drawBorder();
    updateScore();
    // bikin telor ke 0
    genTelor(0);
    // bikin telor 1
    genTelor(1);

    textcolor(LIGHTCYAN);
    gotoxy(WIN_WIDTH + 6, 2);
    cout << "Kumpul Telor";
    gotoxy(WIN_WIDTH + 6, 4);
    cout << "----------";
    gotoxy(WIN_WIDTH + 6, 7);
    cout << "----------";
    gotoxy(WIN_WIDTH + 7, 12);
    cout << "Kontrol ";
    gotoxy(WIN_WIDTH + 6, 13);
    cout << "--------- ";
    gotoxy(WIN_WIDTH + 3, 14);
    cout << " Tombol A - Kiri";
    gotoxy(WIN_WIDTH + 3, 15);
    cout << " Tombol D - Kanan";

    gotoxy(18, 5);
    cout << "Press any key to start";
    textcolor(WHITE);
    getch();
    gotoxy(18, 5);
    cout << "                      ";

    while (true) // KONTROL KEYBOARD SEKALIGUS MENGANIMASIKAN TELOR JATUH
    {
        if (kbhit())
        {
            char ch = getch();
            if (ch == 'a' || ch == 'A' || ch == KEY_LEFT)
            {
                // 18 adalah batas border kiri
                if (keranjangPos > 18)
                    keranjangPos -= 4;
            }
            else if (ch == 'd' || ch == 'D' || ch == KEY_RIGHT)
            {
                // 54 adalah batas border kanan
                // 50 karena lebar keranjang 4 jadi dikurang 4
                if (keranjangPos < 50)
                    keranjangPos += 4;
            }
            // kalo pencet ESC, kembali ke menu utama
            else if (ch == 27)
            {
                break;
            }
        }

        // gambar keranjang
        drawKeranjang();

        // gambar Telor ke-i klo ada
        for (int i = 0; i < 2; i++)
        {
            drawTelor(i);
        }

        // cek jika tabrakan keranjang dengan Telor, maka kalah
        for (int i = 0; i < 2; i++)
        {
            if (collision(i) == true)
            {
                //gameover();
                resetTelor(i);
                score++;
                updateScore();
            }
        }

        Sleep(75);

        // hapus gambar telor
        eraseKeranjang();

        // hapus gambar Telor
        for (int i = 0; i < 2; i++)
        {
            eraseTelor(i);
        }

        // jika posisi telor ke-0 berada pada Y = 10 maka telor ke-1 akan spawn
        if (telorY[0] == 10)
            // jika telor ke-1 tidak ada, maka buat jadi ada(spawn telor ke-1)
            if (telorFlag[1] == false)
                telorFlag[1] = true;

        // jalankan telor ke-i, 1 vertikal ke bawah
        for (int i = 0; i < 2; i++)
        {
            if (telorFlag[i] == true)
                telorY[i] += 1;
        }

        // telor ke-i sampe batas bawah screen maka nyawa berkurang 1
        // dan telor ke-i di hapus, lalu buat baru
        for (int i = 0; i < 2; i++)
        {
            if (telorY[i] > SCREEN_HEIGHT - 4)
            {
                resetTelor(i);
                nyawa--;
                updateScore();
            }

            if (!nyawa)
            {
                gameover();
                return;
            }
        }
    }
}

int main()
{
    setcursor(0, 0);
    srand((unsigned)time(NULL));

    do
    {
        system("cls");
        textcolor(GREEN);
        gotoxy(10, 5);
        cout << " -------------------------- ";
        gotoxy(10, 6);
        cout << " |       Telor Game       | ";
        gotoxy(10, 7);
        cout << " --------------------------";
        gotoxy(10, 9);
        cout << "1. Start Game";
        gotoxy(10, 10);
        cout << "2. Instructions";
        gotoxy(10, 11);
        cout << "3. Hall of Fame";
        gotoxy(10, 12);
        cout << "4. Quit";
        gotoxy(10, 14);
        cout << "Select option: ";
        textcolor(WHITE);
        char op = getch();

        if (op == '1')
            play();
        else if (op == '2')
            instructions();
        else if (op == '3')
            hall_of_fame();
        else if (op == '4')
            exit(0);

    } while (true);

    return 0;
}
