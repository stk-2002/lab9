#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
	struct RecordType *next;
};

// Fill out this structure
struct HashType
{
	int hash;
	struct RecordType *record;
};

// Compute the hash function
int hash(int x)
{
	//using mod 31 gives 15 for every ID
	//23 was specified during lab section
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray[], int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		struct RecordType myRecord = *pHashArray[i]->record;
		if(pHashArray[i] != NULL)
		{
			printf("index %d -> %d %c %d", i, myRecord.id, myRecord.name, myRecord.order);
		}
		//print any collisions stored in each bucket
		while(myRecord.next != NULL)
		{
			myRecord = *myRecord.next;
			printf(" -> %d %c %d", myRecord.id, myRecord.name, myRecord.order);
		}
	}
	printf("\n");
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	struct HashType *table = malloc(sizeof(struct RecordType)*23);
	//struct RecordType myRecord = *pRecords;

	//add each item from the input data
	for(int i=0; i<31; i++)
	{
		//Get the hash, use it as the index
		int hIndex = hash(pRecords[i].id);

		//if the index is empty, add the record
		if(table[hIndex].record == NULL)
		{
			table[hIndex].record = &pRecords[i];
		}
		//if there is a collision, add to linkedlist in bucket
		else
		{
			struct RecordType *ptr = table[hIndex].record;
			while(ptr != NULL)
			{
				//segfaults here not sure why
				ptr = ptr->next;
			}
			ptr->next = &pRecords[i];
		}
	}
	displayRecordsInHash(table, 23);
}