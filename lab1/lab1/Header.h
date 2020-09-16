#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Index {
	int id;
	int position;
};

struct School_class {
	int id;
	int form;
	char letter[3];
	char classroom_teacher_surname[20];
};

struct Pupil {
	int id;
	char name[20];
	char surname[20];
	int age;
	int id_school_class;
	float average_mark;
};

struct Size_of_database {
	int ammount_of_pupil;
	int ammount_of_school;
	int new_index_pupil;
	int new_index_school;
};

void insert_m();
void insert_s();
void get_m();
void get_s();
void update_m();
void update_s();
void del_m();
void del_s();
void help();
void inform();
int binary_search_index_pos(const char *file_name, int idx, int r);