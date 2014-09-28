#include <fstream>
#include "RDP.h"

#define FNAMESIZE 12

int main(int argc, char *argv[])
{
    char filename[FNAMESIZE];

    if (argc < 2)
    {
        cout << "\nEnter filename : ";
        cin >> filename;
    }
    else if (argc == 2)
        strcpy(filename, argv[1]);
    else
    {
        cout << "Argument error!" << endl;
        exit(1);
    }

    ifstream fp;
    fp.open(filename, ios::in);
    if (fp.fail())
    {
        cout << "File open error!" << endl;
        exit(2);
    }

    RDP rdp(fp);
    rdp.parse();

    return 0;
}