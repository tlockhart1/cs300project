#ifndef __CDA_INCUDED__
	#define __CDA_INCLUDED__

#include <stdio.h>
#include <stdlib.h>

typedef struct cda CDA;

extern CDA *newCDA(int capacity);
extern void setCDAdisplay(CDA *items, void (*d)(void *, FILE *));
extern void setCDAfree(CDA *items, void (*f)(void *));
extern void freeCDA(CDA *items);
extern int debugCDA(CDA *items, int level);
extern void insertCDA(CDA *items, int index, void *value);
extern void *removeCDA(CDA *items, int index);
extern void unionCDA(CDA *recipient, CDA *donor);
extern int sizeCDA(CDA *items);
extern void *getCDA(CDA *items, int index);
extern void *setCDA(CDA *items, int index, void *value);
extern void displayCDA(CDA *items, FILE *);

#define insertCDAfront(items, value) insertCDA(items, 0, value)
#define insertCDAback(items, value) insertCDA(items, sizeCDA(items), value)
#define removeCDAfront(items) removeCDA(items, 0)
#define removeCDAback(items) removeCDA(items, sizeCDA(items)-1)

#endif
