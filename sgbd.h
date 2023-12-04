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
  printf("\nChose i for int, c for char, s for string or d for double\n\n");

  while (1) {

    printf("Colum name: ");
    scanf("%s%c", name, &trash);
    printf("Type: ");
    scanf("%c%c", &type, &trash);
    fprintf(table, "(%s, %c) ", name, type);

    printf("New colum [Y/N]: ");
    scanf("%s%c", &option, &trash);
    if (option == 'N') {
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
  // To make
  // 1-usuário deverá fornecer o nome da tabela a ser apagada
}

// Made for Dagson
void insertData() {
  // To make
  /*
    1-Usuário deve informar o nome da tabela
    2-sistema deve solicitar os valores de cada uma das colunas
    3-sistema deve verificar a chave primária
    4-Em uma tabela deve existir um e apenas um valor de chave primária. Se o
    usuário        informar uma chave que já existe, sistema deve emitir uma
    mensagem de erro e não       deve inserir o registro
  */
}

void showData() {
  // To make
  /*
    1-Usuário deve informar qual a tabela para serem listados os dados
    2-os dados deverão ser obtidos a partir do arquivo que armazena o conteúdo
    da            tabela;
  */
}

void searchData() {
  // To make
  /*
    1-Usuário deverá informar o nome da tabela onde realizará a pesquisa
    2-Sistema deverá fornecer as colunas disponíveis na tabela o usuário deverá
    selecionar uma delas 3-Sistema deverá solicitar o valor para pesquisar,
    disponibilizando algumas opções valores maior que o valor informado valores
    maior ou igual que o valor informado valores igual o valor informado valores
    menor que o valor informado valores menor ou igual que o valor informado
        valores próximo ao valor informado
            se aplica apenas se a coluna for do tipo string

  */
}

void delData() {
  // To make
  // 1-Usuário deve informar o nome da tabela e a chave primária da tupla a ser
  // apagada
}