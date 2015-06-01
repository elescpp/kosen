#include <stdio.h>
#define type int

#include "list.h"

void print_int(int* n){
	printf("%d\n", *n);
}

int main(void)
{
	list__type* list;
	list_init__type(&list);
	int i;
	list_foreach__type(list, print_int);
}
