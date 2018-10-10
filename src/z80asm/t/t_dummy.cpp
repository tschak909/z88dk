//-----------------------------------------------------------------------------
// z80asm unit tests
// Copyright (C) Paulo Custodio, 2011-20180
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#include "test.h"

void test_redirect()
{
    cout << "# standard output" << endl;
    cerr << "# standard error" << endl;

    START_CAPTURE();
    cout << "# redirected output" << endl;
    cerr << "# redirected error" << endl;
    END_CAPTURE("# redirected output\n",
                "# redirected error\n");
}
