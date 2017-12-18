//
// Created by nirao on 18.12.17.
//

#include "utest/utest.h"
#include "unity/unity.h"
#include "greentea-client/test_env.h"

using namespace utest::v1;

SDStorage sd(PTE3, PTE1, PTE2, PTE4);

void TESTRenameFile(){

    TEST_ASSERT_EQUAL(0, sd.init(), "Failed to init SD");

    sd.ls(DEFAULT_MOUNT_ADDRESS);

    sd.write("HELLO", "hello world", 11, 1);

    TEST_ASSERT_EQUAL(0, sd.renameFile("HELLO", "world"), "Failed to rename file");

    sd.ls(DEFAULT_MOUNT_ADDRESS);
}

utest::v1::status_t case_teardown_handler(const Case *const source, const size_t passed, const size_t failed,
                                        const failure_t reason){
    return greentea_case_teardown_handler(source, passed, failed, reason);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(60, "SD-Card Test");
    return verbose_test_setup_handler(number_of_cases);
}

int main() {

    Case cases[] = {
            Case("File-Rename", TESTRenameFile, case_teardown_handler, greentea_failure_handler);
    };

    Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);
    return !Harness::run(specification);
}