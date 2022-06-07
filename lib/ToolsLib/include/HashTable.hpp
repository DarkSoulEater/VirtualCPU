#ifndef TOOLSLIB_SRC_HASH_TABLE_H
#define TOOLSLIB_SRC_HASH_TABLE_H

struct HashTable;

HashTable *TableAllocate(char*(*Strdup)(const char*) = nullptr);

void *TableLookup(HashTable* table, const char *key);

void **TableGetDataPtr(HashTable* table, const char *key);

void TableInsert(HashTable* table, const char *key, void *value);

void TableDeallocate(HashTable **table);

#endif //TOOLSLIB_SRC_HASH_TABLE_H