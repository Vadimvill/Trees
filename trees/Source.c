#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "hashTable.h"

int main() {
    printf("set capacity of cache\n");
    int a = SetArraySize(0);
    Cache* cache = createCash(a);
    // readValueFromFile(cache);
    printfCache(cache);
    int type = -1;
    while (1) {
        char* string = malloc(sizeof(char) * 1024);
        char* value = malloc(sizeof(char) * 1024);
        printf("set domain or ip or 3 to exit\n");
        scanf("%s", string);
        if (strcmp(string, "3") == 0) {
            clearQueque(cache->queue->head);
            clearTable(cache->hastTable, cache->size);
            free(string);
            free(value);
            break;
        }
        if (isValidIpAddress(string)) {
            findAllIp(string);
            free(string);
            free(value);
            continue;
        }
        QueueNode* node = findValue(string, cache);
        if (node == NULL) {
            if (findValueInFileAndWriteToStack(string, cache, string) == 1) {
                node = cache->queue->head;
                free(string);
                free(value);
            }
            else {
                printf("domen not found, set ip\n");
                scanf("%s", value);
                printf("set type\n");
                scanf("%d", &type);
                writeValueInFile(type, value, string, cache);
                rewind(stdin);
                free(string);
                free(value);
                continue;
            }
        }
        printf("%s\n", node->domain->ip);
        printfCache(cache);
        rewind(stdin);
    }
    return 0;
}
