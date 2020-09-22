#include "Header.h"

int binary_search_index_pos(const char* file_name, int idx, int r) {
	int pos = -1;
	int l = 0;

	FILE* file = fopen(file_name, "r+b");
	int flag = 0;
	int mid;
	while ((l <= r) && (flag != 1)) {
		mid = (l + r) / 2;

		struct Index i;
		fseek(file, mid * (sizeof(struct Index)), 0);
		fread(&i, sizeof(struct Index), 1, file);

		if (i.id == idx) {
			flag = 1;
			pos = i.position;
		}
		else if (i.id > idx) r = mid - 1;
		else l = mid + 1;
	}
	fclose(file);
	return pos;
}

void help() {
	system("cls");

	printf("Command:\n");
	printf("inform - information about ammount of records.\n");
	printf("insert-m - add an entry to the pupil table.\n");
	printf("insert-s - add an entry to the school class table.\n");
	printf("get-m - get information about student by id.\n");
	printf("get-s - get information about school class by id.\n");
	printf("get-a - get all students from school class.\n");
	printf("ut-m - get information about all students.\n");
	printf("ut-s - get information about all school class.\n");
	printf("update-m - edit information about student.\n");
	printf("update-s - edit information about school class.\n");
	printf("del-m - delete information about student.\n");
	printf("del-s - delete information about school class.\n");
	printf("exit - exit from the program.\n");

	system("pause");
}

void inform() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	printf("Ammount of records - pupil: %d\n", info.ammount_of_pupil);
	printf("Ammount of records - school class: %d\n", info.ammount_of_school);
	fclose(size_info);

	system("pause");
}

void insert_m() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);

	if (info.ammount_of_school == 0) {
		printf("Closed!\n");
	}
	else {
		FILE* index_file = fopen("pupil.idx", "r+b");
		FILE* data_file = fopen("pupil.fl", "r+b");
		struct Index new_index;
		new_index.id = info.new_index_pupil;
		new_index.position = info.ammount_of_pupil;

		struct Pupil new_pupil;
		new_pupil.id = new_index.id;
		printf("Enter student name: ");
		scanf("%19s", &new_pupil.name);
		printf("Enter student surname: ");
		scanf("%19s", &new_pupil.surname);
		printf("Enter student age: ");
		scanf("%d", &new_pupil.age);
		printf("Enter student average mark: ");
		scanf("%f", &new_pupil.average_mark);
		FILE* class_list = fopen("school_class.fl", "r+b");
		printf("You are given a sheet from school classes, choose from[1 - %d", info.ammount_of_school);
		printf("]:\n");
		for (int i = 0; i < info.ammount_of_school; i++) {
			fseek(class_list, i * (sizeof(struct School_class)), 0);
			struct School_class list_of_class;
			fread(&list_of_class, sizeof(struct School_class), 1, class_list);
			printf("%d", i + 1);
			printf(". (%d-%s) Classroom teacher: %s\n", list_of_class.form, list_of_class.letter, list_of_class.classroom_teacher_surname);
		}
		printf("Enter: ");
		int idx;
		scanf("%d", &idx);
		if (idx == 0) {
			new_pupil.id_school_class = 0;
		}
		else {
			struct School_class choosen_class;
			fseek(class_list, (idx - 1) * (sizeof(struct School_class)), 0);
			fread(&choosen_class, sizeof(struct School_class), 1, class_list);
			new_pupil.id_school_class = choosen_class.id;
			add_to_func(new_pupil.id, choosen_class.root);
			fseek(class_list, (idx - 1) * (sizeof(struct School_class)), 0);
			fwrite(&choosen_class, sizeof(struct School_class), 1, class_list);
		}
		fclose(class_list);

		fseek(index_file, info.ammount_of_pupil * (sizeof(struct Index)), 0);
		fwrite(&new_index, sizeof(struct Index), 1, index_file);
		fseek(data_file, new_index.position * (sizeof(struct Pupil)), 0);
		fwrite(&new_pupil, sizeof(struct Pupil), 1, data_file);
		fclose(data_file);
		fclose(index_file);

		printf("New student added. Id: %d\n", new_pupil.id);
		info.ammount_of_pupil++;
		info.new_index_pupil++;
		fseek(size_info, 0, 0);
		fwrite(&info, sizeof(struct Size_of_database), 1, size_info);
	}
	fclose(size_info);
	system("pause");
}

void insert_s() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);

	FILE* index_file = fopen("school_class.idx", "r+b");
	FILE* data_file = fopen("school_class.fl", "r+b");
	struct Index new_index;
	new_index.id = info.new_index_school;
	new_index.position = info.ammount_of_school;

	struct School_class new_school;
	new_school.id = new_index.id;
	printf("Enter form: ");
	scanf("%d", &new_school.form);
	printf("Enter letter: ");
	scanf("%s", &new_school.letter);
	printf("Enter classroom teacher surname: ");
	scanf("%s", &new_school.classroom_teacher_surname);
	new_school.root = init_func();

	fseek(index_file, info.ammount_of_school * (sizeof(struct Index)), 0);
	fwrite(&new_index, sizeof(struct Index), 1, index_file);
	fseek(data_file, new_index.position * (sizeof(struct School_class)), 0);
	fwrite(&new_school, sizeof(struct School_class), 1, data_file);
	fclose(data_file);
	fclose(index_file);

	printf("New class added. Id: %d\n", new_school.id);
	info.ammount_of_school++;
	info.new_index_school++;
	fseek(size_info, 0, 0);
	fwrite(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);
	system("pause");
}

void get_m() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);

	int idx;
	const char file_name[30] = "pupil.idx";
	printf("Enter id: ");
	scanf("%d", &idx);
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_pupil - 1);

	if (pos == -1) printf("There are no records in the database with this id.\n");
	else {
		struct Pupil student;
		FILE* data_file = fopen("pupil.fl", "r+b");
		fseek(data_file, pos * (sizeof(struct Pupil)), 0);
		fread(&student, sizeof(struct Pupil), 1, data_file); 
		fclose(data_file);

		printf("Student id: %d\n", student.id);
		printf("Name: %s\n", student.name);
		printf("Surname: %s\n", student.surname);
		printf("Age: %d\n", student.age);
		printf("Average mark: %f\n", student.average_mark);
		printf("Student class id: %d\n", student.id_school_class);
	}
	system("pause");
}

void get_s() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);

	int idx;
	const char file_name[30] = "school_class.idx";
	printf("Enter id: ");
	scanf("%d", &idx);
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_school - 1);

	if (pos == -1) printf("There are no records in the database with this id.\n");
	else {
		struct School_class class_idx;
		FILE* data_file = fopen("school_class.fl", "r+b");
		fseek(data_file, pos * (sizeof(struct School_class)), 0);
		fread(&class_idx, sizeof(struct School_class), 1, data_file);
		fclose(data_file);

		printf("School class id: %d\n", class_idx.id);
		printf("Form: %d\n", class_idx.form);
		printf("Letter: %s\n", class_idx.letter);
		printf("Classroom teacher surname: %s\n", class_idx.classroom_teacher_surname);
	}
	system("pause");
}

void ut_m() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);

	FILE* data_file = fopen("pupil.fl", "r+b");
	FILE* index_file = fopen("pupil.idx", "r+b");
	for (int i = 0; i < info.ammount_of_pupil; i++) {
		struct Index ids;
		struct Pupil students;

		fseek(index_file, i * (sizeof(struct Index)), 0);
		fread(&ids, sizeof(struct Index), 1, index_file); 

		fseek(data_file, ids.position * (sizeof(struct Pupil)), 0);
		fread(&students, sizeof(struct Pupil), 1, data_file);

		printf("%d :\n Student id: %d\n", ids.id, students.id);
		printf("Name: %s\n", students.name);
		printf("Surname: %s\n", students.surname);
		printf("Age: %d\n", students.age);
		printf("Average mark: %f\n", students.average_mark);
		printf("Student class id: %d\n", students.id_school_class);
	}
	fclose(data_file);
	fclose(index_file);

	system("pause");
}

void ut_s() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);

	FILE* data_file = fopen("school_class.fl", "r+b");
	FILE* index_file = fopen("school_class.idx", "r+b");
	for (int i = 0; i < info.ammount_of_school; i++) {
		struct Index ids;
		struct School_class class_to_read;

		fseek(index_file, i * (sizeof(struct Index)), 0);
		fread(&ids, sizeof(struct Index), 1, index_file);

		fseek(data_file, ids.position * (sizeof(struct School_class)), 0);
		fread(&class_to_read, sizeof(struct School_class), 1, data_file);

		printf("%d :\n School class id: %d\n", ids.id, class_to_read.id);
		printf("Form: %d\n", class_to_read.form);
		printf("Letter: %s\n", class_to_read.letter);
		printf("Classroom teacher surname: %s\n", class_to_read.classroom_teacher_surname);
	}
	fclose(data_file);
	fclose(index_file);

	system("pause");
}

void update_m() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);

	int idx;
	const char file_name[30] = "pupil.idx";
	printf("Enter id: ");
	scanf("%d", &idx);
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_pupil - 1);

	if (pos == -1) printf("There are no records in the database with this id.\n");
	else {
		struct Pupil student;
		FILE* data_file = fopen("pupil.fl", "r+b");
		fseek(data_file, pos * (sizeof(struct Pupil)), 0);
		fread(&student, sizeof(struct Pupil), 1, data_file);

		printf("Student id: %d\n", student.id);
		printf("Name: %s\n", student.name);
		printf("Surname: %s\n", student.surname);
		printf("Age: %d\n", student.age);
		printf("Average mark: %f\n", student.average_mark);
		printf("Student class id: %d\n", student.id_school_class);

		int choice;
		printf("What would you like to change? 1 - Name, 2 - Surname, 3 - Age, 4 - Average mark, 0 - nothing.\n Enter: ");
		scanf("%d", &choice);
		if (choice == 1) {
			printf("Enter name: ");
			scanf("%s", &student.name);
			
		} else if (choice == 2) {
			printf("Enter surname: ");
			scanf("%s", &student.surname);
		} else if (choice == 3) {
			printf("Enter age: ");
			scanf("%d", &student.age);
		} else if (choice == 4) {
			printf("Enter average mark: ");
			scanf("%f", &student.average_mark);
		} 
		fseek(data_file, pos * (sizeof(struct Pupil)), 0);
		fwrite(&student, sizeof(struct Pupil), 1, data_file);
		fclose(data_file);
		printf("Successfully!\n");
	}
	system("pause");
}

void update_s() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);

	int idx;
	const char file_name[30] = "school_class.idx";
	printf("Enter id: ");
	scanf("%d", &idx);
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_school - 1);

	if (pos == -1) printf("There are no records in the database with this id.\n");
	else {
		struct School_class class_idx;
		FILE* data_file = fopen("school_class.fl", "r+b");
		fseek(data_file, pos * (sizeof(struct School_class)), 0);
		fread(&class_idx, sizeof(struct School_class), 1, data_file);

		printf("School class id: %d\n", class_idx.id);
		printf("Form: %d\n", class_idx.form);
		printf("Letter: %s\n", class_idx.letter);
		printf("Classroom teacher surname: %s\n", class_idx.classroom_teacher_surname);

		int choice;
		printf("What would you like to change? 1 - Form, 2 - Letter, 3 - Classroom teacher surname, 0 - nothing.\n Enter: ");
		scanf("%d", &choice);
		if (choice == 1) {
			printf("Enter form: ");
			scanf("%d", &class_idx.form);

		}
		else if (choice == 2) {
			printf("Enter letter: ");
			scanf("%s", &class_idx.letter);
		}
		else if (choice == 3) {
			printf("Enter classroom teacher surname: ");
			scanf("%s", &class_idx.classroom_teacher_surname);
		}

		fseek(data_file, pos * (sizeof(struct School_class)), 0);
		fwrite(&class_idx, sizeof(struct School_class), 1, data_file);
		fclose(data_file);
		printf("Successfully!\n");

	}
	system("pause");
}

void del_m() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);

	int idx;
	const char file_name[30] = "pupil.idx";
	printf("Enter id: ");
	scanf("%d", &idx);
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_pupil - 1);

	if (pos == -1) printf("There are no records in the database with this id.\n");
	else {
		struct Pupil student;
		FILE* data_file = fopen("pupil.fl", "r+b");
		fseek(data_file, pos * (sizeof(struct Pupil)), 0);
		fread(&student, sizeof(struct Pupil), 1, data_file);

		printf("Student id: %d\n", student.id);
		printf("Name: %s\n", student.name);
		printf("Surname: %s\n", student.surname);
		printf("Age: %d\n", student.age);
		printf("Average mark: %f\n", student.average_mark);
		printf("Student class id: %d\n", student.id_school_class);

		int choice;
		printf("Do you sure want to delete this one (yes - 1/no - 0)? ");
		scanf("%d", &choice);

		if (choice == 1) {
			const char file_name[30] = "school_class.idx";
			int pos1 = binary_search_index_pos(file_name, idx, info.ammount_of_school - 1);
			struct School_class class_idx;
			FILE* data_file1 = fopen("school_class.fl", "r+b");
			fseek(data_file1, pos1 * (sizeof(struct School_class)), 0);
			fread(&class_idx, sizeof(struct School_class), 1, data_file1);
			struct list* a = deletelem(student.id, class_idx.root);
			class_idx.root = a;
			fseek(data_file1, pos1 * (sizeof(struct School_class)), 0);
			fwrite(&class_idx, sizeof(struct School_class), 1, data_file1);
			fclose(data_file1);

			struct Pupil transfer;

			fseek(data_file, (info.ammount_of_pupil - 1) * (sizeof(struct Pupil)), 0);
			fread(&transfer, sizeof(struct Pupil), 1, data_file);
			fseek(data_file, pos * (sizeof(struct Pupil)), 0);
			fwrite(&transfer, sizeof(struct Pupil), 1, data_file);

			FILE* index_file = fopen("pupil.idx", "r+b");
			for (int i = 0; i < info.ammount_of_pupil; i++) {
				struct Index indx_transfer;
				fseek(index_file, i * (sizeof(struct Index)), 0);
				fread(&indx_transfer, sizeof(struct Index), 1, index_file);

				if (indx_transfer.id == transfer.id) indx_transfer.position = pos;
				if (indx_transfer.id > idx) {
					fseek(index_file, (i - 1) * (sizeof(struct Index)), 0);
					fwrite(&indx_transfer, sizeof(struct Index), 1, index_file);
				}
				else {
					fseek(index_file, i * (sizeof(struct Index)), 0);
					fwrite(&indx_transfer, sizeof(struct Index), 1, index_file);
				}
			}
			info.ammount_of_pupil--;
			fseek(size_info, 0, 0);
			fwrite(&info, sizeof(struct Size_of_database), 1, size_info);
			fclose(index_file);
		}
		fclose(data_file);
	}
	printf("Successfully!\n");
	fclose(size_info);
	system("pause");
}

void del_s() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);

	int idx;
	const char file_name[30] = "school_class.idx";
	printf("Enter id: ");
	scanf("%d", &idx);
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_school - 1);

	if (pos == -1) {
		printf("There are no records in the database with this id.\n");
		fclose(size_info);
	}
	else {
		struct School_class class_idx;
		FILE* data_file = fopen("school_class.fl", "r+b");
		fseek(data_file, pos * (sizeof(struct School_class)), 0);
		fread(&class_idx, sizeof(struct School_class), 1, data_file);

		printf("School class id: %d\n", class_idx.id);
		printf("Form: %d\n", class_idx.form);
		printf("Letter: %s\n", class_idx.letter);
		printf("Classroom teacher surname: %s\n", class_idx.classroom_teacher_surname);

		int choice;
		printf("Do you sure want to delete this one (yes - 1/no - 0)? ");
		scanf("%d", &choice);
		if (choice == 1) {
			struct School_class transfer;

			fseek(data_file, (info.ammount_of_school - 1) * (sizeof(struct School_class)), 0);
			fread(&transfer, sizeof(struct School_class), 1, data_file);
			fseek(data_file, pos * (sizeof(struct School_class)), 0);
			fwrite(&transfer, sizeof(struct School_class), 1, data_file);

			FILE* ix_file = fopen("school_class.idx", "r+b");
			for (int j = 0; j < info.ammount_of_school; j++) {
				struct Index indx_transfer;
				fseek(ix_file, j * (sizeof(struct Index)), 0);
				fread(&indx_transfer, sizeof(struct Index), 1, ix_file);

				if (indx_transfer.id == transfer.id) indx_transfer.position = pos;
				if (indx_transfer.id > idx) {
					fseek(ix_file, (j - 1) * (sizeof(struct Index)), 0);
					fwrite(&indx_transfer, sizeof(struct Index), 1, ix_file);
				}
				else {
					fseek(ix_file, j * (sizeof(struct Index)), 0);
					fwrite(&indx_transfer, sizeof(struct Index), 1, ix_file);
				}
			}
			info.ammount_of_school--;
			fseek(size_info, 0, 0);
			fwrite(&info, sizeof(struct Size_of_database), 1, size_info);
			fclose(ix_file);
			fclose(size_info);
			del_all_from(class_idx.root);
		} else fclose(size_info);
		fclose(data_file);
	}
	printf("Successfully!\n");
	system("pause");
}

struct list* init_func()
{
	struct list* lst;
	lst = (struct list*)malloc(sizeof(struct list));
	lst->id = 0;
	lst->ptr = NULL;
	return(lst);
}

void add_to_func(int idx, struct list* root)
{
	struct list* lst;
	lst = root;
	if (lst->id == 0) lst->id = idx;
	else {
		struct list* temp;
		temp = (struct list*)malloc(sizeof(struct list));
		temp->id = idx;
		temp->ptr = NULL;
		while (lst->ptr != NULL)
		{
			lst = lst->ptr;
		}
		lst->ptr = temp;
	}
}

void del_all_from(struct list* lst)
{
	struct list* p;
	p = lst;
	do {
		del_m_by_id(p->id);
		p = p->ptr;
	} while (p != NULL);
}

void get_all_students(struct list* lst) {
	struct list* p;
	p = lst;
	printf("Students: \n");
	do {
		if (p->id == 0) printf("No students in class. \n");
		else get_m_s(p->id);
		p = p->ptr;
	} while (p != NULL);
}

struct list* deletelem(int idx, struct list* root) {
	struct list* temp;
	temp = root;
	if (temp->id == idx) {
		temp = root->ptr;
		if (root->ptr == NULL) {
			struct list* lst;
			lst = (struct list*)malloc(sizeof(struct list));
			lst->id = 0;
			lst->ptr = NULL;
			return lst;
		}
		free(root);
		return temp;
	}
	else {
		struct list* lst;
		while ((temp->ptr)->id != idx) temp = temp->ptr;
		lst = temp->ptr;
		temp->ptr = lst->ptr;
		free(lst);
		return root;
	}
}

void del_m_by_id(int idx) {
	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);

	const char file_name[30] = "pupil.idx";
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_pupil - 1);

	FILE* data_file = fopen("pupil.fl", "r+b");
	struct Pupil transfer;

	fseek(data_file, (info.ammount_of_pupil - 1) * (sizeof(struct Pupil)), 0);
	fread(&transfer, sizeof(struct Pupil), 1, data_file);
	fseek(data_file, pos * (sizeof(struct Pupil)), 0);
	fwrite(&transfer, sizeof(struct Pupil), 1, data_file);

	FILE* index_file = fopen("pupil.idx", "r+b");
	for (int i = 0; i < info.ammount_of_pupil; i++) {
		struct Index indx_transfer;
		fseek(index_file, i * (sizeof(struct Index)), 0);
		fread(&indx_transfer, sizeof(struct Index), 1, index_file);

		if (indx_transfer.id == transfer.id) indx_transfer.position = pos;
		if (indx_transfer.id > idx) {
			fseek(index_file, (i - 1) * (sizeof(struct Index)), 0);
			fwrite(&indx_transfer, sizeof(struct Index), 1, index_file);
		}
		else {
			fseek(index_file, i * (sizeof(struct Index)), 0);
			fwrite(&indx_transfer, sizeof(struct Index), 1, index_file);
		}
	}
	info.ammount_of_pupil--;
	fseek(size_info, 0, 0);
	fwrite(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(index_file);
    fclose(data_file);
	fclose(size_info);
}

void show_all_student_in_class() {
	system("cls");

	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);

	int idx;
	const char file_name[30] = "school_class.idx";
	printf("Enter id: ");
	scanf("%d", &idx);
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_school - 1);

	if (pos == -1) printf("There are no records in the database with this id.\n");
	else {
		struct School_class class_idx;
		FILE* data_file = fopen("school_class.fl", "r+b");
		fseek(data_file, pos * (sizeof(struct School_class)), 0);
		fread(&class_idx, sizeof(struct School_class), 1, data_file);
		fclose(data_file);

		printf("School class id: %d\n", class_idx.id);
		printf("Form: %d\n", class_idx.form);
		printf("Letter: %s\n", class_idx.letter);
		printf("Classroom teacher surname: %s\n", class_idx.classroom_teacher_surname);

		get_all_students(class_idx.root);
	}
	system("pause");
}

void get_m_s(int idx) {
	FILE* size_info = fopen("size_database.dat", "r+b");
	struct Size_of_database info;
	fread(&info, sizeof(struct Size_of_database), 1, size_info);
	fclose(size_info);

	const char file_name[30] = "pupil.idx";
	int pos = binary_search_index_pos(file_name, idx, info.ammount_of_pupil - 1);

	struct Pupil student;
	FILE* data_file = fopen("pupil.fl", "r+b");
	fseek(data_file, pos * (sizeof(struct Pupil)), 0);
	fread(&student, sizeof(struct Pupil), 1, data_file);
	fclose(data_file);

	printf("Student id: %d\n", student.id);
	printf("Name: %s\n", student.name);
	printf("Surname: %s\n", student.surname);
	printf("Age: %d\n", student.age);
	printf("Average mark: %f\n", student.average_mark);
	printf("Student class id: %d\n", student.id_school_class);
	printf("\n");
}