#include <stdio.h>
#include "stdlib.h"
#include "string.h"

typedef struct Domain {
    char* domain;
    char* ip;
}Domain;
typedef struct QueueNode {
    struct Domain* domain;
    struct QueueNode* next;
    struct QueueNode* prev;
}QueueNode;
typedef struct Queue {
    struct QueueNode* head;
    struct QueueNode* tail;
}Queue;
typedef struct Node {
    unsigned long hash;
    QueueNode* queueNode;
    struct Node* next;
}
Node;
typedef struct HastTable {
    struct Node** table;
} HastTable;
typedef struct Cache {
    HastTable* hastTable;
    int capacity;
    int size;
    Queue* queue;
}Cache;
int SetArraySize();
int isDigit(char ch);
int isValidIpAddress(const char* str);
unsigned long hash(const char* str);
void prioritize(struct Queue* queque, struct QueueNode* node);
void addElement(struct Queue* queque, Domain* domain);
void removeTail(struct Queue* queque);
void clearQueque(QueueNode* head);
struct HastTable* createTable(int capacity);
struct Node* createNode(int hash, QueueNode* quequeNode);
struct Cache* createCash(int capacity);
void addValue(char* key, QueueNode* quequeNode, struct Cache* cache);
char handleFoundKey(char* key, char* type, char* value, char* startKey, Cache* cache);
QueueNode* findValue(char* key, Cache* cache);
void deleteFromHashTable(char* key, struct Cache* cache);
void addValueInCache(char* key,const char* value, Cache* cache);
void printfCache(Cache* cache);
int isDomenInFile(char* key);
char findValueInFileAndWriteToStack(char* key, Cache* cache, char* startKey);
void readValueFromFile(Cache* cache);
int writeValueInFile(int type, char* value, char* key, Cache* cache);
void findAllIp(char* key);
void clearTable(HastTable* table, size_t size);

