#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "hashTable.h"
int isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}
int isValidIpAddress(const char* str) {
    int num, dots = 0;
    char* ptr;

    if (str == NULL || strlen(str) < 7 || strlen(str) > 15)
        return 0;

    char* copy = _strdup(str);
    ptr = strtok(copy, ".");

    if (ptr == NULL)
        return 0;

    while (ptr) {
        if (!isDigit(*ptr))
            return 0;

        num = atoi(ptr);

        if (num < 0 || num > 255)
            return 0;

        dots++;
        ptr = strtok(NULL, ".");

        if (dots > 3 && ptr != NULL)
            return 0;
    }

    free(copy);

    if (dots < 3)
        return 0;

    return 1;
}
unsigned long hash(char* str) {
    unsigned int hash = 5381;
    char c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}
void prioritize(struct Queue* queque, struct QueueNode* node) {
    if (queque->head == node)
        return;

    QueueNode* temp = queque->head;

    queque->head = node;
    node->prev->next = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;

    if (queque->tail == node)
        queque->tail = node->prev;

    node->next = temp;
    node->prev = NULL;

    temp->prev = node;
}
void addElement(struct Queue* queque, Domain* domain) {
    QueueNode* node = malloc(sizeof(QueueNode));
    node->domain = domain;
    node->next = NULL;
    node->prev = NULL;
    if (queque->head == NULL) {
        queque->head = node;
        queque->tail = node;
        return;
    }
    node->next = queque->head;
    queque->head = node;
    queque->head->next->prev = queque->head;
}

void removeTail(struct Queue* queque) {
    QueueNode* temp = queque->tail;
    if (queque->head == queque->tail) {
        queque->head = NULL;
        queque->tail = NULL;
    }
    queque->tail->prev->next = NULL;
    queque->tail = queque->tail->prev;
    free(temp->domain->domain);
    free(temp->domain->ip);
    free(temp->domain);
    free(temp);
}

void clearQueque(QueueNode* head) {
    QueueNode* current = head;
    while (current != NULL) {
        QueueNode* temp = current;
        current = current->next;
        free(temp->domain->domain);
        free(temp->domain->ip);
        free(temp->domain);
        free(temp);
    }
}

struct HastTable* createTable(int capacity) {
    struct HastTable* cache = malloc(sizeof(struct HastTable));
    cache->table = malloc(sizeof(struct Node*) * capacity);
    for (int i = 0;i < capacity;i++) {
        cache->table[i] = NULL;
    }
    return cache;
}
struct Node* createNode(int hash, QueueNode* quequeNode) {
    struct Node* node = malloc(sizeof(struct Node));
    node->hash = hash;
    node->queueNode = quequeNode;
    node->next = NULL;
    return node;
}
struct Cache* createCash(int capacity) {
    Cache* cache = calloc(1, sizeof(Cache));
    HastTable* hastTable = createTable(capacity);
    cache->queue = calloc(1, sizeof(Queue));
    cache->capacity = capacity;
    cache->hastTable = hastTable;
    return cache;
}
void addValue(char* key, QueueNode* quequeNode, struct Cache* cache) {
    unsigned long hashCode = hash(key);
    int index = hashCode % cache->capacity;
    if (cache->hastTable->table[index] == NULL) {
        struct Node* node = createNode(hashCode, quequeNode);
        cache->hastTable->table[index] = node;
    }
    else {
        struct Node* node = cache->hastTable->table[index];
        while (node->next != NULL) {
            node = node->next;
        }
        struct Node* newNode = createNode(hashCode, quequeNode);
        node->next = newNode;
    }
}
QueueNode* findValue(char* key, Cache* cache) {
    unsigned long hashCode = hash(key);
    int index = hashCode % cache->capacity;
    if (cache->hastTable->table[index] == NULL) {
        return NULL;
    }
    else {
        struct Node* node = cache->hastTable->table[index];
        if (node->hash == hashCode) {
            prioritize(cache->queue, node->queueNode);
            return node->queueNode;
        }
        while (node->next != NULL) {
            if (node->hash == hashCode) {
                if (strcmp(node->queueNode->domain->domain, key) == 0) {
                    prioritize(cache->queue, node->queueNode);
                    return node->queueNode;
                }
            }
            node = node->next;
        }
        if (node->hash == hashCode) {
            if (strcmp(node->queueNode->domain->domain, key) == 0) {
                prioritize(cache->queue, node->queueNode);
                return node->queueNode;
            }
        }
    }
    return NULL;
}
void deleteFromHashTable(char* key, struct Cache* cache) {
    int index = hash(key) % cache->capacity;

    struct Node* current = cache->hastTable->table[index];
    struct Node* previous = NULL;


    while (current != NULL && current->hash != hash(key)) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Element with key %d not found in hash table.\n", key);
        return;
    }

    if (previous == NULL) {
        cache->hastTable->table[index] = current->next;
    }
    else {
        previous->next = current->next;
    }

    free(current);
}
void addValueInCache(char* key, char* value, Cache* cache) {
    if (cache->size == cache->capacity) {
        deleteFromHashTable(cache->queue->tail->domain->domain, cache);
        removeTail(cache->queue);
        cache->size--;
    }
    Domain* domain = calloc(1, sizeof(Domain));
    char* domainString = malloc(sizeof(char) * (strlen(key) + 1));
    char* keyString = malloc(sizeof(char) * (strlen(value) + 1));
    domain->domain = domainString;
    domain->ip = keyString;
    strcpy(domain->domain, key);
    strcpy(domain->ip, value);
    addElement(cache->queue, domain);
    addValue(key, cache->queue->head, cache);
    cache->size++;
}
void printfCache(Cache* cache)
{
    QueueNode* temp = cache->queue->head;

    while (temp != NULL)
    {
        printf("%s", temp->domain->domain);

        if (temp->next != NULL)
            printf(" -> ");

        temp = temp->next;
    }
    printf("\n");
}
int isDomenInFile(char* key) {
    FILE* file = fopen("C:\\Users\\botme\\hashTable\\dns.txt", "r");
    char** temp = malloc(sizeof(char*) * 4);
    for (int i = 0; i < 4; i++) {
        temp[i] = malloc(sizeof(char) * 1024);
    }
    while (1) {
        fscanf(file, "%s", temp[0]);
        if (strcmp(temp[0], "$") == 0) break;
        for (int i = 0; i < 3; i++) {
            fscanf(file, "%s", temp[i + 1]);
        }
        if (strcmp(temp[0], key) == 0) return 1;
    }
    return 0;
}
char findValueInFileAndWriteToStack(char* key, Cache* cache, char* startKey) {
    FILE* file = fopen("C:\\Users\\botme\\hashTable\\dns.txt", "r");
    char** temp = malloc(sizeof(char*) * 4);

    while (1) {
        for (int i = 0;i < 4;i++) {
            temp[i] = malloc(sizeof(char) * 1024);
        }
        fscanf(file, "%s", temp[0]);
        if (strcmp(temp[0], "$") == 0) break;
        for (int i = 0;i < 3;i++) {
            fscanf(file, "%s", temp[i + 1]);
        }
        if (strcmp(temp[0], key) == 0) {
            if (strcmp(temp[2], "A") == 0) {
                addValueInCache(startKey, temp[3], cache);
                return 1;
            }
            else if (strcmp(temp[2], "CNAME") == 0) {
                fclose(file);
                if (findValueInFileAndWriteToStack(temp[3], cache, startKey)) return 1;
            }
            else {
                for (int i = 0;i < 4;i++) {
                    free(temp[i]);
                }
            }

        }
    }
    if (file != NULL) fclose(file);
    return 0;
}
void readValueFromFile(Cache* cache) {
    FILE* file = fopen("C:\\Users\\botme\\hashTable\\dns.txt", "r");
    char** temp = malloc(sizeof(char*) * 4);
    while (cache->size != cache->capacity) {
        for (int i = 0;i < 4;i++) {
            temp[i] = malloc(sizeof(char) * 1024);
        }
        fscanf(file, "%s", temp[0]);
        if (strcmp(temp[0], "$") == 0) break;
        for (int i = 0;i < 3;i++) {
            fscanf(file, "%s", temp[i + 1]);
        }
        if (strcmp(temp[2], "A") == 0) addValueInCache(temp[0], temp[3], cache);
        else if (strcmp(temp[2], "CNAME") == 0) {
            long pos = ftell(file);
            fclose(file);
            findValueInFileAndWriteToStack(temp[3], cache, temp[0]);
            file = fopen("C:\\Users\\botme\\hashTable\\dns.txt", "r");
            fseek(file, pos, SEEK_CUR);
        }
        else {
            for (int i = 0;i < 4;i++) {
                free(temp[i]);
            }
        }

    }
    if (file != NULL) fclose(file);
}
int writeValueInFile(int type, char* value, char* key, Cache* cache) {
    FILE* file = fopen("C:\\Users\\botme\\hashTable\\dns.txt", "r");
    char** temp = malloc(sizeof(char*) * 4);
    int bool = 1;
    while (1) {
        for (int i = 0;i < 4;i++) {
            temp[i] = malloc(sizeof(char) * 1024);
        }
        fscanf(file, "%s", temp[0]);
        if (strcmp(temp[0], "$") == 0) break;
        for (int i = 0;i < 3;i++) {
            fscanf(file, "%s", temp[i + 1]);
        }
        if (strcmp(temp[0], key) != 0 && strcmp(temp[3], key) != 0) {

        }
        else {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    file = fopen("C:\\Users\\botme\\hashTable\\dns.txt", "r+");
    FILE* fileLinks = fopen("C:\\Users\\botme\\hashTable\\dnslinks.txt", "r+");
    if (bool) {
        if (type == 2) {
            fseek(file, -2, SEEK_END);
            fseek(fileLinks, -2, SEEK_END);
            if (isDomenInFile(value)) {
                fprintf(file, "\n%s\tIN CNAME %s\n$", key, value);
                findValueInFileAndWriteToStack(value, cache, value);
                fprintf(fileLinks, "%s\tIN A %s\n$", key, cache->queue->head->domain->ip);
                fclose(fileLinks);
            }
            else {
                printf("error\n");
                for (int i = 0;i < 4;i++) {
                    free(temp[i]);
                }
                return 0;
            }
            fclose(file);
            fclose(fileLinks);
            return 1;
        }
        if (type == 1) {
            if (isValidIpAddress(value)) {
                fseek(file, -2, SEEK_END);
                fseek(fileLinks, -2, SEEK_END);
                fprintf(file, "%s\tIN A %s\n$", key, value);
                addValueInCache(key, value, cache);
                fprintf(fileLinks, "%s\tIN A %s\n$", cache->queue->head->domain->domain, cache->queue->head->domain->ip);
                fclose(fileLinks);
                fclose(file);
                return 1;
            }
            else {
                for (int i = 0;i < 4;i++) {
                    free(temp[i]);
                }
                printf("valid ip\n");
                return 0;
            }

        }
        else {
            for (int i = 0;i < 4;i++) {
                free(temp[i]);
            }
            printf("valid type\n");
            return 0;
        }
        return 1;
    }
}
void findAllIp(char* key) {
    FILE* file = fopen("C:\\Users\\botme\\hashTable\\dnslinks.txt", "r");
    char** temp = malloc(sizeof(char*) * 4);
    while (1) {
        for (int i = 0;i < 4;i++) {
            temp[i] = malloc(sizeof(char) * 1024);
        }
        fscanf(file, "%s", temp[0]);
        if (strcmp(temp[0], "$") == 0) break;
        for (int i = 0;i < 3;i++) {
            fscanf(file, "%s", temp[i + 1]);
        }
        if (strcmp(temp[3], key) == 0) {
            printf("%s\n", temp[0]);
        }
        for (int i = 0;i < 4;i++) {
            free(temp[i]);
        }
    }
}

void clearTable(HastTable* table, size_t size) {
    for (size_t i = 0; i < size; i++) {
        Node* current = table->table[i];
        Node* next;
        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
    }
    free(table->table);
}
float SetArraySize(float a) {
    while ((scanf("%f", &a)) != 1 || getchar() != '\n') {
        printf("Error\n");
        rewind(stdin);
    }
    if (a <= 0) {
        printf("Error\n");
        rewind(stdin);
        a = SetArraySize(a);
    }
    else return a;
}
