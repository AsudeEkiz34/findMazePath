
//
//  main.c
//  veriYapilariProjeSon
//
//  Created by Asude Ekiz


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


enum field{
    path, //represents 0
    wall, //represens 1
    finish, //represent 2
    trace,//represents 3
    apple //represents 4
};

char **allocateMaze(int row,int column);
//char **fillMazeWithDots(char **maze, int row, int column);
char **fillMazeFromFile(char **maze, char *mazeLine, int row, int column);
void printMaze(char **maze,int row, int column);

int **allocateVisited(int row, int column);
int **fillVisitedWithEnums(char **maze,int **visited, int row,int column);
void printVisited(int **visited,int row, int column);


void addRandomApples(char **maze,int row,int column);
void findStartingPoint(char **maze,int row,int column ,int *srow,int *scol);
int solveTheMaze(int row, int column,char**maze, int**visited,int *point);
void markThePath(char **maze, int **visited, int row, int column);




int main(int argc, const char * argv[]) {
    
    
    
    printf("\n\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
      printf("\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
      printf("\n\t\t\t        =                  WELCOME                  =");
      printf("\n\t\t\t        =                    TO                     =");
      printf("\n\t\t\t        =            Finding Maze Path              =");
      printf("\n\t\t\t        =                  PROJECT                  =");
      printf("\n\t\t\t        =         created by Asude Merve Ekiz       =");
      printf("\n\t\t\t        =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
      printf("\n\t\t\t  **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");

    FILE *fp;
     char buffer[255],mazeLine[255];
     char c;
     int zero=0;
     int *point=&zero;
     int column=0,row=0,i=0,j=0;
     char **maze = NULL;
     int **visited;
     int srow=0,scol=0;
    srand(time(NULL));
    fp=fopen("maze.txt", "r");
    if (fp==NULL) {
        printf("Could not open the file\n");}
    else{
        fgets(buffer, 255, fp);
        column= strlen(buffer);
        column--;
        c=getc(fp);
        while (c!=EOF) {
            if (c == '\n'){
                row = row + 1;}
            c=getc(fp);}
        row+=2;}

    //printf("\n Column number:%d",column);
    //printf("\nRow number:%d\n",row);
    
    maze=allocateMaze(row, column);
    maze=fillMazeFromFile(maze, mazeLine, row, column);
    printMaze(maze, row, column);
    
    visited=allocateVisited(row, column);
    addRandomApples(maze, row, column);
    printf("\n");
    printMaze(maze, row, column);
    visited=fillVisitedWithEnums(maze, visited, row, column);
    //printf("\nVisitedMatrix\n");
    //printVisited(visited, row, column);
    findStartingPoint(maze, row, column, &srow, &scol);
    //printf("Starting point row:\n %d",srow);
    //printf("Starting point of column:\n %d",scol);
    solveTheMaze(srow, scol, maze, visited, point);
    markThePath(maze, visited, row, column);
    //printf("After the maze solved\n\n");
    printMaze(maze, row, column);
    printf("Total point is:%d\n",*point);
    
    return 0;
}



char **allocateMaze(int row,int column){
    char **tmp;
    int i;
    tmp= (char**) malloc((row)*sizeof(char*));
    for(i=0;i<(row);i++){
        tmp[i] = (char*) malloc((column)*sizeof(char));
    }
    return tmp;
}

int **allocateVisited(int row, int column){
    int **tmp;
    int i;
    tmp= (int**) malloc((row)*sizeof(int*));
    for(i=0;i<(row);i++){
        tmp[i] = (int*) malloc((column)*sizeof(int));
    }
    return tmp;
}


char **fillMazeWithDots(char **maze, int row, int column){
    int i,j;
    for(i=0;i<(row);i++){
        for(j=0;j<(column);j++){
            maze[i][j]='.';
        }
    }
    return maze;
}

char **fillMazeFromFile(char **maze, char *mazeLine, int row, int column){
    FILE *fp;
    int i;
    fp=fopen("maze.txt", "r");
    if (fp==NULL) {
        printf("Could not open the file\n");

    }
    else{
        
        i=0;
        while (fgets(mazeLine, 255, fp)!=NULL) {
            strcpy(maze[i], mazeLine);
            i++;
        }
    }
    fclose(fp);
    return maze;
}

int **fillVisitedWithEnums(char **maze,int **visited, int row,int column){
    int i, j;
    for (i=0; i<row; i++) {
        for (j=0; j<column; j++) {
            if (maze[i][j]=='+'||maze[i][j]=='-'||maze[i][j]=='|'||maze[i][j]=='*') {
                visited[i][j]=wall;
            }
            else if(maze[i][j]=='c'){
                visited[i][j]=finish;
            }
            else if(maze[i][j]=='o'){
                visited[i][j]=apple;
            }
            else
                visited[i][j]=path;
        }
    }
    return visited;
}

void printMaze(char **maze,int row, int column){
    int i,j;
    for(i=0;i<(row);i++){
        for(j=0;j<(column);j++){
            printf("%c",maze[i][j]);
        }
        printf("\n");
    }
}

void printVisited(int **visited,int row, int column){
    int i,j;
    for(i=0;i<(row);i++){
        for(j=0;j<(column);j++){
            printf("%d",visited[i][j]);
        }
        printf("\n");
    }
}

int solveTheMaze(int row, int column,char**maze, int**visited,int *point){
    int* current=&visited[row][column];
    if(*current==finish){
        printf("\n\n Maze solved!!!\n");
        return 1;
    }
   
    if(*current==path||*current==apple){
       
        if (*current==apple) {
            (*point)+=10;
            printf("\nCongrats!!You met with an apple and earned 10 points.\n");
        }
        *current=trace;
        
        if (solveTheMaze(row-1,column,maze,visited,point)) { //kuzey
            *current=trace;
            
            return 1;
        }
        if (solveTheMaze(row,column-1,maze,visited,point)) {//batı
            *current=trace;
            return 1;
        }
        if (solveTheMaze(row+1,column,maze,visited,point)) {//guney
            *current=trace;
            return 1;
        }
        if (solveTheMaze(row,column+1,maze,visited,point)) {//dogu
            *current=trace;
            return 1;
        }
        else{
            *current = path;
            if ((visited[row+1][column]!=path)&&(visited[row-1][column]!=path)&&(visited[row][column+1]!=path)&&(visited[row][column-1]!=path)) {
                (*point)-=5;
                printf("\nYou hit the wall and lost 5 points.:( \n ");
                
            }
        }
    }
    
    return 0;
}

void findStartingPoint(char **maze,int row,int column ,int *srow,int *scol){
    int i,j;
    for (i=0; i<row; i++) {
        for (j=0; j<column; j++) {
            if (maze[i][j]=='b') {
                *srow=i;
                *scol=j;
            }
        }
    }
}

void markThePath(char **maze, int **visited, int row, int column){
    int i,j;
    for (i=0; i<row; i++) {
        for (j=0; j<column; j++) {
            if (maze[i][j]!='b') {
                if (visited[i][j]==trace) {
                    maze[i][j]='.';
                }
            }
        }
    }
    
}

void addRandomApples(char **maze,int row,int column){
    int i,j;
    int appleNumber=10;
    int num=0;
    while (num<appleNumber) {
        i = rand() % row;
        j = rand() % column;
       
        if (maze[i][j]==' ') {
            maze[i][j]='o';
            num++;
    
        }
        
    }
    
}
