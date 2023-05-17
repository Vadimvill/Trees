#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "hashTable.h"

int main() {
    printf("set capacity of cache\n");
    int a = SetArraySize();
    Cache* cache = createCash(a);
    printfCache(cache);
    int type = -1;
    int j = 0;
    char* string;
    char* value;
    while (1) {
        if (j != 0) {
            free(value);
            free(string);
        }
        j++;
        string = malloc(sizeof(char) * 1024);
        value = malloc(sizeof(char) * 1024);
        printf("set domain or ip or 3 to exit\n");
        scanf("%s", string);
        if (strcmp(string, "3") == 0) {
            clearQueque(cache->queue->head);
            clearTable(cache->hastTable, cache->size);
            break;
        }
        if (isValidIpAddress(string)) {
            findAllIp(string);
            continue;
        }
        QueueNode* node = findValue(string, cache);
        if (node == NULL) {
            if (findValueInFileAndWriteToStack(string, cache, string) == 1) {
                node = cache->queue->head;
            }
            else {
                printf("domen not found, set ip\n");
                scanf("%s", value);
                printf("set type\n");
                scanf("%d", &type);
                writeValueInFile(type, value, string, cache);
                rewind(stdin);
                continue;
            }
        }
        printf("%s\n", node->domain->ip);
        printfCache(cache);
        rewind(stdin);
    }
    free(value);
    free(string);
    return 0;
}