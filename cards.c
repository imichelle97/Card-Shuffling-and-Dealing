/*
 * cards.c
 *
 *  Created on: Jul 23, 2017
 *      Author: michelle
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUITS 4
#define FACES 13
#define CARDS 52

//Function Prototypes
void shuffle(int wDeck[][FACES]);
void deal(const int wDeck[][FACES], const char *wface[], const char *wSuit[], FILE *p, char *result[], unsigned int rindex);
void display(const char *wfFace[], const char *wfSuit[], const char *wsFace[], const char *wsSuit[], FILE *p);

int main(void) {

	//Print ouput into a text file
	FILE *p;
	FILE *r;

	p = fopen("projectLog.txt", "w");		//Includes all 100 rounds of the poker game
	r = fopen("projectResult.txt", "w");	//Includes the results from eval in an array

	//initialize suit array
	const char *suit[SUITS] = {"Hearts", "Diamonds", "Clubs", "Spades"};

	//initialize face array
	const char *face[FACES] = {"Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven",
								"Eight", "Nine", "Ten", "Jack", "Queen", "King"};

	char *result[100];

	//initialize deck array
	int deck[SUITS][FACES] = {0};

	srand(time(0));

	//Plays the poker game 100 times
	for(unsigned int hand = 1; hand <= 100; hand++) {

		fprintf(p, "ROUND %u: \n", hand);

		shuffle(deck);	//Shuffles the deck
		deal(deck, face, suit, p, result, hand-1);	//deal the deck

		fprintf(r, "%s\n", result[hand-1]);

		for(int i = 0; i < SUITS; i++) {
			for(int j = 0; j < FACES; j++) {
				deck[i][j] = 0;
			}
		}

	}

	fclose(p);
	fclose(r);

}

//shuffle cards in deck
void shuffle(int wDeck[][FACES]) {
	int row, column, card;

	//for each card, choose slot of deck randomly
	for(card = 1; card <= CARDS; card++) {
		//choose new random location until unoccupied slot found
		do {
			row = rand() % SUITS;
			column = rand() % FACES;
		}
		while(wDeck[row][column] != 0);

		//place card number in chosen slot of deck
		wDeck[row][column] = card;
	}
}

//deal 5-cards poker hand in deck
void deal(const int wDeck[][FACES], const char *wFace[], const char *wSuit[], FILE *p, char *result[], unsigned int rindex) {

	//operational functions
	void pair(const char *wfFace[], const char *wfSuit[], int result[], FILE *p);
	void twoPair(const char *wfFace[], const char *wfSuit[], int result[], FILE *p);
	void threeKind(const char *wfFace[], const char *wfSuit[], int result[], FILE *p);
	void fourKind(const char *wfFace[], const char *wfSuit[], int result[], FILE *p);
	void flush(const char *wfFace[], const char *wfSuit[], int result[], FILE *p);
	void straight(const char *wfFace[], const char *wfSuit[], const char *wFace[], int result[], FILE *p);
	void eval(const int firstResult[], const int secondResult[], FILE *p, char *result[], unsigned int rindex);


	int card, row, column, i, j = 0;

	/*Storage for the two five-hand cards
	  Boolean Result:
	  	  0 = Pair
	  	  1 = Two Pair
	  	  2 = Three of a kind
	  	  3 = Four of a kind
	  	  4 = Flush
	  	  5 = Straight
	 */

	//FIRST HAND
	char *fSuit[5];
	char *fFace[5];
	int fResult[6] = {0};

	//SECOND HAND
	char *sSuit[5];
	char *sFace[5];
	int sResult[6] = {0};


	//deal 5-card poker hand
	for(card = 1, i = 0; card <= 10; card++, i++){
		for(row = 0; row < 4; row++){
			for(column = 0; column < 13; column++){
				if(wDeck[ row ][ column ] == card && i < 5){
					fSuit[ i ] = wSuit[ row ];
	                fFace[ i ] = wFace[ column ];
				}else if(wDeck[ row ][ column ] == card && i > 4){
					sSuit[ j ] = wSuit[ row ];
	                sFace[ j ] = wFace[ column ];
	                j++;
				}
			}
		}
	}

	//display 5-card poker hand
	display(fFace, fSuit, sFace, sSuit, p);

	//EVALUATE
	fprintf(p,"\n******************\nEvaluate both hand\n******************\n\nFirst hand\n**********\n\n");
	pair( fFace, fSuit, fResult, p);
	//fprintf(p,"\n  fResult[ 0 ] = %d\n", fResult[ 0 ] );
	twoPair( fFace, fSuit, fResult, p);
	//fprintf(p,"\n  fResult[ 1 ] = %d\n", fResult[ 1 ] );
	threeKind( fFace, fSuit, fResult, p);
	//fprintf(p,"\n  fResult[ 2 ] = %d\n", fResult[ 2 ] );
	fourKind( fFace, fSuit, fResult, p);
	//fprintf(p,"\n  fResult[ 3 ] = %d\n", fResult[ 3 ] );
	flush( fFace, fSuit, fResult, p);
	//fprintf(p,"\n  fResult[ 4 ] = %d\n", fResult[ 4 ] );
	straight( fFace, fSuit, wFace, fResult, p);
	//fprintf(p,"\n  fResult[ 5 ] = %d\n", fResult[ 5 ] );

	fprintf(p,"\nSecond hand\n***********\n\n");
	pair( sFace, sSuit, sResult, p);
	//fprintf(p,"\n  sResult[ 0 ] = %d\n", sResult[ 0 ] );
	twoPair( sFace, sSuit, sResult, p);
	//fprintf(p,"\n  sResult[ 1 ] = %d\n", sResult[ 1 ] );
	threeKind( sFace, sSuit, sResult, p);
	//fprintf(p,"\n  sResult[ 2 ] = %d\n", sResult[ 2 ] );
	fourKind( sFace, sSuit, sResult, p);
	//fprintf(p,"\n  sResult[ 3 ] = %d\n", sResult[ 3 ] );
	flush( sFace, sSuit, sResult, p);
	//fprintf(p,"\n  sResult[ 4 ] = %d\n", sResult[ 4 ] );
	straight( sFace, sSuit, wFace, sResult, p);
	//fprintf(p,"\n  sResult[ 5 ] = %d\n", sResult[ 5 ] );

	eval(fResult, sResult, p, result, rindex);

}

//Displays the two hands
void display(const char *wfFace[], const char *wfSuit[], const char *wsFace[], const char *wsSuit[], FILE *p) {
	int i;

	fprintf(p,"*************************\nYour five-card poker hand\n*************************\n\nFirst hand %28s\n********** %28s\n\n", "Second hand", "***********");

	for(i = 0; i < 5; i++) {
		fprintf(p,"%10s of %-8s %10s of %-8s\n", wfFace[i], wfSuit[i], wsFace[i], wsSuit[i]);
	}
}

//Determine whether the hand contains a pair
void pair(const char *wfFace[], const char *wfSuit[], int result[], FILE *p) {
	int card;
	int i, j = 0, k;
	int pair[5] = {0};

	fprintf(p,"\nPair...\n");
	for(card = 0; card < 5; card++) {
		for(i = card + 1; i < 5; i++) {
			if(wfFace[card] == wfFace[i]) {
				j++;
				pair[card] = i;
			}
		}
	}

	if(j == 1) {
		for(card = 0; card < 5; card++) {
			if(pair[card] != 0) {
				k = pair[card];
				fprintf(p,"\n%10s of %-8s and %5s of %-8s\n", wfFace[card], wfSuit[card], wfFace[k], wfSuit[k]);
				result[0] = 1;
			}
		}
	} else {
		fprintf(p,"\n Your five-hand card does not contain any pair. \n");
	}
}

//Determine whether the hand contains two pairs
void twoPair(const char *wfFace[], const char *wfSuit[], int result[], FILE *p) {
    int card;
	int i, j = 0, k;
    int pair[5] = {0};

    //compute whether the five-hand card contains two pair
    fprintf(p,"\nTwo-pair...\n");
    for(card = 0; card < 5; card++) {
        for(i = card + 1; i < 5; i++) {
           if(wfFace[card] == wfFace[i]){
             j++;
             pair[card] = i;
           }
        }
    }

    //display result
    if(j == 2) {
       for(card = 0; card < 5; card++) {
          if(pair[card] != 0) {
             k = pair[card];
             fprintf(p,"\n%10s of %-8s and %5s of %-8s\n", wfFace[card], wfSuit[card], wfFace[k], wfSuit[k]);
          }
       }
    } else {
       fprintf(p,"\n Your five-hand card does not contains two pair.\n");
    }
}

//Determine whether the hand contains three of a kind
void threeKind(const char *wfFace[], const char *wfSuit[], int result[], FILE *p) {
	int card;
	int i, j = 0, k, x;
	int pair[3] = {0};

	fprintf(p,"\nThree of a kind...\n");
	    for(card = 0; card < 3; card++) {
	        for(i = card + 1; i < 5; i++) {
	           if(wfFace[card] == wfFace[i]) {
	              for(x = i + 1; x < 5; x++){
	                 if(wfFace[i] == wfFace[x]) {
	                    j++;
	                    pair[0] = card;
	                    pair[1] = i;
	                    pair[2] = x;
	                 }
	              }
	           }
	        }
	    }

	if(j == 1) {
	       for(card = 0; card < 3; card++) {
	             k = pair[card];
	             fprintf(p,"\n%10s of %-8s\n", wfFace[k], wfSuit[k]);
	       }
	    } else {
	       fprintf(p,"\n  Your five-hand card doesn't contains three of a kind.\n");
	    }
}

//Determine whether the hand contains four of a kind
void fourKind(const char *wfFace[], const char *wfSuit[], int result[], FILE *p) {
	int card;
	int i, index = 0;
	int pair[5] = {0};

	fprintf(p,"\nFour of a kind...\n");
	for(card = 0; card < 2; ++card) {
		for(i = card + 1; i < 5; i++) {
				if(wfFace[card] == wfFace[i]) {
					index++;
					if(index == 1)
						pair[i - 1] = 1;
					pair[i] = 1;
				}
		}
		if(index == 3)
			break;
		else if(index > 3)
			break;
		else {
			index = 0;
			for(i = 0; i < 5; i++)
				pair[i] = 0;
		}
	}

	if(index == 3) {
		for(i = 0; i < 5; i++) {
			if(pair[1] == 1)
				fprintf(p,"\n%5s of %-9s\n", wfFace[i], wfSuit[i]);
		}
		fprintf(p,"\n");
	} else {
		fprintf(p,"\n  Your five-hand card doesn't contains four-of-a-kind.\n");
	}
}

//Determines if the hand is a flush
void flush (const char *wfFace[], const char *wfSuit[], int result[], FILE *p) {
   int i, j = 0;

   //compute whether your five-hand card contains a flush
   fprintf(p,"\nFlush...\n");
   for(i = 0; i < 5; i++) {
      if(wfSuit[0] == wfSuit[i])
         j++;
   }

   if(j == 4) {
	   result[4] = 1;
	   for(i = 0; i < 5; i++) {
		   fprintf(p,"\n %5s of %-8s\n", wfFace[i], wfSuit[i]);
	   }
   }
   else {
	   fprintf(p,"\n Not flush\n");
   }
}

//Determines if the hand is a straight
void straight(const char *wfFace[], const char *wfSuit[], const char *wFace[], int result[], FILE *p) {
   int i, j, k;
   int pass, count, hold, check = 1;
   int faceValue[5] = {0};

   fprintf(p,"\nStraight...\n");
   //locate face value and store in an array
   for(i = 0 ; i < 5; i++) {
      for(j = 0 ; j < 13; j++) {
         if(wfFace[i] == wFace[ j ]) {
            faceValue[i] = j;
         }
      }
   }

   //sort face value in ascending order using bubble sort
   for(pass = 0; pass < 4; pass++) {
      for(count = 0; count < 4; count++){
         if(faceValue[ count ] > faceValue[ count + 1 ]) {
            //swap
            hold = faceValue[ count ];
            faceValue[count] = faceValue[count + 1];
            faceValue[count + 1] = hold;
         }
      }
   }

   //check if the hand contains a straight
   for(i = 0; i < 4; i++) {
      if(faceValue[i] + 1 != faceValue[i + 1]) {
         check = 0;
      }
   }

   if(check == 1) {
	   result[5] = 1;
	   for(i = 0; i < 5; i++) {
		   fprintf(p,"\n %5s of %-8s\n", wfFace[i], wfSuit[i]);
	   }
   }
   else {
	   fprintf(p,"\n Not a straight\n");
   }

}

//Evaluates the hands to determine which is the better hand
void eval(const int firstResult[], const int secondResult[], FILE *p, char *result[], unsigned int rindex) {
	int i, x = 0, y = 0;
	fprintf(p,"\n******\nResult\n******\n\n");

	for(i = 0; i < 6; ++i) {
		if(firstResult[i] == 1)
			x = i + 1;
		if(secondResult[i] == 1)
			y = i + 1;
	}

	fprintf(p,"x = %d  y = %5d\n\n", x, y);
	if(x > y){
		fprintf(p, "First hand wins\n\n");
		result[rindex] = "First hand wins";
	}
	else if(y > x){
		fprintf(p, "Second hand wins\n\n");
		result[rindex] = "Second hand wins";
	}
	else if(x == y) {
		fprintf(p, "Tie\n\n");
		result[rindex] = "Tie";
	}
	else {
		fprintf(p, "Error, could not evaluate\n\n");
		result[rindex] = "Error, could not evaluate";
	}
}

