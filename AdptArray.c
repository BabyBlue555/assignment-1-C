#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"
#include "book.h"
#include "Person.h"


typedef struct AdptArray_
{
	int ArrSize;
	PElement* pElemArr;
	DEL_FUNC delFunc;
	COPY_FUNC copyFunc;
    PRINT_FUNC printFunc;

}AdptArray;


// pointer size - always 8 bytes
// returns pointer to the data base
PAdptArray CreateAdptArray(COPY_FUNC copyFunc_, DEL_FUNC delFunc_, PRINT_FUNC printFunc_ )
{
	PAdptArray pArr = (PAdptArray)malloc(sizeof(AdptArray));
	if (pArr == NULL) // memory check
		return NULL;
	pArr->ArrSize = 0;
	pArr->pElemArr = NULL;
	pArr->delFunc = delFunc_;
	pArr->copyFunc = copyFunc_;
	pArr->printFunc= printFunc_;
	return pArr;
}

void DeleteAdptArray(PAdptArray pArr)
{
	int i;
    // make sure we are not passing a NULL to delFunc
	if (pArr == NULL)
		return;
	for(i = 0; i < pArr->ArrSize; ++i)
	{
		pArr->delFunc((pArr->pElemArr)[i]);
	}
	free(pArr->pElemArr);
	free(pArr);
}

// gets index, pointer to element and pointer to the  array
// and saves a copy of the element in the specified index
// frees the old element if exists
Result SetAdptArrayAt(PAdptArray pArr, int idx, PElement pNewElem)
{
	PElement* newpElemArr;
	if (pArr == NULL)
		return FAIL;

    //  Extend Array
	if (idx >= pArr->ArrSize)
	{
		// calloc -allocates the requested memory and returns a pointer to it
		// check the allocation 
		if ((newpElemArr = (PElement*)calloc((idx + 1), sizeof(PElement))) == NULL)
			return FAIL;
		// Init new array and copy old array to new array
		memcpy(newpElemArr, pArr->pElemArr, (pArr->ArrSize) * sizeof(PElement));
		// Free old array and save new array
		free(pArr->pElemArr);
		pArr->pElemArr = newpElemArr;

    }
        // Delete Previous Elem and Set New Elem
        pArr->delFunc((pArr->pElemArr)[idx]);
        (pArr->pElemArr)[idx] = pArr->copyFunc(pNewElem);

        // Update Array Size, if the idx is bigger that the array size
        pArr->ArrSize = (idx >= pArr->ArrSize) ? (idx + 1) : pArr->ArrSize;
        return SUCCESS;
	

}

//PELEMENT GetDB(PDATA_BASE pDB, PKEY pKey)
//{
    // get index of element in DB
//int i = InternalFindDB(pDB, pKey); 
// if index is valid, returns the element
//return (i >= 0) ? pDB->ArrElements[i] : NULL;
//}


// gets index and returns a copy of the element in this index
//pElemKey = pDB->GetKeyFunc(pDB->ArrElements[i]);
//if (pDB->CompareKeysFunc(pKey, pElemKey))
//return i;

PElement GetAdptArrayAt(PAdptArray pArr, int idx){

	if(NULL == (pArr->pElemArr)[idx])
		idx=-1;
	return (idx>=0) ? (pArr->copyFunc(pArr->pElemArr[idx])) : NULL;

}

// returns the size of the array , and -1 if the array wasn't created successfully
int GetAdptArraySize(PAdptArray pArr){
	if(pArr->ArrSize){
		return pArr->ArrSize;
	}
	else{
		return -1;
	}
}

// prints the elements in the array
void PrintDB(PAdptArray pArr){
    int i;
    printf("the database contains the following:\n");
    for(i=0;i<GetAdptArraySize(pArr);i++){
       // make sure we are not passing NULL to printFunc
        if((pArr->pElemArr)[i]){
            pArr->printFunc((pArr->pElemArr)[i]);
        }
    }
}