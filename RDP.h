#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class RDP
{
    ifstream& fp;

    enum tsymbol {
        tdot, tbegin, tsemicolon, tend, tlabel, tinteger, tcomma, tcolon,
        tassign, tgoto, tif, tthen, telse, tfi, toutput,
        tlparenthsis, trparenthsis, tless, tgreat, tequal,
        tplus, tminus, tmul, tdiv, tinput, teof, tid, tnumber
    } nextSymbol;

    void error(int errnum);

    inline bool whitespace(char c) {
        return c == ' ' || c == '\t' || c == '\n';
    }
    inline void apply(int n) {
        printf(" * rule number = %3d\n", n);
    }
    inline bool letterchar(char c) {
        return c >= 'a' && c <= 'z';
    }
    inline bool numberchar(char c) {
        return c >= '0' && c <= '9';
    }

public:
    RDP(ifstream& fp);
    ~RDP();

    void parse();

    void getNextSymbol();

    void pSPL_PROGRAM();
    void pBLOCK();
    void pDCL();
    void pST_LIST();
    void pST();
    void pSTATEMENT();
    void pASSIGNMENT();
    void pGOTO_ST();
    void pIF_ST();
    void pWRITE_ST();
    void pCONDITION();
    void pEXP();
    void pTERM();
    void pFACTOR();
};

