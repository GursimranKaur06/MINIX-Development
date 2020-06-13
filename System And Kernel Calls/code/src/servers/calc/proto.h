#ifndef _CALCSERVER_PROTO_H
#define _CALCSERVER_PROTO_H

/* main.c */
int main(int argc, char **argv);

/* calc.c */
int do_add(message *m);
int do_multiply(message *m);

#endif
