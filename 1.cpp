#include <iostream>
#include <unistd.h>
using namespace std;
int main()
{
    uid_t uid = getuid();
    uid_t euid = geteuid();
    cout << "userid is " << uid << "effective userid is: " << euid << endl;
    return 0;
}

