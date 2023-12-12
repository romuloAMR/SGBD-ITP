//--||Includes||--
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--|||Functions|||--

// Made for Rômulo
char *toLower(char *string) {
  for (int i = 0; i < strlen(string); i++) {
    string[i] = tolower(string[i]);
  }
  return string;
}

void removeStructLine(char* name){
  char line[1000];
  FILE* output = fopen("temp.txt", "a");
  FILE* input = fopen("struct.txt", "r");
  char *nameTwo = (char *)malloc(51 * sizeof(char));
  strcpy(nameTwo, name);
  strcat(nameTwo, ":");
  while(fgets(line, sizeof(line), input)){
    if(strstr(line, nameTwo) == NULL){
      printf("%s\n",line);
      fprintf(output, "%s", line);
    }
  }
  fclose(input);
  remove("struct.txt");
  rename("temp.txt", "struct.txt"); 
  fclose(output);
  free(nameTwo);
}

void serverOn() {
  FILE *server;
  server = fopen("struct.txt", "a");
  fclose(server);
}

char *path(char *name) {
  char *path = (char *)malloc(sizeof(char) * 60);
  strcpy(path, "Tables/");
  strcat(path, name);
  strcat(path, ".txt");
  return path;
}

int tableExists(char *name) {
  FILE *table = fopen(path(name), "r");
  if (table == NULL) {
    return 0;
  }
  fclose(table);
  return 1;
}

void makeTable() {
  char *name = (char *)malloc(50 * sizeof(char));
  char trash;
  FILE *table;

  printf("Table name: ");
  scanf("%s%c", name, &trash);
  name = toLower(name);
  if (tableExists(name)) {
    printf("Invalid table!\n");
    return;
  }
  table = fopen(path(name), "a");
  fclose(table);

  table = fopen("struct.txt", "a");
  fprintf(table, "%s: ", name);

  printf("Primary Key name: ");
  scanf("%s%c", name, &trash);
  fprintf(table, "(%s, pk) ", name);

  char type, option;
  printf("\nChoose i for int, c for char, s for string or d for double\n\n");

  while (1) {

    printf("Colum name: ");
    scanf("%s%c", name, &trash);
    printf("Type: ");
    scanf("%c%c", &type, &trash);
    fprintf(table, "(%s, %c) ", name, type);
    printf("New colum [Y/N]: ");
    scanf("%s%c", &option, &trash);
    if (tolower(option) == 'n') {
      fprintf(table, "\n");
      break;
    }
  }

  fclose(table);
}

void showTables() {
  char arqName[] = "struct.txt";
  FILE *arq = fopen(arqName, "r");
  char line[1000];
  int cont = 1;
  printf("\nAll tables:\n");
  while (fgets(line, sizeof(line), arq) != NULL) {
    printf("%d - %s", cont, line);
    cont++;
  }
}

void delTable() {
  char *name = (char *)malloc(50 * sizeof(char));
  char trash;
  FILE *table;
  printf("Table name: ");
  scanf("%s", name);
  name = toLower(name);
  if(tableExists(name)){
    remove(path(name));
    removeStructLine(name);
  } else {
    printf("Invalid table!\n");
  }
}

// Made for Dagson
int verifyPK(char *nameArq, char *data) {
  FILE *table = fopen(nameArq, "r");
  char row[1000];
  char aux[50];
  snprintf(aux, 50, "): %s]", data);

  while (fgets(row, 1000, table) != NULL) {
    if (strstr(row, aux) != NULL) {
      return 1;
    }
  }
    fclose(table);
    return 0;
}

void listColumns(char *name){
  char row[1000];
  char arqName[50];
  snprintf(arqName, sizeof(arqName), "%s", name);

  FILE *tables = fopen("struct.txt", "r");
  strcat(arqName, ":");

  while (fgets(row, 1000, tables) != NULL) {
    if (strstr(row, arqName) != NULL) {
      char *indexTwoPoints = strchr(row, ':');
      if (indexTwoPoints != NULL) {
        *indexTwoPoints = '\0';

        char *initialColumn = indexTwoPoints + 1;
        char *indexParenthesis = strchr(initialColumn, '(');
        int count = 1;

        while (indexParenthesis != NULL) {
          char *indexComma = strchr(indexParenthesis, ',');
          if (indexComma != NULL) {
            *indexComma = '\0';
            char *nameColumn = indexParenthesis + 1;
            printf("%d - %s\n", count++, nameColumn);
            initialColumn = indexComma + 1;
            indexParenthesis = strchr(initialColumn, '(');
          } else {
            break;
          }
        }
      }
    }
  }

  fclose(tables);
}

char *getType(char *nameTable, char *nameCol){
    char row[1000];
    char arqName[50];
    snprintf(arqName, sizeof(arqName), "%s", nameTable);

    FILE *tables = fopen("struct.txt", "r");
    strcat(arqName, ":");

    while (fgets(row, 1000, tables) != NULL) {
      if (strstr(row, arqName) != NULL) {
        char *indexTwoPoints = strchr(row, ':');
        if (indexTwoPoints != NULL) {
          *indexTwoPoints = '\0';

          char *initialColumn = indexTwoPoints + 1;
          char *indexParenthesis = strchr(initialColumn, '(');

          while (indexParenthesis != NULL) {
            char *indexFinalParenthesis = strchr(indexParenthesis, ')');

            if (indexFinalParenthesis != NULL) {
              *indexFinalParenthesis = '\0';
              char *nameColumn = indexParenthesis + 1;

              if(strstr(nameColumn, nameCol) != NULL){
                const char delimiter = ' ';
                char *commaPos = strchr(nameColumn, delimiter);

                if (commaPos != NULL) {
                      commaPos++;
                }
                return commaPos;
              }

              initialColumn = indexFinalParenthesis + 1;
              indexParenthesis = strchr(initialColumn, '(');
            } else {
              break;
            }
          }
        }
      }
    }
  return nameCol;
}

void getEqual(char *name, char *columnName, char *keyword){
  char row[1000];

  FILE *tables = fopen(path(name), "r");

  while (fgets(row, 1000, tables) != NULL) {
    char *begin = row;
    char *end = NULL;

    while ((begin = strchr(begin, '[')) != NULL) {
      begin++;
      end = strchr(begin, ']');

      if (end != NULL) {
        size_t length = end - begin;
        char substring[length + 1];
        strncpy(substring, begin, length);
        substring[length] = '\0';

        if(strstr(substring, columnName) != NULL){
          const char delimiter = ' ';
          char *commaPos = strchr(substring, delimiter);

          if (commaPos != NULL) {
            commaPos++;
          }
          if (strcmp(keyword, commaPos) == 0){
            printf("%s\n", row);
          } else if (strstr(commaPos, keyword) != NULL){
            printf("%s\n", row);
          }
        }
      }
      begin = end + 1;
    }
  }
  fclose(tables);
}

void getGreater(char *name, char *columnName, char *keyword){
  char row[1000];

  FILE *tables = fopen(path(name), "r");

  while (fgets(row, 1000, tables) != NULL) {
    char *begin = row;
    char *end = NULL;

    while ((begin = strchr(begin, '[')) != NULL) {
      begin++;
      end = strchr(begin, ']');

      if (end != NULL) {
        size_t length = end - begin;
        char substring[length + 1];
        strncpy(substring, begin, length);
        substring[length] = '\0';

        if(strstr(substring, columnName) != NULL){
          const char delimiter = ':';
          char *commaPos = strchr(substring, delimiter);

          if (commaPos != NULL) {
            commaPos++;
          }

          if (atoi(commaPos) > atoi(keyword)){
            printf("%s\n", row);
          } 
        }
      }
      begin = end + 1;
    }
  }
  fclose(tables);
}

void getGreaterEqual(char *name, char *columnName, char *keyword){
  char row[1000];

  FILE *tables = fopen(path(name), "r");

  while (fgets(row, 1000, tables) != NULL) {
    char *begin = row;
    char *end = NULL;

    while ((begin = strchr(begin, '[')) != NULL) {
      begin++;
      end = strchr(begin, ']');

      if (end != NULL) {
        size_t length = end - begin;
        char substring[length + 1];
        strncpy(substring, begin, length);
        substring[length] = '\0';

        if(strstr(substring, columnName) != NULL){
          const char delimiter = ':';
          char *commaPos = strchr(substring, delimiter);

          if (commaPos != NULL) {
            commaPos++;
          }

          if (atof(commaPos) >= atof(keyword)){
            printf("%s\n", row);
          }
        }
      }
      begin = end + 1;
    }
  }
  fclose(tables);
}

void getLess(char *name, char *columnName, char *keyword){
  char row[1000];

  FILE *tables = fopen(path(name), "r");

  while (fgets(row, 1000, tables) != NULL) {
    char *begin = row;
    char *end = NULL;

    while ((begin = strchr(begin, '[')) != NULL) {
      begin++;
      end = strchr(begin, ']');

      if (end != NULL) {
        size_t length = end - begin;
        char substring[length + 1];
        strncpy(substring, begin, length);
        substring[length] = '\0';

        if(strstr(substring, columnName) != NULL){
          const char delimiter = ':';
          char *commaPos = strchr(substring, delimiter);

          if (commaPos != NULL) {
            commaPos++;
          }

          if (atof(commaPos) < atof(keyword)){
            printf("%s\n", row);
          }
        }
      }
      begin = end + 1;
    }
  }
  fclose(tables);
}

void getLessEqual(char *name, char *columnName, char *keyword){
  char row[1000];

  FILE *tables = fopen(path(name), "r");

  while (fgets(row, 1000, tables) != NULL) {
    char *begin = row;
    char *end = NULL;

    while ((begin = strchr(begin, '[')) != NULL) {
      begin++;
      end = strchr(begin, ']');

      if (end != NULL) {
        size_t length = end - begin;
        char substring[length + 1];
        strncpy(substring, begin, length);
        substring[length] = '\0';

        if(strstr(substring, columnName) != NULL){
          const char delimiter = ':';
          char *commaPos = strchr(substring, delimiter);

          if (commaPos != NULL) {
            commaPos++;
          }

          if (atof(commaPos) <= atof(keyword)){
            printf("%s\n", row);
          }
        }
      }
      begin = end + 1;
    }
  }
  fclose(tables);
}

void insertData(char *name) {
  char arqName[50];
  snprintf(arqName, sizeof(arqName), "%s", name);
  char row[1000];
  char data[100];

  FILE *arq = fopen(path(arqName), "a");
  FILE *tables = fopen("struct.txt", "r");
  strcat(arqName, ":");

  while (fgets(row, 1000, tables) != NULL) {
    if (strstr(row, arqName) != NULL) {
      char *indexTwoPoints = strchr(row, ':');
      if (indexTwoPoints != NULL) {
        *indexTwoPoints = '\0';

        char *initialColumn = indexTwoPoints + 1;
        char *indexParenthesis = strchr(initialColumn, '(');
        int flag = 0;

        while (indexParenthesis != NULL) {
          char *indexComma = strchr(indexParenthesis, ',');
          if (indexComma != NULL) {
            *indexComma = '\0';
            char *nameColumn = indexParenthesis + 1;
            printf("%s:", nameColumn);
            scanf("%s", data);
            if(verifyPK(path(name), data) && flag == 0){
              printf("Invalid name!\n");
              return;
            } else{
              if(flag == 0){
                fprintf(arq, "[%s (pk): %s] ", nameColumn, data);
                flag = 1;
              } else{
                fprintf(arq, "[%s: %s] ", nameColumn, data);
              }
            }

            initialColumn = indexComma + 1;
            indexParenthesis = strchr(initialColumn, '(');
          } else {
            break;
          }
        }
      fprintf(arq, "\n");
      }
    }
  }

  fclose(tables);
  fclose(arq);
}

void showData(char *name) {
  char arqName[50];
  snprintf(arqName, sizeof(arqName), "%s", path(name));
  FILE *arq = fopen(arqName, "r");
  char line[1000];
  int cont = 1;
  printf("\n%s Table Data:\n", name);
  while (fgets(line, sizeof(line), arq) != NULL) {
    printf("%d - %s", cont, line);
    cont++;
  }
  printf("\n");
}

void searchData(char *name) {
  char keyword[50];
  char columnName[50];

  printf("\nDisponible columns in table %s:\n", name);
  listColumns(name);
  printf("Enter the column: ");
  scanf("%s", columnName);

  char *type = getType(name, columnName);
  printf("==%s==", type);

  printf("\nEnter what you search: ");
  scanf("%s", keyword);

  printf("\n1 - Values greater than the value reported\n");
  printf("2 - Values greater than or equal to the value informed\n");
  printf("3 - Values equal to the value informed\n");
  printf("4 - Values less than the value informed\n");
  printf("5 - Values less than or equal to the value informed\n");
  printf("6 - Values close to the reported value (only if type = string)\n\n");

  int searchType;
  printf("Choose the type of search:");
  scanf("%d", &searchType);
  switch(searchType){

    case 1: 
      if (strcmp(type, "i") == 0 || strcmp(type, "pk") == 0 || strcmp(type, "d") == 0){
        getGreater(name, columnName, keyword);
      } else if (strcmp(type, "c") == 0 || strcmp(type, "s") == 0){
        printf("%s must have type int or double!", columnName);
      }
      break;
    case 2:
      if (strcmp(type, "i") == 0 || strcmp(type, "pk") == 0 || strcmp(type, "d") == 0){
        getGreaterEqual(name, columnName, keyword);
      } else if (strcmp(type, "c") == 0 || strcmp(type, "s") == 0){
        printf("%s must have type int or double!", columnName);
      }
      break;
    case 3:
      getEqual(name, columnName, keyword);
      break;
    case 4:
      if (strcmp(type, "i") == 0 || strcmp(type, "pk") == 0 || strcmp(type, "d") == 0){
        getLess(name, columnName, keyword);
      } else if (strcmp(type, "c") == 0 || strcmp(type, "s") == 0){
        printf("%s must have type int or double!", columnName);
      }
      break;
    case 5:
      if (strcmp(type, "i") == 0 || strcmp(type, "pk") == 0 || strcmp(type, "d") == 0){
        getLessEqual(name, columnName, keyword);
      } else if (strcmp(type, "c") == 0 || strcmp(type, "s") == 0){
        printf("%s must have type int or double!", columnName);
      }
      break;
    case 6: ;
      break;
  }

}

void delData(char *name) {
  char pk[100];
  printf("\nEnter the Primary Key: ");
  scanf("%s", pk);
  if(verifyPK(path(name), pk)){
    FILE *table = fopen(path(name), "r");
    FILE *tempTable = fopen("Tables/tempTable.txt", "w");

    char row[1000];
    char aux[50] = "): ";
    strcat(aux, strcat(pk, "]"));

    while (fgets(row, sizeof(row), table) != NULL) {
      if (strstr(row, aux) == NULL) {
        fputs(row, tempTable);
      }
    }
    fclose(table);
    remove(path(name));
    rename("Tables/tempTable.txt", path(name));
    fclose(tempTable);

  } else {
    printf("\nInvalid Primary Key!\n");
    return;
  }

}