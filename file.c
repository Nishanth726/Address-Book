#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) {
FILE *fp;
fp = fopen("add.csv", "w");
if(fp == NULL)
{
    perror("fopen");
    return;
}
fprintf(fp, "%d\n",addressBook -> contactCount);

for(int i = 0;i < addressBook -> contactCount; i++)
{
    fprintf(fp, "%s,", addressBook -> contacts[i].name);
    fprintf(fp, "%s,", addressBook -> contacts[i].phone);
    fprintf(fp, "%s\n", addressBook -> contacts[i].email);
}
fclose(fp);
}

void loadContactsFromFile(AddressBook *addressBook) {
FILE *fc;
fc = fopen("add.csv", "r");
if(fc == NULL)
{
  perror("fopen");
  return;
}
fscanf(fc, "%d\n", &addressBook -> contactCount);
for(int i = 0;i < addressBook -> contactCount; i++)
{
    fscanf(fc, "%[^,],", addressBook -> contacts[i].name);
    fscanf(fc, "%[^,],", addressBook -> contacts[i].phone);
    fscanf(fc, "%[^\n]\n", addressBook -> contacts[i].email);
}
fclose(fc);
}
