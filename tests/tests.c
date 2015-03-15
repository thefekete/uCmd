/*! \file ********************************************************************
 *
 *  \brief      Testing app for ucmd
 *  \author     Dan Fekete <thefekete@gmail.com>
 *  \date       March 10, 2015
 *
 *  \copyright  Copyright 2015, Dan Fekete <thefekete@gmail.com>,
 *              GNU GPL version 2
 *
 *  Tests functionality of ucmd module
 *
 *****************************************************************************/
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // asprintf with -D_GNU_SOURCE

#include <ucmd.h>


/*
 * LOCAL FUNCTIONS
 */

void print_help(Command cmd_list[])
{
    printf("Available Commands:\n");
    Command *p = cmd_list;
    while (p->cmd) {
        printf("\t%s \t%s\n", p->cmd, p->help);
        p++;
    }
}

int return_27_cmd(int argc, char *argv[]) {
    return 27;
}

int hello_cmd(int argc, char *argv[]) {
    printf("Hello World!\n");
    return 0;
}

int list_cmd(int argc, char *argv[]) {
    printf("1 2 3\n");
    return 0;
}

int print_cmd(int argc, char *argv[]) {
    for (int i=0; i<argc; i++) {
        printf("%s\n", argv[i]);
    }
    return 0;
}

int count_cmd(int argc, char *argv[]) {
    return argc;
}

Command cmds[] = {
    {
        .cmd = "27",
        .help = "returns 27",
        .fn = return_27_cmd,
    },
    {
        .cmd = "hello",
        .help = "says \"Hello World!\"",
        .fn = hello_cmd,
    },
    {
        .cmd = "ls",
        .help = "list some integers",
        .fn = list_cmd,
    },
    {
        .cmd = "c",
        .help = "count arguments",
        .fn = count_cmd,
    },
    {
        .cmd = "p",
        .help = "print arguments",
        .fn = print_cmd,
    },
    {},  // end of list
};


/*
 * TEST FIXTURE
 */

typedef struct {
} Fixture;

void setup(Fixture *f, gconstpointer test_data)
{
}

void teardown(Fixture *f, gconstpointer test_data)
{
}


/*
 * UNIT TESTS
 */

void test_null_list(void)
{
    g_assert_cmpint(ucmd_parse(NULL, " ", "some_cmd"), == , UCMD_CMD_NOT_FOUND);
}

void test_null_cmd(void)
{
    g_assert_cmpint(ucmd_parse(cmds, " ", NULL), == , 0);
}

void test_empty_cmd(void)
{
    g_assert_cmpint(ucmd_parse(cmds, " ", ""), == , 0);
}

void test_bad_cmd(void)
{
    g_assert_cmpint(ucmd_parse(
                cmds, " ", "bad_cmd not in list"), == , UCMD_CMD_NOT_FOUND);
}

void test_cmd_return(void)
{
    g_assert_cmpint(ucmd_parse(cmds, NULL, "27"), == , 27);
}

void test_argc(void)
{
    g_assert_cmpint(ucmd_parse(cmds, " ", "c 1 2 3 4"), == , 5);
}

void test_argv(void)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        ucmd_parse(cmds, " ", "p arg1 arg2 arg3");
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("p\n"
                              "arg1\n"
                              "arg2\n"
                              "arg3\n");
}

void test_delim(void)
{
    g_assert_cmpint(ucmd_parse(cmds, "&", "c&1&2&3&4"), == , 5);
}

void test_null_delim(void)
{
    char *s;
    asprintf(&s, "c%sarg", UCMD_DEFAULT_DELIMETER);
    int argc = ucmd_parse(cmds, UCMD_DEFAULT_DELIMETER, s);
    free(s);
    g_assert_cmpint(argc, == , 2);
}

void test_dbl_delim(void)
{
    g_assert_cmpint(ucmd_parse(cmds, ":", "c::4"), == , 2);
    g_assert_cmpint(ucmd_parse(cmds, ":", "c:::::::4"), == , 2);
}

void test_trail_delim(void)
{
    g_assert_cmpint(ucmd_parse(cmds, ":", "c:4:"), == , 2);
    g_assert_cmpint(ucmd_parse(cmds, ":", "c:4::::"), == , 2);
}

void test_hello(void)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        ucmd_parse(cmds, NULL, "hello");
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("Hello World!\n");
}

void test_hello_extra_args(void)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        ucmd_parse(cmds, " ", "hello with extra arguments");
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("Hello World!\n");
}

void test_ls(void)
{
    if (g_test_trap_fork(0, G_TEST_TRAP_SILENCE_STDOUT)) {
        ucmd_parse(cmds, NULL, "ls");
        exit(0);
    }
    // rerun this test in a subprocess
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout("1 2 3\n");
}


/*
 * TEST MAIN
 */
int main(int argc, char** argv)
{
    //print_commands(cmds);
    print_help(cmds);

    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/ucmd/null list", test_null_list);
    g_test_add_func("/ucmd/null cmd", test_null_cmd);
    g_test_add_func("/ucmd/empty cmd", test_empty_cmd);
    g_test_add_func("/ucmd/bad cmd", test_bad_cmd);
    g_test_add_func("/ucmd/hello", test_hello);
    g_test_add_func("/ucmd/cmd return", test_cmd_return);
    g_test_add_func("/ucmd/argc", test_argc);
    g_test_add_func("/ucmd/argv", test_argv);
    g_test_add_func("/ucmd/delimeter", test_delim);
    g_test_add_func("/ucmd/null delimeter", test_null_delim);
    g_test_add_func("/ucmd/double delimeter", test_dbl_delim);
    g_test_add_func("/ucmd/trailing delimeter", test_trail_delim);
    g_test_add_func("/ucmd/hello extra args", test_hello_extra_args);
    g_test_add_func("/ucmd/ls", test_ls);

    return g_test_run();
}
