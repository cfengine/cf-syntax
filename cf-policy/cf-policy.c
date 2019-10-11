#include <parse_lib.h>

int main(int argc, const char *const *argv)
{
    if (ParseFileStream(stdin))
    {
        return 0;
    }
    return 1;
}
