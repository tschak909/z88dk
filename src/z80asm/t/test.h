//-----------------------------------------------------------------------------
// z80asm restart
// unit tests
// Copyright (C) Paulo Custodio, 2011-20180
// License: http://www.perlfoundation.org/artistic_license_2_0
//-----------------------------------------------------------------------------
#pragma once
#include "../pch.h"

class RedirectOutput
{
public:
    RedirectOutput();
    ~RedirectOutput();
    string out() const;
    string err() const;

private:
    streambuf* old_out;
    streambuf* old_err;
    stringstream out_buffer;
    stringstream err_buffer;
};

void is_ok(bool value, const string& filename, int line_nr);

#define DIAG(out)   cout << "# " << out << endl
#define OK(x)       is_ok((x), __FILE__, __LINE__)
#define NOK(x)      OK(!(x))
#define IS(a, b)    do { OK((a) == (b)); if ((a) != (b)) { DIAG(a); DIAG(b); } } while (0)
#define ISNT(a, b)  do { OK((a) != (b)); if ((a) == (b)) { DIAG(a); DIAG(b); } } while (0)
#define PASS()      OK(true)
#define FAIL()      OK(false)
#define ASSERT(x)   do { OK(x); if (!(x)) return; } while (0)
#define IS_STR(a, b) do {  string sa(a), sb(b); \
                OK(sa == sb); \
                if (sa != sb) { \
                            DIAG("<<<"); \
                            DIAG(sa); \
                            DIAG(">>>"); \
                            DIAG(sb); \
                            DIAG("---"); \
                        } \
                } while (0)
#define START_CAPTURE() \
            {   string got_out, got_err; \
            {   RedirectOutput redir; \

#define END_CAPTURE(out_, err_) \
                got_out = redir.out(); \
                got_err = redir.err(); \
            } \
            IS_STR(out_, got_out); \
            IS_STR(err_, got_err); \
            }
