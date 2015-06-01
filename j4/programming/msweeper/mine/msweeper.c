#include <stdio.h>
#include <stdlib.h>

typedef enum{
	//visual
	unknown = 1 << 0,
	opened,
	normal,

	//internal infomation
	bomb = 1 << 3,
	empty,
}ms_flag;

typedef struct{
	char flag_;
	char num_bombs_;
}mass;

typedef struct{
	mass* mass_;
	int width_;
	int height_;
	int num_mass_;
}board;

board* ms_make_board(const int width, const int height){
	board* temp = malloc(sizeof(board));
	temp->mass_ = malloc(sizeof(mass)*(width+2)*(height+2));
	temp->width_ = width;
	temp->height_= height;
	temp->num_mass_ = (width+2)*(height+2);

	int i;
	for(i = 0; i < (width+2)*(height+2); i++){
		temp->mass_[i].flag_ = 0;
	}

	return temp;
}

mass* ms_ref_mass(board* bd, int x, int y){
	return bd->mass_+(bd->width_+2)*(y+1)+(x+1);
}

void ms_set_flag(board* bd, int x, int y, ms_flag flag){
	(*(ms_ref_mass(bd, x, y))).flag_ |= flag;
}

void ms_remove_flag(board* bd, int x, int y, ms_flag flag)
{
	(*(ms_ref_mass(bd, x, y))).flag_ &= ~flag;
}

ms_flag ms_get_flag(board* bd, int x, int y){
	return ms_ref_mass(bd, x, y)->flag_;
}

int ms_check_flag(board* bd, int x, int y, ms_flag flag){
	return ms_get_flag(bd, x, y) && flag;
}

void ms_print_board(board* bd)
{
	int i, j;
	for(i = 0; i < bd->width_; i++){
		for(j = 0; j < bd->height_; j++){
		}
	}
}

void ms_init_board(board* bd){
	int i, j;
	for(i = 0; i < bd->width_; i++){
		for(j = 0; j < bd->height_; j++){
			if(rand() % 10 == 0){
				ms_set_flag(bd, i, j, bomb);
			}
		}
	}
}

int main(void)
{
}

