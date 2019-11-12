// Unit test / mocking framework, cmocka:
#include <cmocka.h>
#include <unistd.h> // chdir()

// Functionality we are testing:
#include <parse_lib.h>
#include <file_lib.h>
#include <string_lib.h>

#include <stdlib.h>

static void parse_test_cf(void **data)
{
    chdir("../../examples");

    Seq *const policy_files = ListDir(".", ".cf");
    assert_true(policy_files != NULL);
    const size_t length = SeqLength(policy_files);
    assert_true(length > 0);

    for (int i = 0; i < length; ++i)
    {
        const char *const filename = SeqAt(policy_files, i);
        assert_true(StringEndsWith(filename, ".cf"));
        if (StringEndsWith(filename, ".x.cf"))
        {
            PolicyFile *policy = ParseFile(filename);
            assert_true(policy == NULL);
        }
        else
        {
            PolicyFile *policy = ParseFile(filename);
            assert_true(policy != NULL);
            free(policy);
        }
    }
    SeqDestroy(policy_files);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(parse_test_cf),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
