#include "cda.h"

/************** PRIVATE FUNCTION DECS *****************/
static int decrementINDEX(CDA *items, int index);
static int incrementINDEX(CDA *items, int index);
static int correctINDEX(CDA *items, int index);
static int getCAPACITY(CDA *items);
static void changeCDAcapacity(CDA *items, int how);
static int checkCDAfullcapacity(CDA *items);
static int checkCDAshrinkcapacity(CDA *items);
/******************************************************/

struct cda{
	void **c_list;
	int debug_flag;
	int capacity;
	int size;
	int startIndex;		//first leftmost index
	int endIndex;		//index of first open slot
	void (*display)(void *, FILE *);
	void (*free)(void *);
};

CDA *newCDA(int capacity){ //if not 0, user must check for discontinuity when accessing elements
	CDA *newCDA = (CDA*) malloc(sizeof(CDA));
	if(capacity == 0)
		newCDA->capacity = 1;
	else newCDA->capacity = capacity;
	newCDA->debug_flag = 0;
	newCDA->size = newCDA->capacity - 1;
	newCDA->startIndex = 0;
	newCDA->endIndex = newCDA->capacity - 1;
	newCDA->display = NULL;
	newCDA->free = NULL;
	newCDA->c_list = (void*) malloc(sizeof(void*)*newCDA->capacity);
	return newCDA;
}

void setCDAdisplay(CDA *items, void (*d)(void *, FILE *)){
	items->display = d;		
}

void setCDAfree(CDA *items, void (*f)(void *)){
	items->free = f;
}

void freeCDA(CDA *items){
	if(items->free != NULL){
		for (int i = 0; i < sizeCDA(items); i++) items->free(getCDA(items, i));
	}
	free(items->c_list);
	free(items);
}

int debugCDA(CDA *items, int level){
	int prev_flag = items->debug_flag;
	items->debug_flag = level;
	return prev_flag;
}

void insertCDA(CDA *items, int index, void *value){
	if (checkCDAfullcapacity(items) == 1) changeCDAcapacity(items, 1);
	if((index == 0 || index == -1) && sizeCDA(items) != 0){
		items->startIndex = decrementINDEX(items, items->startIndex);
		items->c_list[items->startIndex] = value;		
		items->size++;
	}
	else if(index == sizeCDA(items)){
		items->c_list[items->endIndex] = value;
		items->endIndex = incrementINDEX(items, items->endIndex);
		items->size++;
	}
	else{
		if(index - 0 > (sizeCDA(items)-1) - index){ 
			items->endIndex = incrementINDEX(items, items->endIndex);
			items->size++;
			for (int i = sizeCDA(items)-1; i > index; i--){
				items->c_list[correctINDEX(items, i + items->startIndex)] = items->c_list[correctINDEX(items,(i-1) + items->startIndex)];
			}	
		}
		else{
			items->startIndex = decrementINDEX(items, items->startIndex);
			items->size++;
			for (int i = 0; i < index; i++){
				items->c_list[correctINDEX(items, i + items->startIndex)] = items->c_list[correctINDEX(items, (i+1) + items->startIndex)];
			}	
		}	
		items->c_list[correctINDEX(items, index + items->startIndex)] = value;
	}
}

void *removeCDA(CDA *items, int index){
	void *p = getCDA(items, index);
	if((index == 0 || index == -1)){
		items->startIndex = incrementINDEX(items, items->startIndex);
		items->size--;
	}
	else if(index == sizeCDA(items)-1){
		items->endIndex = decrementINDEX(items, items->endIndex);
		items->size--;
	}
	else{
		if(index - 0 > (sizeCDA(items)-1) - index){ 
			items->endIndex = decrementINDEX(items, items->endIndex);
			items->size--;
			for (int i = index; i < sizeCDA(items); i++){
				items->c_list[correctINDEX(items, i + items->startIndex)] = items->c_list[correctINDEX(items, (i+1) + items->startIndex)];
			}	
		}
		else{
			items->size--;
			for (int i = index; i > 0; i--){
				items->c_list[correctINDEX(items, i + items->startIndex)] = items->c_list[correctINDEX(items,(i-1) + items->startIndex)];
			}	
			items->startIndex = incrementINDEX(items, items->startIndex);
		}	

	}
	if (checkCDAshrinkcapacity(items)) changeCDAcapacity(items, 0);
	return p;
}

void unionCDA(CDA *recipient, CDA *donor){
	int size = sizeCDA(donor);
	for (int i = 0; i < size; i++) insertCDAback(recipient, removeCDAfront(donor));
	free(donor->c_list);
	donor->capacity = 1;
	donor->size = 0;
	donor->startIndex = 0;
	donor->endIndex = 0;
	donor->c_list = malloc(sizeof(void*)*donor->capacity);
}
 
int sizeCDA(CDA *items){
	return items->size;
}

void *getCDA(CDA *items, int index){
	return items->c_list[correctINDEX(items, index + items->startIndex)];
}

void *setCDA(CDA *items, int index, void *value){
	if(index == sizeCDA(items)){
		insertCDAback(items, value);
		return NULL;
	}
	if(index == -1){
		insertCDAfront(items, value);
		return NULL;
	}
	void *p = getCDA(items, index);
	items->c_list[correctINDEX(items, index + items->startIndex)] = value;
	return p;
}

void displayCDA(CDA *items, FILE *where){
	if (sizeCDA(items) == 0){
		if(items->debug_flag > 0) fprintf(where,"((%d))", items->capacity - items->size);
		else fprintf(where,"()");
	}
	else{
		fprintf(where,"(");
		for (int i = 0; i < sizeCDA(items); i++){
			items->display(getCDA(items, i), where);
			if(i != sizeCDA(items)-1) fprintf(where,",");
		}
		if(items->debug_flag > 0) fprintf(where,",(%d)", items->capacity - sizeCDA(items));
		fprintf(where,")");

	}
}

int decrementINDEX(CDA *items, int index){
	return correctINDEX(items, index - 1);	
}

int incrementINDEX(CDA *items, int index){
	return correctINDEX(items, index + 1);
}

int correctINDEX(CDA *items, int index){
	return (index + getCAPACITY(items)) % getCAPACITY(items);
}

int getCAPACITY(CDA *items){
	return items->capacity;
}

void changeCDAcapacity(CDA *items, int how){
	void **newlist;
	if(how == 1) newlist = malloc(sizeof(void*)*(items->capacity*2));
	else{
		if(sizeCDA(items) == 0) newlist = malloc(sizeof(void*)*1);
		else newlist = malloc(sizeof(void*)*(items->capacity/2));
	}
	for (int i = 0; i < sizeCDA(items); i++){
		newlist[i] = items->c_list[items->startIndex];
		items->startIndex = incrementINDEX(items, items->startIndex);
		
	}
	free(items->c_list);
	items->c_list = newlist;
	if(how == 1) items->capacity = items->capacity*2;
	else{
		if(sizeCDA(items) == 0) items->capacity = 1;
		else items->capacity = items->capacity/2;
	}
	items->startIndex = 0;
	items->endIndex = sizeCDA(items);
}

int checkCDAfullcapacity(CDA *items){
	if(sizeCDA(items) == getCAPACITY(items)) return 1;
	else return 0;
}

int checkCDAshrinkcapacity(CDA *items){
	if(sizeCDA(items) < items->capacity/4) return 1;
	else return 0;
}
