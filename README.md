# uCmd
Simple command parser for microcontrollers

The full documentation can be found on the project's
[gh-pages site](http://thefekete.github.io/uCmd/).

uCmd is a very simple command line parser designed for use in
microcontrollers. It is just a function pointer typedef, struct typedef and
a single interface funcion. Check out \ref example to get started quickly.

# Basic Usage
```c
// define some command callbacks

int print_help_cb(int argc, char *argv[])
{
    printf("Available Commands:\n");
    Command *p = my_cmd_list;
    while (p->cmd) {
        printf("\t%s \t%s\n", p->cmd, p->help);
        p++;
    }

    return 0;
}

int led_cb(int argc, char *argv[])
{
    if (argc < 2)
        printf("requires argument of 'ON' or 'OFF'\n");

    if (strcmp(argv[1], "ON")==0) {
        LED_ON();
    } else if (strcmp(argv[1], "OFF")==0) {
        LED_OFF();
    }
}


// define command list
Command my_cmd_list[] = {
    {
        .cmd = "?",
        .help = "print available commands with their help text",
        .fn = print_help_cb,
    },
    {
        .cmd = "led",
        .help = "<ON|OFF> turns led on or off",
        .fn = led_cb,
    },
    {}, // null list terminator DON'T FORGET THIS!
}

int main()
{
    char *str1 = "?";
    char *str2 = "led ON";
    char *str2 = "bad cmd";

    ucmd_parse(my_cmd_list, " ", str1);                   // prints help message
    ucmd_parse(my_cmd_list, " ", str2);                          // turns led on
    int ret = ucmd_parse(my_cmd_list, " ", str3);   // ret == UCMD_CMD_NOT_FOUND
}
```
