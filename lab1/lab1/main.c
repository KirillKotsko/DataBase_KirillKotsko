#include "Header.h"

int main()
{
	while (1) {

		char command[30];
		printf("Input command: ");
		scanf("%29s", command);

		if (strcmp(command, "insert-m") == 0) {
			insert_m();
		}
		else if (strcmp(command, "insert-s") == 0) {
			insert_s();
		}
		else if (strcmp(command, "get-m") == 0) {
			get_m();
		}
		else if (strcmp(command, "get-s") == 0) {
			get_s();
		}
		else if (strcmp(command, "update-m") == 0) {
			update_m();
		}
		else if (strcmp(command, "update-s") == 0) {
			update_s();
		}
		else if (strcmp(command, "del-m") == 0) {
			del_m();
		} 
		else if (strcmp(command, "del-s") == 0) {
			del_s();
		}
		else if (strcmp(command, "inform") == 0) {
			inform();
		}
		else if (strcmp(command, "help") == 0) {
			help();
		}
		else if (strcmp(command, "exit") == 0) {
			break;
		}
		system("cls");
	}
	return 0;
}