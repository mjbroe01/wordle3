#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char dict[13000][8];
int dictLen;

char guess[2400][8];
int guessLen;

unsigned char *dictGuess[13000];

typedef struct _guessBins {
  int bin;
  int idx;
} GuessBins;

void init() {
  int i,j,k,n,x;
  char buff[8], buff2[8], *pbuff;
  FILE *inDict, *inGuess;

  inDict  = fopen("list.dict" ,"r");
  inGuess = fopen("list.guess","r");

  for(i=0; i<12972; i++) {
    fscanf(inDict,"%s\n",dict[i]);
  }
  dictLen = 12972;

  
  for(i=0; i<2315; i++) {
    fscanf(inGuess,"%s\n",guess[i]);
  }
  guessLen = 2315;

  for(i=0; i<dictLen; i++) {
    dictGuess[i] = (char *)malloc(sizeof(char) * guessLen);
  }

  printf("HI\n");
  
  for(i=0; i<dictLen; i++) {
    for(j=0; j<guessLen; j++) {
      strcpy(buff,guess[j]);
      strcpy(buff2,dict[i]);
      
      for(n=k=0,x=2; k<5; k++,x*=3) {
	if (buff2[k] == buff[k]) {
	  n+=x;
	  buff2[k] = buff[k] = ' ';
	}
      }

      for(k=0,x=1; k<5; k++,x*=3) {
	pbuff = strchr(buff,buff2[k]);
        if (pbuff && buff2[k] != ' ') {
	  n+=x;
          pbuff[0] = ' ';
        }
      }

      dictGuess[i][j] = n;
    }
  }
}


int binComp(const void* a, const void* b) {
    return ((GuessBins*)a)->bin - ((GuessBins*)b)->bin;
}


void main() {
  int i,j,k,m;
  int kk;
  
  GuessBins guessBin[2400];
  
  int lastBin, startGuess;
  int bestWord[256];
  int bestUsed[256];
  int countBin[256];
  int binSize[256];
  int inBin[256];

  int count,bestBin,maxBin;
  init();

  printf("Init Done\n");
  maxBin = 0;
  //  for(i=0; i<dictLen; i++) {
  for(i=11581; i<11582; i++) {
    for(j=0; j<guessLen; j++) {
      guessBin[j].bin = dictGuess[i][j];
      guessBin[j].idx = j;
    }

    guessBin[j].idx = -1;
    guessBin[j].bin = 255;

    qsort(guessBin,guessLen,sizeof(GuessBins),&binComp);

    printf("%4d %s \n",i,dict[i]);
    
    for(j=0; j<256; j++) {
      bestWord[j]=-1;
      bestUsed[j]=0;
      binSize[j]=0;
      inBin[j]=0;
    }
    
    lastBin = guessBin[0].bin;
    for(startGuess=j=0; j<=guessLen; j++) {
      printf("%s %3d %4d\n",guess[guessBin[j].idx],guessBin[j].bin,j);
	    
      if (lastBin != guessBin[j].bin) {
	printf("GGG %d %d %d\n",guessBin[j].bin,startGuess,j);

	bestBin=0;

	for(k=0; k<dictLen && bestBin < j-startGuess; k++) {
	  memset(countBin,0,256 * sizeof(int));

	  for(m=startGuess,count=0; m<j; m++) {
	    kk = dictGuess[k][guessBin[m].idx];
	    if (countBin[kk]==0) {
	      countBin[kk]=1;
	      count++;
	    }
	  }

	  if (count > bestBin) {
	    bestBin = count;
	    bestWord[lastBin] = k;
	    bestUsed[lastBin] = bestBin;
	    binSize[lastBin]  = j-startGuess;
	    inBin[lastBin]    = 0;	    
	    for(m=startGuess; m<j; m++) {
	      if (!strcmp(guess[guessBin[m].idx],dict[k])) {
		inBin[lastBin] = 1;		
	      }
	    }
	  }
	}
	lastBin = guessBin[j].bin;
	startGuess=j;
      }
    }

    count = 0;
    for(j=0; j<256; j++) {
      count += bestUsed[j];
    }

    if (count > maxBin) {
      maxBin = count;
      printf("current %5d %s %d\n",i,dict[i],maxBin);
      for(j=0; j<256; j++) {
	if (bestUsed[j]) {
	  printf("    %4d %5d %s %4d %d\n",j,bestUsed[j],dict[bestWord[j]],binSize[j], inBin[j]);
	}
      } printf("\n");
    }
  }    
}
