#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000
#define MIL 1000000

int length = 0;

//-------MEMORY STRING---------//
char* makeMemory()
{
   char *tabel = malloc(sizeof(char)*MIL);
   tabel[0] = '\0';
   return tabel;
}

char** makeTable()
{
   char** newTab = malloc(sizeof(char*)*MAX);
   return newTab;
}

void fileReadStandard(char *data1)
{
    char line[MAX];
    int i = 0;
    int j = 0;

    while (gets(line) !=  NULL)
    {
         i=0;
         while (line[i] != '\0')
         {
          if((line[i] == ' ') || (line[i] == '\t') || (line[i] == '\n'))
          {
              i++;
              continue;
          }
          else
          {
              data1 = line[i];
              i++;
              j++;
          }
      }
   }
}

//---------READ DATA----------//
void fileRead(char *pathData, char *dataRewrite)
{
   FILE *file = fopen (pathData, "r");
   /*if we cannot open file*/
   if(file == NULL)
   {
       printf("napaka 1\n");
       exit(0);
   }

   char line[MAX];
   int i = 0;
   int j = 0;

   while (fgets(line, MAX, file) !=  NULL)
   {
       i=0;
      while (line[i] != '\0')
      {
          if((line[i] == ' ') || (line[i] == '\t') || (line[i] == '\n'))
          {
              i++;
              continue;
          }
          else
          {
              dataRewrite[j] = line[i];
              i++;
              j++;
          }
      }
   }
   fclose(file);
}

//-------------CREAT MEMORY FOR TAG---------------//
char *memoryString(char *tag1, char *tag)
{
   int len = strlen(tag);
   tag1 = malloc(sizeof(char)*(len+4));
   tag1[0] = '\0';
   return tag1;
}

//-------------GET VALUE BETWEEN TAGS-----------------//
char* getValue(char *tag, char *niz) /*check XML */
{
   //---SET BEGIN AND END TAG------//
   char *tag1 = memoryString(tag1,tag);
  /* tag2 = '<'+'\'+tag+'>'*/
   strcat(tag1, "<");
   strcat(tag1, tag);
   strcat(tag1, ">");

   char *tag2 = memoryString(tag2,tag);
   /* tag2 = '<'+'\'+tag+'>'*/
   strcat(tag2, "<");
   strcat(tag2, "/");
   strcat(tag2, tag);
   strcat(tag2, ">");

   //--------CHECK BETWEEN-------------//
   char *ptr1 = strstr(niz, tag1);
  
   char *ptr2 = strstr(niz, tag2);

   int lenT1 = strlen(tag1);

   char *between = malloc(sizeof(char)*1000);

   int dolg = (int)ptr2 - (int)ptr1 - lenT1;


  if(dolg == 0)       
  {
      printf("napaka 3");
      exit(0);
  }

   strncpy(between, ptr1 + lenT1, dolg );

   between[dolg] = '\0';

   /*nastavimo zadnji pointer za pregled celotne tabele ocena*/
   free(tag1);
   free(tag2);
   return between;
}

//-------CHECK XML---------------//
void check(char *data, char** ID, char** PREDMETI, char** OCENA)
{
   int len = strlen(data);

   //----CHECK OCENE------------------//
   char *pData = data;
   char *pointer = strstr(data, "<ocene>");

   if(!(pData == pointer))
   {
       printf("napaka 2\n");
        exit(1);
   }

   char *ERRORpointer = strstr(pointer+7, "<ocene>");

   if(!(ERRORpointer == NULL))
   {
       printf("napaka 2\n");
        exit(1);
   }

   char *pointerEnd = strstr(data, "</ocene>");

   int umes = pointerEnd-pointer;

   if(!((umes+8)==len))
   {
       printf("napaka 3\n");  
       exit(0);
   }

   char* buffer;
   char* curr = data;
   char* nID;
   char* nPR;
   char* nOC;
   int j = 0;
   int updated = 0;
   
   while(1)
   {
       if(curr + 15 == pointer+strlen(data))
           break;
           
      updated = 0;
      buffer = getValue("ocena", curr);
      curr += strlen(buffer) + 15;
      
      nID = getValue("vpisnaStevilka", buffer);
     
      nPR = getValue("nalogaId", buffer);
      
      nOC = getValue("vrednost", buffer);
      

       for(j = 0; j < length; j++)
       {
           if(strcmp(nID, ID[j]) == 0)
               if(strcmp(nPR, PREDMETI[j]) == 0)
               {
                   OCENA[j] = nOC;
                   updated++;
                   break;
               }
       }

       if(updated == 0)
       {
           ID[length] = nID;
           PREDMETI[length] = nPR;
           OCENA[length] = nOC;
           length++;
           continue;
       }
   }
}

//--------BUUBLE SORT FOR PRINTF-------//
void BubbleSort(char *ID[],char *PREDMET[], char *OCENA[], int array_size)
{
    int i, j;
    char *tempID= makeMemory(tempID);
    char *tempPredmet= makeMemory(tempPredmet);
    char *tempOcena= makeMemory(tempOcena);

    for (i = 0; i < (array_size -1); ++i)
    {
         for (j = 0; j < array_size - 1 - i; ++j )
         {
              if (atoi(ID[j]) > atoi(ID[j+1]))
              {
                   tempID = ID[j+1];
                   ID[j+1] = ID[j];
                   ID[j] = tempID;

                   tempPredmet = PREDMET[j+1];
                   PREDMET[j+1] = PREDMET[j];
                   PREDMET[j] = tempPredmet;

                   tempOcena = OCENA[j+1];
                   OCENA[j+1] = OCENA[j];
                   OCENA[j] = tempOcena;
              }
         }
    }
}

void sort(char *ID[], char *PREDMETI[], char *OCENA[])
{
   int lengOfTabels = length;
   int i, j;
   
   BubbleSort(ID, PREDMETI, OCENA, lengOfTabels);
   
   int ass = 0;
   int grade = 0;
   
   for(i = 0; i < lengOfTabels; i++)
   {
       if(ID[i][0] == NULL)
           continue;
           
       ass = 0;
       grade = 0;
       
       for(j = 0; j < lengOfTabels; j++)
       {
           if( strcmp(ID[i], ID[j]) == 0  )
           {
               ass++;
               grade += atoi(OCENA[j]);
               
               if( i != j)
                  ID[j][0] = NULL;

           }
       }
       sprintf(OCENA[i], "%d", grade);
       sprintf(PREDMETI[i], "%d", ass);
       printf("%s %s %s\n",ID[i], PREDMETI[i], OCENA[i]);
   }
}


//------------MAIN--------------//
int main(int argc, char *argv[])
{
   /*naredimo tabelo za prepis iz prve datoteke*/
   char *data1 = NULL;
   data1 = makeMemory(data1);
   
   if(argc == 1)
     fileReadStandard(data1);
   
   if(argc == 2)
     fileRead(argv[1], data1);

   char **ID = makeTable();
   char **PREDMET = makeTable();
   char **OCENA = makeTable();
   check(data1, ID, PREDMET, OCENA);

   sort(ID, PREDMET, OCENA);

  return 0;
}
