#include <iostream>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#include "sqlite3.h"


// Turn stdin echo on or off
void setEcho(bool enable);


int main()
{
    // Use std here
    using namespace std;

    cout << "Welcome to Cipher Kick!\n\nPlease enter key string: ";

    // Turn off echo to read password
    setEcho(false);

    // Read password
    string keystr;
    getline(cin, keystr);

    // Turn echo back on
    setEcho(true);

    string pragma_str = "PRAGMA key = '" + keystr + "'";

    cout << pragma_str << endl;

    // Return success
    return 0;
}


// Turn stdin echo on or off
void setEcho(bool enable = true)
{
    // This section for windows
    #ifdef WIN32

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);

    if (!enable) mode &= ~ENABLE_ECHO_INPUT;
    else mode |= ENABLE_ECHO_INPUT;

    SetConsoleMode(hStdin, mode);

    // This section for unix / linux
    #else

    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);

    if (!enable) tty.c_lflag &= ~ECHO;
    else tty.c_lflag |= ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &tty);

    #endif
}






