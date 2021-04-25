#include "2-3_tree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	node* root = NULL;
	while (1)
	{
		int tmp;
		char c;
		printf("What do you want to do?\n");
		printf("Press 'a' to add new number\n");
		printf("Press 'd' to delete a number\n");
		printf("Press 'f' to find a number\n");
		printf("Press 'e' to end\n");
		scanf("%c", &c);
		if (c == '\n')
			scanf("%c", &c);
		if (c == 'a')
		{
			printf("Please enter number\n");
			scanf("%d", &tmp);
			if (c == '\n')
				scanf("%c", &c);
			root = insert(root, tmp);
		}
		else if (c == 'd') {
			printf("Please enter number\n");
			scanf("%d", &tmp);
			if (c == '\n')
				scanf("%c", &c);
			root = del_key(root, tmp);
		}
		else if (c == 'f') {
			printf("Please enter number\n");
			scanf("%d", &tmp);
			if (c == '\n')
				scanf("%c", &c);
			if (!search(root, tmp))
				printf("Not found\n");
			else
				printf("found\n");
		}
		else if (c == 'e')
			break;
		else
			printf("Unknown command\n");
	}
	del_tree(&root);
	return 0;
}