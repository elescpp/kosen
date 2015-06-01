#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//A node that consists of a list to store problems the player failed.
typedef struct string_list_node{
	char *str_;

	struct string_list_node* next_;
}string_list_node;

//Creates a list object and let the passed pointer point to the list
void string_list_create(string_list_node** list){
	*list = malloc(sizeof(string_list_node));
	(*list)->str_ = NULL;
	(*list)->next_ = NULL;
}

void print_failed_problems(string_list_node* head){
	string_list_node* node = head->next_;
	while(node != NULL){
		printf("%s\n", node->str_);
		node = node->next_;
		printf("%s\n\n", node->str_);
		node = node->next_;
	}
}

//Deallocates memory
void string_list_destroy(string_list_node* target){
	if(target == NULL){
		return;
	}
	string_list_node* next = target->next_;
	free(target->str_);
	free(target);

	string_list_destroy(next);
}

//Adds an string next to the "list"
string_list_node* string_list_add(string_list_node* list, char const* str){
	list->next_ = malloc(sizeof(string_list_node));
	list->next_->str_ = malloc(strlen(str) + 1);
	list->next_->next_ = NULL;
	strcpy(list->next_->str_, str);

	return list->next_;
}

char getChar(void);

typedef enum{
	used,
	unused,
}alphabet_status;

typedef enum{
	succeeded,
	failed,
}result;

typedef struct{
	//Use this pointer to deallocate the memory. This poins to memory storing the line of a current problem.
	char *to_free_;

	//These 2 pointers point to a string representing an answer and the meaning in Japanese.
	char const* answer_;
	char const* nihongo_;

	//A string to be displayed, must be freed.
	char* disp_;

	//The length of a answer word
	int answer_length_;

	//The number of times the player can fail.
	int remaining_;

	//To manage used alphabets
	alphabet_status alphabet_map_[26];
}hangman_context;


//Counts the total line of "f"
size_t FILE_count_line(FILE* f){
	int temp;
	int line_ = 0;

	for(temp = fgetc(f); temp != EOF; temp = fgetc(f)){
		if(temp == '\n')
			line_++;
	}
	fseek(f, 0, SEEK_SET);

	return line_;
}

//Return value: A pointer to memory storing a string of the desired line. The pointer should be freed by yourself.
//If the "line" is greater than the actual number of f's line, behaviour is undefined.
char* FILE_get_line_at(FILE* f, size_t line){
	int i = 0;
	char* read;

	line--;
	while(line > 0){
		if(fgetc(f) == '\n')
			line--;
	}

	int num_char = 0;
	while(fgetc(f) != '\n'){
		num_char++;
	}

	num_char++;
	fseek(f, -num_char, SEEK_CUR);

	read = malloc(num_char+1);

	fgets(read, num_char, f);

	return read;
}

//Initilizes a hangman context.
void hangman_init(hangman_context* hm){
	hm->remaining_ = 7;

	hm->answer_ = NULL;
	hm->nihongo_ = NULL;
	hm->disp_ = NULL;

	hm->answer_length_ = 0;

	int i;
	for(i = 0; i < 26; i++){
		hm->alphabet_map_[i] = unused;
	}
}

//Sets a problem from "file_name".
//Returns 0 on success or -1 on failure.
result hangman_set_problem(hangman_context* hm, char const* file_name){
	FILE* f = fopen(file_name, "r");
	if(f == NULL){
		fprintf(stderr, "could not read the file:%s\n", file_name);
		return failed;
	}
	int file_line_count = FILE_count_line(f);

	char* line;
	int i;

	int problem_number = rand() % file_line_count + 1;
	hm->to_free_ = line = FILE_get_line_at(f, problem_number);

	while(*(line++) != ' ');
	hm->answer_ = line;

	while(*(line++) != ' ');
	*(line-1) = '\0';

	hm->nihongo_ = line;

	hm->disp_ = malloc(strlen(hm->nihongo_)+1);
	for(i = 0; i < strlen(hm->answer_); i++){
		hm->disp_[i] = '-';
	}
	hm->disp_[i] = '\0';

	fclose(f);

	return succeeded;
}

//Checks if "alphabet" is valid.
//If "alphabet" is valid, "hm" will be modified.
void hangman_check_alphabet(hangman_context* hm, char const alphabet){
	int i;
	char not_hit = 1;

	if(alphabet < 'a' || alphabet > 'z')
		return;

	if(hm->alphabet_map_[alphabet-'a'] == used)
		return;

	hm->alphabet_map_[alphabet-'a'] = used;

	for(i = 0; hm->answer_[i] != '\0'; i++){
		if(alphabet == hm->answer_[i]){
			not_hit = 0;
			hm->disp_[i] = hm->answer_[i];
		}
	}

	if(not_hit)
		hm->remaining_ -= 1;
}

//Prints used alphabets.
void hangman_print_used(hangman_context const* hm){
	int i;
	for(i = 0; i < 26; i++){
		if(hm->alphabet_map_[i] == used){
			printf("%c", 'a' + i);
		}
	}
}

//Returns 1 if the player solved a problem or 0 the player not solved.
int hangman_is_clear(hangman_context const* hm){
	int i;
	for(i = 0; hm->disp_[i]; i++){
		if(hm->disp_[i] == '-'){
			return 0;
		}
	}
	return 1;
}

//Returns the answer of a current problem.
char const* hangman_get_answer(hangman_context const* hm){
	return hm->answer_;
}

//Returns the meaning in Japanese of an answer.
char const* hangman_get_nihongo(hangman_context const* hm){
	return hm->nihongo_;
}

//Returns a string to be displayed.
char const* hangman_get_disp(hangman_context const* hm){
	return hm->disp_;
}

//Returns a number of remaining.
int hangman_get_remaining(hangman_context const* hm){
	return hm->remaining_;
}

//Deallocate the memory that a hangmane_context used.
void hangman_destroy_context(hangman_context * hm){
	free(hm->to_free_);
	free(hm->disp_);
}

//Returns "success" if the player solved the problem or "failed" the player could not solved the problem.
result play_hangman(hangman_context* hm){
	char input;
	while(hangman_get_remaining(hm) > 0){
		if(hangman_is_clear(hm)){
			printf("\n%s!!!\n\n", hangman_get_answer(hm));
			return succeeded;
		}

		printf("\nremaining: %d\n", hangman_get_remaining(hm));

		printf("used: ");
		hangman_print_used(hm);

		printf("\n%s\n", hangman_get_disp(hm));

		printf("input: ");
		input = getChar();
		printf("\n");

		hangman_check_alphabet(hm, input);
	}

	return failed;
}

//Contorls hangman game.
void ctrl_hangman(char const* file_name){
	hangman_context hm;
	char tudukeru = 'y';
	int success_count = 0;
	int play_count = 0;

	string_list_node* failed_problems;
	string_list_create(&failed_problems);


	//this pointer does not have ownership
	string_list_node* ite = failed_problems;

	while(tudukeru != 'n'){
		play_count++;

		hangman_init(&hm);
		if(hangman_set_problem(&hm, file_name) == failed){
			string_list_destroy(failed_problems);
			return;
		}

		if(play_hangman(&hm) == succeeded){
			success_count++;
		}else{
			ite = string_list_add(ite, hangman_get_answer(&hm));
			ite = string_list_add(ite, hangman_get_nihongo(&hm));
		}

		hangman_destroy_context(&hm);

		printf("continue? input 'n': no, another: yes: ");
		tudukeru = getChar();
	}
	printf("\n\n-------result---------\n");
	printf("seikairistu:%d/%d(%3lf)\n\n", success_count, play_count, (double)success_count / play_count);

	print_failed_problems(failed_problems);
	string_list_destroy(failed_problems);
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
	if(argc == 2)
		ctrl_hangman(argv[1]);
	else
		ctrl_hangman("./toeic1500.dat");
}

