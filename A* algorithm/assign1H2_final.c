#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

typedef struct Board15{

    struct Board15* parent;
    int b[16];
    int blank;
    int cost;
}Board15;

typedef struct List{

    Board15 *board;
    struct List* right;
}List;

void printBoard(Board15 *board){

    int i,j;
    j=0;
    for(i=0;i<16;i++){

        if(board->blank==i){

            printf("  ");
        }
        else{

            printf("%d ",board->b[i]);
        }
        j++;
        if(j%4==0){

            printf("\n");
        }
    }
    printf("Blank: %d\n\n",board->blank+1);
}

void swapTiles(Board15 *board,int p1,int p2){

    int temp = board->b[p2];
    board->b[p2] = board->b[p1];
    board->b[p1] = temp;
    board->blank = p1;
}

int checkSameBoard(Board15* board1,Board15* board2){

    int i;
    for(i=0;i<16;i++){

        if(board1->b[i] != board2->b[i]){

            return -1;
        }
    }
    return 1;
}

List* openList;
List* closeList;

List* removeBoard(List* list,Board15* board){

    List* listhead = list;
    if(checkSameBoard(list->board,board)==1){

        list = list->right;
    }
    else{

        while(listhead->right!=NULL){

            if(checkSameBoard(listhead->right->board,board)==1){

                listhead->right = listhead->right->right;
                break;
            }
            listhead = listhead->right;
        }
    }
    return list;
}

int checkLists(Board15* board){

    List* openListHead = openList;
    while(openListHead->board!=NULL){

        if(checkSameBoard(openListHead->board,board)==1){

            if(openListHead->board->cost > board->cost){

                openListHead->board->cost = board->cost;
                openListHead->board->parent = board->parent;
            }
            return 1;
        }
        openListHead = openListHead->right;
    }

    List* closeListHead = closeList;
    while(closeListHead->board!=NULL){

        if(checkSameBoard(closeListHead->board,board)==1){

            if(closeListHead->board->cost > board->cost){

                closeList = removeBoard(closeList,closeListHead->board);
                return -1;
            }
            return 1;
        }
        closeListHead = closeListHead->right;
    }
    return -1;
}

int getPosition(int value,Board15* board){

    int i;
    for(i=0;i<16;i++){

        if(board->b[i] == value){

            return i;
        }
    }
}

float getCost(Board15* board,Board15* startboard){

    int i;
    int position;
    float cost=0;
    for(i=0;i<16;i++){

        position = getPosition(board->b[i],startboard);
        cost += abs(((i/4)-(position/4)))+abs(((i%4)-(position%4)));
    }
    return cost;
}

void getPossibleNodes(Board15 *head,Board15* startboard, int moves, int* time_comp, int* space_comp){

    List* openListHead = openList;
    while(openListHead->board!=NULL){

        openListHead = openListHead->right;
    }
    if(head->blank == 5 || head->blank == 6 || head->blank == 9 || head->blank == 10){

        *time_comp +=4;
        int hBlank = head->blank;
        Board15 *b1 = (Board15 *)malloc(sizeof(Board15));
        Board15 *b2 = (Board15 *)malloc(sizeof(Board15));
        Board15 *b3 = (Board15 *)malloc(sizeof(Board15));
        Board15 *b4 = (Board15 *)malloc(sizeof(Board15));

        List *temp =(List *)malloc(sizeof(List));
        List *temp2 =(List *)malloc(sizeof(List));
        List *temp3 =(List *)malloc(sizeof(List));
        List *temp4 =(List *)malloc(sizeof(List));

        swapTiles(head,hBlank-4,hBlank);
        *b1 = *head;
        swapTiles(head,hBlank,hBlank-4);
        
        swapTiles(head,hBlank+4,hBlank);
        *b2 = *head;
        swapTiles(head,hBlank,hBlank+4);
        
        swapTiles(head,hBlank-1,hBlank);
        *b3 = *head;
        swapTiles(head,hBlank,hBlank-1);
        
        swapTiles(head,hBlank+1,hBlank);
        *b4 = *head;
        swapTiles(head,hBlank,hBlank+1);
        
        if(checkLists(b1)==-1){

            *space_comp += 1;
            b1->parent = head;
            b1->cost = moves;
            openListHead->board = b1;
            openListHead->right = temp;
            openListHead = openListHead->right;
        }
        if(checkLists(b2)==-1){

            *space_comp += 1;
            b2->parent = head;
            b2->cost = moves;
            openListHead->board = b2;
            openListHead->right = temp2;
            openListHead = openListHead->right;
        }
        if(checkLists(b3)==-1){

            *space_comp += 1;
            b3->parent = head;
            b3->cost = moves;
            openListHead->board = b3;
            openListHead->right = temp3;
            openListHead = openListHead->right;
        }
        if(checkLists(b4)==-1){

            *space_comp += 1;
            b4->parent = head;
            b4->cost = moves;
            openListHead->board = b4;
            openListHead->right = temp4;
            openListHead = openListHead->right;
        }
        openListHead->board=NULL;
        openListHead->right=NULL;
    }
    else if(head->blank == 1 || head->blank == 2 || head->blank == 7 || head->blank == 11 || head->blank == 4 || head->blank == 8 || head->blank == 13 || head->blank == 14 ){

        *time_comp +=3;
        int hBlank = head->blank;
        Board15 *b1 = (Board15 *)malloc(sizeof(Board15));
        Board15 *b2 = (Board15 *)malloc(sizeof(Board15));
        Board15 *b3 = (Board15 *)malloc(sizeof(Board15));

        List *temp =(List *)malloc(sizeof(List));
        List *temp2 =(List *)malloc(sizeof(List));
        List *temp3 =(List *)malloc(sizeof(List));

        if(hBlank == 1 || hBlank == 2){

            swapTiles(head,hBlank-1,hBlank);
            *b1 = *head;
            swapTiles(head,hBlank,hBlank-1);
            
            swapTiles(head,hBlank+4,hBlank);
            *b2 = *head;
            swapTiles(head,hBlank,hBlank+4);
            
            swapTiles(head,hBlank+1,hBlank);
            *b3 = *head;
            swapTiles(head,hBlank,hBlank+1);
        }
        else if(hBlank == 13 || hBlank == 14){

            swapTiles(head,hBlank-1,hBlank);
            *b1 = *head;
            swapTiles(head,hBlank,hBlank-1);
            
            swapTiles(head,hBlank-4,hBlank);
            *b2 = *head;
            swapTiles(head,hBlank,hBlank-4);
            
            swapTiles(head,hBlank+1,hBlank);
            *b3 = *head;
            swapTiles(head,hBlank,hBlank+1);
        }
        else if(hBlank == 4 || hBlank == 8){

            swapTiles(head,hBlank+4,hBlank);
            *b1 = *head;
            swapTiles(head,hBlank,hBlank+4);
            
            swapTiles(head,hBlank-4,hBlank);
            *b2 = *head;
            swapTiles(head,hBlank,hBlank-4);
            
            swapTiles(head,hBlank+1,hBlank);
            *b3 = *head;
            swapTiles(head,hBlank,hBlank+1);
        }
        else if(hBlank == 7 || hBlank == 11){

            swapTiles(head,hBlank+4,hBlank);
            *b1 = *head;
            swapTiles(head,hBlank,hBlank+4);
            
            swapTiles(head,hBlank-4,hBlank);
            *b2 = *head;
            swapTiles(head,hBlank,hBlank-4);
            
            swapTiles(head,hBlank-1,hBlank);
            *b3 = *head;
            swapTiles(head,hBlank,hBlank-1);
        }

        if(checkLists(b1)==-1){
         
            *space_comp += 1;
            b1->parent = head;
            b1->cost = moves;
            openListHead->board = b1;
            openListHead->right = temp;
            openListHead = openListHead->right;
        }
        if(checkLists(b2)==-1){

            *space_comp += 1;
            b2->parent = head;
            b2->cost = moves;
            openListHead->board = b2;
            openListHead->right = temp2;
            openListHead = openListHead->right;
        }
        if(checkLists(b3)==-1){

            *space_comp += 1;
            b3->parent = head;
            b3->cost = moves;
            openListHead->board = b3;
            openListHead->right = temp3;
            openListHead = openListHead->right;
        }
        openListHead->board=NULL;
        openListHead->right=NULL;
    }
    else if(head->blank == 0 || head->blank == 3 || head->blank == 12 || head->blank == 15){

        *time_comp +=2;
        int hBlank = head->blank;
        Board15 *b1 = (Board15 *)malloc(sizeof(Board15));
        Board15 *b2 = (Board15 *)malloc(sizeof(Board15));

        List *temp =(List *)malloc(sizeof(List));
        List *temp2 =(List *)malloc(sizeof(List));

        if(hBlank == 0){

            swapTiles(head,hBlank+1,hBlank);
            *b1 = *head;
            swapTiles(head,hBlank,hBlank+1);
            
            swapTiles(head,hBlank+4,hBlank);
            *b2 = *head;
            swapTiles(head,hBlank,hBlank+4);
        }
        else if(hBlank == 3){

            swapTiles(head,hBlank-1,hBlank);
            *b1 = *head;
            swapTiles(head,hBlank,hBlank-1);
            
            swapTiles(head,hBlank+4,hBlank);
            *b2 = *head;
            swapTiles(head,hBlank,hBlank+4);
        }
        else if(hBlank == 12){

            swapTiles(head,hBlank-4,hBlank);
            *b1 = *head;
            swapTiles(head,hBlank,hBlank-4);
            
            swapTiles(head,hBlank+1,hBlank);
            *b2 = *head;
            swapTiles(head,hBlank,hBlank+1);
        }
        else if(hBlank == 15){

            swapTiles(head,hBlank-1,hBlank);
            *b1 = *head;
            swapTiles(head,hBlank,hBlank-1);
            
            swapTiles(head,hBlank-4,hBlank);
            *b2 = *head;
            swapTiles(head,hBlank,hBlank-4);
        }       

        if(checkLists(b1)==-1){
         
            *space_comp += 1;
            b1->parent = head;
            b1->cost = moves;
            openListHead->board = b1;
            openListHead->right = temp;
            openListHead = openListHead->right;
        }
        if(checkLists(b2)==-1){

            *space_comp += 1;
            b2->parent = head;
            b2->cost = moves;
            openListHead->board = b2;
            openListHead->right = temp2;
            openListHead = openListHead->right;
        } 
        openListHead->board=NULL;
        openListHead->right=NULL;
    } 
}

//Heuristic - Number of misplaced tiles
Board15* optimalBoard1(Board15* finalboard){

    List* listhead = openList;
    float f_min = -1;
    int i;
    Board15* optBoard = NULL;
    float cost = 0;
    while(listhead->right!=NULL){

        cost = getCost(listhead->board,finalboard);
        if(f_min==-1){

            f_min = cost + listhead->board->cost;
            optBoard = listhead->board;
        }
        else if(f_min > cost + listhead->board->cost){

            f_min = cost + listhead->board->cost;
            optBoard = listhead->board;
        }
        listhead = listhead->right;
    }
    
    printf("F Value:%f\n",f_min);
    openList=removeBoard(openList,optBoard);
    return optBoard;
}

int checkFinalState(Board15* board){

    int i=0;
    for(i=0;i<15;i++){

        if(board->b[i]!=i+1){

            return -1;
        }
    }
    if(board->blank==15){

        return 1;
    }
    else{

        return -1;
    }
}

Board15* solvePuzzle15(Board15* board, int* time_comp, int* space_comp,Board15* finalboard){

    Board15* startboard = board; 
    List* closehead = closeList;
    List* node = (List*)malloc(sizeof(List));
    node->board=NULL;
    node->right=NULL;
    openList->board=board;
    openList->right=node;
    *time_comp +=1;
    *space_comp += 1; 

    int moves=0;
    while(openList->board!=NULL){

        closehead->board=optimalBoard1(finalboard);
        printf("Optimal:\n");
        printBoard(closehead->board);
        closehead->right = (List *)malloc(sizeof(List));
        
        if(checkFinalState(closehead->board)==1){

            return closehead->board;
        }

        moves++;
        getPossibleNodes(closehead->board,startboard,moves,time_comp,space_comp);

        closehead = closehead->right;
    }
    return closehead->board;
}

int getInvCount(Board15* board){
    int inv_count = 0;
    int i,j;
    for(i=0;i<15;i++){
        for(j=i+1;j<16;j++){            
            if (board->b[j] && board->b[i] && board->b[i] > board->b[j]){

                inv_count++;
            }
        }
    }
    return inv_count;
}

int findXPosition(Board15* board){
    int i,j;
    for(i=3;i>=0;i--){

        for(j=3;j>=0;j--){

            if(board->b[4*i+j] == 0){

                return 4-i;
            }
        }
    }
}

int isSolvable(Board15* board){
    int invCount = getInvCount(board);
    int pos = findXPosition(board);
    if (pos%2 == 0 && invCount%2 != 0){

        return 1;
    }
    else if(pos%2 == 1 && invCount%2 == 0){

        return 1;
    }
    else{

        return 0;
    }    
}

int main(){
    srand((unsigned int) time(NULL));
    Board15 *board =(Board15 *)malloc(sizeof(Board15));
    Board15 *final =(Board15 *)malloc(sizeof(Board15));
    openList =(List *)malloc(sizeof(List));
    closeList =(List *)malloc(sizeof(List)); 

    board->blank = rand()%16;
    board->parent = NULL;
    int i,j;
    for(i=0;i<16;i++){

        if(board->blank!=i){
            board->b[i] = 1 + rand()%15;
            for(j=0;j<i;j++){

                if(board->b[i]==board->b[j]){

                    i--;
                    break;
                }
            }
        }
        else{
            board->b[i] = 0;
        }
    }
    for(i=0;i<16;i++){

        final->b[i] = (i+1)%16;
    }

    printf("Starting State:\n");
    printBoard(board);
    if(checkFinalState(board)==1){

        printf("Final State:\n");
        printBoard(board);
    }
    else if(isSolvable(board)==0){

        printf("The puzzle cannot be solved.\n");
    }
    else{
        
        int *time_comp =(int *)malloc(sizeof(int));
        int *space_comp =(int *)malloc(sizeof(int));
        *time_comp=0;
        *space_comp=0;
        Board15* finalboard=solvePuzzle15(board,time_comp,space_comp,final);
        if(finalboard!=NULL){

            printf("Solved!!!\n\n");
            printf("Moves: %d\n",finalboard->cost);
            printf("Time: %d nodes generated.\n",*time_comp);
            printf("Space: %d nodes in memory.\n\n",*space_comp);            
        }
        else{

            printf("The puzzle cannot be solved.\n");
        }
    }
    return 0;
}