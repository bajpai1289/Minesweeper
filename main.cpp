#include<stdio.h>
#include<graphics.h>
#include<time.h>

#define SPRITES_NUM 13

const int W = 672, H = 784;
const int ROWS = 14, COLS = 12;

void theGame(int numofbombs);   /* runs the mouse interface and checks for gameover */
void pre_loading(void* sprites[]);  /* loads the sprites from hard disk to RAM and sets the initial state of the game */
void initgrid(int numofbombs);  /* initializes the grids */
void initbombs(int bombs);  /* initializes the random position for the bombs */
int disclosecell(int row, int col);  /* opens the cells */
void thegameover();  /* displays the final state of the game */

/* setbkcolour(): function of graphics.h, responsible for changing background color */
void background(int colour){
    setfillstyle(SOLID_FILL, colour);
    bar(0, 0, W-1, H-1);
    return;
}

int main(){
    /* number of bombs: 14.5% of total cells are to be bomb */
    int numofbombs = ROWS*COLS*29.0/200.0;

    initwindow(W, H, "HELLO");
    initgrid(numofbombs);
    theGame(numofbombs);

    return 0;

}

void* (sprites[SPRITES_NUM]);
const int cellw = W/COLS, cellh = H/ROWS;  /* choose the WIDTH & HEIGHT and ROWS & COLS such that cellw and cellh will be equal */
int cellnum[ROWS+2][COLS+2];  /* to keep all the details of cell */
char cellstate[ROWS+2][COLS+2];  /* to keep track of whether the cell is opened or closed */
enum{
    BLANK, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, BOMB, FLAG, WFLAG, CLOSED, OPENED
};


void theGame(int numofbombs){

    int mx, mc, my, mr;
    bool gameover = false;
    int exposedcells = 0;
    pre_loading(sprites);

    while(!mousex() || !mousey())
        delay(32);

    /* press escape to exit */
    while(!GetAsyncKeyState(VK_ESCAPE) && !gameover){

        if(ismouseclick(WM_MOUSEMOVE)){
            mc = mousex()/cellw;
            mr = mousey()/cellh;
            mx = mc*cellw;
            my = mr*cellh;
        }

        /* click left mouse button to open the cells */
        if(ismouseclick(WM_LBUTTONDOWN) && cellstate[mr+1][mc+1] == CLOSED){
            cellstate[mr+1][mc+1] = OPENED;
            exposedcells++;
            putimage(mx, my, sprites[cellnum[mr+1][mc+1]], COPY_PUT);
            if(cellnum[mr+1][mc+1] == BOMB){
                /* gameover(LOSS): if the clicked cell has bomb */
                thegameover();
                gameover = true;
            }
            else if(cellnum[mr+1][mc+1] == BLANK)
                exposedcells += disclosecell(mr+1, mc+1);
        }
        /* click right mouse button to flag the cells */
        else if(ismouseclick(WM_RBUTTONDOWN) && cellstate[mr+1][mc+1] != OPENED){
            if(cellstate[mr+1][mc+1] == FLAG){
                cellstate[mr+1][mc+1] = CLOSED;
                putimage(mx, my, sprites[CLOSED], COPY_PUT);
            }
            else{
                cellstate[mr+1][mc+1] = FLAG;
                putimage(mx, my, sprites[FLAG], COPY_PUT);
            }
        }

        clearmouseclick(WM_MOUSEMOVE);
        clearmouseclick(WM_LBUTTONDOWN);
        clearmouseclick(WM_RBUTTONDOWN);
        delay(48);

        /* gameover(WIN): if the number of opened cells equals total number of cells without bombs */
        if(exposedcells == (ROWS*COLS - numofbombs))
            gameover = true;

    }


    for(int i = 0; i<SPRITES_NUM; i++){
        free(sprites[i]);
    }

    getch();
    return;

}

void pre_loading(void* sprites[]){
    unsigned int img_size;

    /* names of the sprites */
    char sprite_names[SPRITES_NUM][24] = {"sprites/blank.ico", "sprites/1.ico",
                                          "sprites/2.ico", "sprites/3.ico",
                                          "sprites/4.ico", "sprites/5.ico", "sprites/6.ico",
                                          "sprites/7.ico", "sprites/8.ico","sprites/bomb.ico",
                                          "sprites/flag.ico", "sprites/wflag.ico", "sprites/closed.ico"};

    img_size = imagesize(0, 0, cellw, cellh);

    setactivepage(ONE);

    /* loading the sprites from disk to RAM */
    for(int i = 0; i<SPRITES_NUM; i++){
    	
        readimagefile(sprite_names[i], 0, 0, cellw-1, cellh-1);
        sprites[i] = malloc(img_size);
        getimage(0, 0, cellw-1, cellh-1, sprites[i]);
    }
    cleardevice();

    /* displaying the closed cells */
    for(int px = 0; px < W; px+=cellw){
        for(int py = 0; py < H; py+=cellh){
            putimage(px, py, sprites[CLOSED], COPY_PUT);
        }
    }

    setvisualpage(ONE);

    return;
}


void initgrid(int numofbombs){

    /* seeding the random function */
    srand(time(NULL));

    for(int i = 0; i < ROWS + 2; i++){
        for(int j = 0; j < COLS + 2; j++){
            /* Setting the grids */
            cellnum[i][j] = BLANK;
            cellstate[i][j] = CLOSED;
        }
        /* Setting the LEFT and the RIGHT boundary of both grids accordingly*/
        cellstate[i][COLS + 1] = cellstate[i][0] = OPENED;
    }

    /* Setting the TOP and the BOTTOM boundary of both grids accordingly*/
    for(int j = 0; j < COLS + 2; j++){
        cellstate[ROWS + 1][j] = cellstate[0][j] = OPENED;
    }

    initbombs(numofbombs);

    return;
}


void initbombs(int bombs){

    if(!bombs)  /* terminating condition: if amounts of bombs is zero */
        return;
    else{
        int bombposx, bombposy;

        /* loop continues until random position selected doesn't have bomb */
        while(cellnum[bombposy = (1 + rand()%ROWS)][bombposx = (1 + rand()%COLS)] == BOMB)
            ;

        /* assigning bomb to the selected random position */
        cellnum[bombposy][bombposx] = BOMB;

        /* applying recursion to mark other bombs before incrementing the neighbors */
        initbombs(bombs - 1);

        for(int j = bombposy - 1; j < bombposy + 2; j++)
            for(int k = bombposx - 1; k < bombposx + 2; k++)
                if(cellnum[j][k] != BOMB)
                    cellnum[j][k]++;  /* increments the neighbors of the bomb only if they aren't bombs themselves */

    }
    return;

}


int disclosecell(int row, int col){
    int counter = 0;
    for(int i = row - 1; i < row + 2; i++){
        for(int j = col - 1; j< col + 2; j++){
            if(cellstate[i][j] == CLOSED){  /* opens the closed cells only */
                counter++;  /* increments amount of opened cells */
                cellstate[i][j] = OPENED;   /* updates the state of the cell */
                putimage((j-1)*cellw, (i-1)*cellh, sprites[cellnum[i][j]], COPY_PUT);  /* renders the corresponding sprites at the required position */

                if(cellnum[i][j] == BLANK)
                    counter += disclosecell(i, j);  /* recursion */
            }
        }
    }
    return counter;
}


void thegameover(){
    setactivepage(TWO);
    for(int i = 0; i<ROWS; i++){
        for(int j = 0; j<COLS; j++){
            if(cellstate[i+1][j+1] == FLAG){
                if(cellnum[i+1][j+1] == BOMB)
                    putimage(j*cellw, i*cellh, sprites[FLAG], COPY_PUT);
                else
                    putimage(j*cellw, i*cellh, sprites[WFLAG], COPY_PUT);
            }
            else
                putimage(j*cellw, i*cellh, sprites[cellnum[i+1][j+1]], COPY_PUT);
        }
    }
    setvisualpage(TWO);
    return;
}

