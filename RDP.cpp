#include "RDP.h"


RDP::RDP(ifstream& fp)
:fp(fp)
{
}


RDP::~RDP()
{
}


void RDP::parse()
{
    cout << endl << "    *** Left Parse ***" << endl << endl;
    getNextSymbol();
    pSPL_PROGRAM();
    if (nextSymbol == teof)
        cout << endl << "    *** Accept ***" << endl;
    else
        cout << endl << "    *** Invalid source ***" << endl;
}


void RDP::error(int errnum)
{
    printf("    *** Syntax Error : ");
    switch (errnum)
    {
    case 1:
        cout << "next symbol bust be 'begin'" << endl;
        break;
    case 2:
        cout << "next symbol bust be '.'" << endl;
        break;
    case 3:
        cout << "next symbol bust be ';'" << endl;
        break;
    case 4:
        cout << "next symbol bust be <id> | <number> | '(' | 'input' | 'goto' | 'if' | 'output' | 'begin'" << endl;
        break;
    case 5:
        cout << "next symbol bust be 'end'" << endl;
        break;
    case 6:
        cout << "next symbol bust be 'label' | 'integer'" << endl;
        break;
    case 7:
        cout << "next symbol bust be <id>" << endl;
        break;
    case 8:
        cout << "next symbol bust be ':'" << endl;
        break;
    case 9:
        cout << "next symbol bust be ')'" << endl;
        break;
    case 10:
        cout << "next symbol bust be '<' | '>' | '='" << endl;
        break;
    case 11:
        cout << "next symbol bust be 'input' | <id> | <number> | '('" << endl;
        break;
    case 12:
        cout << "next symbol bust be '=>'" << endl;
        break;
    case 13:
        cout << "next symbol bust be 'goto'" << endl;
        break;
    case 14:
        cout << "next symbol bust be 'if'" << endl;
        break;
    case 15:
        cout << "next symbol bust be 'then'" << endl;
        break;
    case 16:
        cout << "next symbol bust be 'fi'" << endl;
        break;
    case 17:
        cout << "next symbol bust be 'output'" << endl;
        break;
    case 18:
        cout << "next symbol bust be '('" << endl;
        break;
    case 100:
        cout << "<id> must start letter" << endl;
    default:
        cout << "unknown error" << endl;
    }
    exit(1);
}


void RDP::getNextSymbol()
{
    if (fp.eof())
        nextSymbol = teof;

    char buf[128];

    fp >> buf;

    if (strcmp(".", buf) == 0)
        nextSymbol = tdot;
    else if (strcmp("begin", buf) == 0)
        nextSymbol = tbegin;
    else if (strcmp(";", buf) == 0)
        nextSymbol = tsemicolon;
    else if (strcmp("end", buf) == 0)
        nextSymbol = tend;
    else if (strcmp("label", buf) == 0)
        nextSymbol = tlabel;
    else if (strcmp("integer", buf) == 0)
        nextSymbol = tinteger;
    else if (strcmp(",", buf) == 0)
        nextSymbol = tcomma;
    else if (strcmp(":", buf) == 0)
        nextSymbol = tcolon;
    else if (strcmp("=>", buf) == 0)
        nextSymbol = tassign;
    else if (strcmp("goto", buf) == 0)
        nextSymbol = tgoto;
    else if (strcmp("if", buf) == 0)
        nextSymbol = tif;
    else if (strcmp("then", buf) == 0)
        nextSymbol = tthen;
    else if (strcmp("else", buf) == 0)
        nextSymbol = telse;
    else if (strcmp("fi", buf) == 0)
        nextSymbol = tfi;
    else if (strcmp("output", buf) == 0)
        nextSymbol = toutput;
    else if (strcmp("(", buf) == 0)
        nextSymbol = tlparenthsis;
    else if (strcmp(")", buf) == 0)
        nextSymbol = trparenthsis;
    else if (strcmp("<", buf) == 0)
        nextSymbol = tless;
    else if (strcmp(">", buf) == 0)
        nextSymbol = tgreat;
    else if (strcmp("=", buf) == 0)
        nextSymbol = tequal;
    else if (strcmp("+", buf) == 0)
        nextSymbol = tplus;
    else if (strcmp("-", buf) == 0)
        nextSymbol = tminus;
    else if (strcmp("*", buf) == 0)
        nextSymbol = tmul;
    else if (strcmp("/", buf) == 0)
        nextSymbol = tdiv;
    else if (strcmp("input", buf) == 0)
        nextSymbol = tinput;
    else if (strcmp("", buf) == 0)
        nextSymbol = teof;

    else if (numberchar(buf[0]))
    {
        for (unsigned int i = 1; i < strlen(buf); i++)
        {
            if (!numberchar(buf[i]))
                error(100); // no id, no number
        }
        nextSymbol = tnumber;
    }
    else
        nextSymbol = tid;
}


void RDP::pSPL_PROGRAM()
{
    if (nextSymbol == tbegin)
    {
        apply(1);
        pBLOCK();
        if (nextSymbol == tdot)
            getNextSymbol();
        else
            error(2); // tdot
    }
    else
        error(1); // tbegin
}


void RDP::pBLOCK()
{
    if (nextSymbol == tbegin)
    {
        apply(2);
        getNextSymbol();

        while (nextSymbol == tlabel || nextSymbol == tinteger)
        {
            pDCL();
            if (nextSymbol == tsemicolon)
                getNextSymbol();
            else
                error(3); // tsemicolon
        }

        if (nextSymbol == tid || nextSymbol == tinput ||
            nextSymbol == tnumber || nextSymbol == tlparenthsis ||
            nextSymbol == tgoto || nextSymbol == tif ||
            nextSymbol == toutput || nextSymbol == tbegin)
        {
            pST_LIST();
            if (nextSymbol == tend)
                getNextSymbol();
            else
                error(5); // tend
        }
        else
            error(4); // tid, tinput, tnumber, tlparenthsis, tgoto, tif, toutput, tbegin
    }
    else
        error(1); // tbegin
}


void RDP::pDCL()
{
    if (nextSymbol == tlabel || nextSymbol == tinteger)
    {
        apply(3);
        getNextSymbol();

        if (nextSymbol == tid)
            getNextSymbol();
        else
            error(7); // tid

        while (nextSymbol == tcomma)
        {
            getNextSymbol();
            if (nextSymbol == tid)
                getNextSymbol();
            else
                error(7); // tid
        }
    }
    else
        error(6); // tlabel, tinteger
}


void RDP::pST_LIST()
{
    if (nextSymbol == tid || nextSymbol == tinput ||
        nextSymbol == tnumber || nextSymbol == tlparenthsis ||
        nextSymbol == tgoto || nextSymbol == tif ||
        nextSymbol == toutput || nextSymbol == tbegin)
    {
        apply(4);
        pST();
        while (nextSymbol == tsemicolon)
        {
            getNextSymbol();
            if (nextSymbol == tid || nextSymbol == tinput ||
                nextSymbol == tnumber || nextSymbol == tlparenthsis ||
                nextSymbol == tgoto || nextSymbol == tif ||
                nextSymbol == toutput || nextSymbol == tbegin)
                pST();
            else
                error(4); // tid, tinput, tnumber, tlparenthsis, tgoto, tif, toutput, tbegin
        }
    }
    else
        error(4); // tid, tinput, tnumber, tlparenthsis, tgoto, tif, toutput, tbegin
}


void RDP::pST()
{
    // apply 전용 조건문
    if (nextSymbol == tid || nextSymbol == tinput ||
        nextSymbol == tnumber || nextSymbol == tlparenthsis ||
        nextSymbol == tgoto || nextSymbol == tif ||
        nextSymbol == toutput || nextSymbol == tbegin)
        apply(5);

    if (nextSymbol == tid)
    {
        getNextSymbol();
        if (nextSymbol == tcolon)
            getNextSymbol();
        else
            error(8); // tcolon
    }

    if (nextSymbol == tid || nextSymbol == tinput ||
        nextSymbol == tnumber || nextSymbol == tlparenthsis ||
        nextSymbol == tgoto || nextSymbol == tif ||
        nextSymbol == toutput || nextSymbol == tbegin)
        pSTATEMENT();
    else
        error(4); // tid, tinput, tnumber, tlparenthsis, tgoto, tif, toutput, tbegin
}


void RDP::pSTATEMENT()
{
    if (nextSymbol == tid || nextSymbol == tinput ||
        nextSymbol == tnumber || nextSymbol == tlparenthsis)
    {
        apply(6);
        pASSIGNMENT();
    }
    else if (nextSymbol == tgoto)
    {
        apply(6);
        pGOTO_ST();
    }
    else if (nextSymbol == tif)
    {
        apply(6);
        pIF_ST();
    }
    else if (nextSymbol == toutput)
    {
        apply(6);
        pWRITE_ST();
    }
    else if (nextSymbol == tbegin)
    {
        apply(6);
        pBLOCK();
    }
    else
        error(4); // tid, tinput, tnumber, tlparenthsis, tgoto, tif, toutput, tbegin
}


void RDP::pASSIGNMENT()
{
    if (nextSymbol == tinput || nextSymbol == tid ||
        nextSymbol == tnumber || nextSymbol == tlparenthsis)
    {
        apply(7);
        pEXP();
        if (nextSymbol == tassign)
        {
            getNextSymbol();
            if (nextSymbol == tid)
            {
                getNextSymbol();
                while (nextSymbol == tassign)
                {
                    getNextSymbol();
                    if (nextSymbol == tid)
                        getNextSymbol();
                    else
                        error(7); // tid
                }
            }
            else
                error(7); // tid
        }
        else
            error(12); // tassign
    }
    else
        error(11); // tinput, tid, tnumber, tlparenthsis
}


void RDP::pGOTO_ST()
{
    if (nextSymbol == tgoto)
    {
        apply(8);
        if (nextSymbol == tid)
            getNextSymbol();
        else
            error(7); // tid
    }
    else
        error(13); // tgoto
}


void RDP::pIF_ST()
{
    if (nextSymbol == tif)
    {
        apply(9);
        getNextSymbol();
        if (nextSymbol == tinput || nextSymbol == tid ||
            nextSymbol == tnumber || nextSymbol == tlparenthsis)
        {
            pCONDITION();
            if (nextSymbol == tthen)
            {
                getNextSymbol();
                if (nextSymbol == tid || nextSymbol == tinput ||
                    nextSymbol == tnumber || nextSymbol == tlparenthsis ||
                    nextSymbol == tgoto || nextSymbol == tif ||
                    nextSymbol == toutput || nextSymbol == tbegin)
                {
                    pST_LIST();

                    if (nextSymbol == telse)
                    {
                        getNextSymbol();

                        if (nextSymbol == tid || nextSymbol == tinput ||
                            nextSymbol == tnumber || nextSymbol == tlparenthsis ||
                            nextSymbol == tgoto || nextSymbol == tif ||
                            nextSymbol == toutput || nextSymbol == tbegin)
                            pST_LIST();
                        else
                            error(4); // tid, tinput, tnumber, tlparenthsis, tgoto, tif, toutput, tbegin
                    }

                    if (nextSymbol == tfi)
                        getNextSymbol();
                    else
                        error(16); // tfi
                }
                else
                    error(4); // tid, tinput, tnumber, tlparenthsis, tgoto, tif, toutput, tbegin
            }
            else
                error(15); // tthen
        }
        else
            error(11); // tinput, tid, tnumber, tlparenthsis
    }
    else
        error(14); // tif
}


void RDP::pWRITE_ST()
{
    if (nextSymbol == toutput)
    {
        apply(10);
        getNextSymbol();
        if (nextSymbol == tlparenthsis)
        {
            getNextSymbol();
            if (nextSymbol == tinput || nextSymbol == tid ||
                nextSymbol == tnumber || nextSymbol == tlparenthsis)
            {
                pEXP();
                
                while (nextSymbol == tcomma)
                {
                    getNextSymbol();

                    if (nextSymbol == tinput || nextSymbol == tid ||
                        nextSymbol == tnumber || nextSymbol == tlparenthsis)
                        pEXP();
                    else
                        error(11); // tinput, tid, tnumber, tlparenthsis
                }

                if (nextSymbol == trparenthsis)
                    getNextSymbol();
                else
                    error(9); // trparenthsis
            }
            else
                error(11); // tinput, tid, tnumber, tlparenthsis
        }
        else
            error(18); // tlparenthsis
    }
    else
        error(17); // toutput
}


void RDP::pCONDITION()
{
    if (nextSymbol == tinput || nextSymbol == tid ||
        nextSymbol == tnumber || nextSymbol == tlparenthsis)
    {
        apply(11);
        pEXP();
        if (nextSymbol == tless || nextSymbol == tgreat || nextSymbol == tequal)
        {
            getNextSymbol();
            if (nextSymbol == tinput || nextSymbol == tid ||
                nextSymbol == tnumber || nextSymbol == tlparenthsis)
                pEXP();
            else
                error(11); // tinput, tid, tnumber, tlparenthsis
        }
        else
            error(10); // tless, tgreat, tequal
    }
    else
        error(11); // tinput, tid, tnumber, tlparenthsis
}


void RDP::pEXP()
{
    if (nextSymbol == tinput || nextSymbol == tid ||
        nextSymbol == tnumber || nextSymbol == tlparenthsis)
    {
        apply(12);
        pTERM();

        while (nextSymbol == tplus || nextSymbol == tminus)
        {
            getNextSymbol();
            if (nextSymbol == tinput || nextSymbol == tid ||
                nextSymbol == tnumber || nextSymbol == tlparenthsis)
                pTERM();
            else
                error(11); // tinput, tid, tnumber, tlparenthsis
        }
    }
    else
        error(11); // tinput, tid, tnumber, tlparenthsis
}


void RDP::pTERM()
{
    if (nextSymbol == tinput || nextSymbol == tid ||
        nextSymbol == tnumber || nextSymbol == tlparenthsis)
    {
        apply(13);
        pFACTOR();

        while (nextSymbol == tmul || nextSymbol == tdiv)
        {
            getNextSymbol();
            if (nextSymbol == tinput || nextSymbol == tid ||
                nextSymbol == tnumber || nextSymbol == tlparenthsis)
                pFACTOR();
            else
                error(11); // tinput, tid, tnumber, tlparenthsis
        }
    }
    else
        error(11); // tinput, tid, tnumber, tlparenthsis
}


void RDP::pFACTOR()
{
    if (nextSymbol == tinput || nextSymbol == tid || nextSymbol == tnumber)
    {
        apply(14);
        getNextSymbol();
    }
    else if (nextSymbol == tlparenthsis)
    {
        apply(14);
        getNextSymbol();
        if (nextSymbol == tinput || nextSymbol == tid ||
            nextSymbol == tnumber || nextSymbol == tlparenthsis)
        {
            pEXP();
            if (nextSymbol == trparenthsis)
                getNextSymbol();
            else
                error(9); // trparenthsis
        }
        else
            error(11); // tinput, tid, tnumber, tlparenthsis
    }
    else
        error(11); // tinput, tid, tnumber, tlparenthsis
}