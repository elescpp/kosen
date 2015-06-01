#include <stdlib.h>


#define lnew(type) (type*)malloc(sizeof(type))

typedef struct list_node__type{
	type obj_;
	struct list_node__type* next_;
	struct list_node__type* prev_;
}list_node__type;

typedef struct list__type{
	size_t object_count_;
	struct list_node__type* head_;
	struct list_node__type* end_;
}list__type;

enum direction{
	from_head_to_end,
	from_end_to_head,
};

typedef struct list_ite__type{
	direction dir_;
	list_node__type* node_;

//public
	type* obj;
};

list_ite__type list_ite_get(list__type* list, direction dir){
	list_ite__type temp;
	temp.dir_ = dir;
	if(dir == from_head_to_end){
		temp.node_ = list->head_->next_;
	}
	if(dir == from_end_to_head){
		temp.node_ = list->end_->prev_;
	}
	temp.obj = (temp.node)->obj_;

	return temp;
}

int list_ite_advance(list_ite__type* ite){
	if(ite->dir_ == from_head_to_end){
		ite->node_->next_ == NULL;
	}
}

void list_init__type(list__type** tar){
	(*tar) = lnew(list__type);
	(*tar)->object_count_ = 0;

	(*tar)->head_ = lnew(list_node__type);
	(*tar)->end_ = lnew(list_node__type);

	(*tar)->head_->next_ = (*tar)->end_;
	(*tar)->head_->prev_ = NULL;

	(*tar)->end_->next_ = NULL;
	(*tar)->end_->prev_ = (*tar)->head_;
}

void list_push_back__type(list__type* list, const type obj){
	list_node__type* temp = lnew(list_node__type);
	temp->obj_ = obj;
	temp->next_ = list->end_;
	temp->prev_ = list->end_->prev_;

	list->end_->prev_->next_ = temp;
	list->end_->prev_ = temp;
}

void list_push_front__type(list__type* list, const type obj){
	list_node__type* temp = lnew(list_node__type);
	temp->obj_ = obj;
	temp->next_ = list->head_->next_;
	temp->prev_ = list->head_;

	list->head_->next_->prev_ = temp;
	list->head_->next_ = temp;
}

void list_foreach__type(list__type* list, void (*visitor)(type*))
{
	list_node__type* ite = list->head_->next_;
	while(ite != list->end_){
		visitor(&(ite->obj_));
		ite = ite->next_;
	}
}

