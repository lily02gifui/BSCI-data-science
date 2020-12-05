/* Assignment2 
 * A program to process credit card and transaction records and 
 * identify fraudulent transactions.
 *
 * Written by Aoi Fujii in May 2020
 *
 * reference:
 * listop.c: 
 * https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c
 * bsearch: 
 * https://www.tutorialspoint.com/c_standard_library/c_function_bsearch.htm
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>


#define CARD_ID_LENGTH 8           // credit card ID length 
#define TRANS_ID_LENGTH 12         // transaction ID length 
#define MAX_NUM_CARDS 100          // maximum number of credit cards 
#define END_OF_CREDIT "%%%%%%%%%%" // line that shows the end of credit records 
// stage header format string
#define HEADER "=========================Stage %d=========================\n"
// stage numbers 
#define STAGE_NUM_ONE 1
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define FIRST_RECORD 0
#define REST_OF_LINE 2
#define READ 1
#define DONE 0
#define BEFORE -1
#define AFTER 1
#define SAME 0
#define IN_BOTH_LIMITS "IN_BOTH_LIMITS"
#define OVER_DAILY_LIMIT "OVER_DAILY_LIMIT"
#define OVER_TRANS_LIMIT "OVER_TRANS_LIMIT"
#define OVER_BOTH_LIMITS "OVER_BOTH_LIMITS"


typedef char cid_t[CARD_ID_LENGTH+1];   // credit card id
typedef char tid_t[TRANS_ID_LENGTH+1];  // transaction id


// Holds a date in yyyy, mm, dd format 
typedef struct{
    int yyyy, mm, dd;
} date_t;

// Holds a date and time 
typedef struct{
    date_t date; 
    int hh, mm, ss;
} datetime_t;

// Holds total amout of daily transaction and its date 
typedef struct{
    int daytotal;
    date_t date;
} total_t;

// stage1: design a struct to represent a credit card record
// Holds all components of a credit card record and total daily transaction 
typedef struct {
    cid_t card_id;
    int daily_limit;
    int transaction_limit;
    total_t total;
} card_t;

// stage3: design a struct to represent a transaction
typedef struct {
    tid_t transid;
    cid_t cardid;
    datetime_t datetime;
    int amount;
} trans_t;

// linked structure for stage3: refer to listop.c
typedef struct node node_t;

// linked structure for stage3: refer to listop.c
struct node {
	trans_t data;
	node_t *next;
};

// linked structure for stage3: refer to listop.c
typedef struct {
	node_t *head;
	node_t *foot;
} list_t;


/****************************************************************/


// function prototypes 
void print_stage_header(int stage_num);
int read_one_credit(card_t cards[], int *num_cards);
void print_one_credit(card_t one_card);
list_t *make_empty_list(void);
int is_empty_list(list_t *list);
void free_list(list_t *list);
list_t *insert_at_foot(list_t *list, trans_t value);
list_t *readin(list_t *list);
void printout(list_t *list);
card_t* findcard(card_t *key, int num_cards, card_t cards[]);
int datacmp(const void *a, const void *b);
int checkdate(date_t date1, date_t date2);
void fraudchecker(trans_t transdata, card_t *card);
void checklim(card_t *card, trans_t transdata);
void checktransaction(list_t *list, int num_cards, card_t cards[]);
void stage_one(card_t cards[], int *num_cards);
void stage_two(card_t cards[], int *num_cards);
list_t *stage_three(void);
void stage_four(list_t *list, int num_cards, card_t cards[]);


/****************************************************************/


int
main(int argc, char *argv[]) { 
    
    // to hold all input card infomation 
    card_t cards[MAX_NUM_CARDS];
    
    // to hold the number of input card infomation 
    int num_cards = 0;
    
    // stage 1: reading one credit card record 
	stage_one(cards, &num_cards); 
	
	// stage 2: reading all credit card records 
	stage_two(cards, &num_cards);
	
	// stage 3:  reading the transactions into the linked list 
	list_t* list = stage_three();
	
	// stage 4: reading the dictionary and finding the longest emoticon
    // also free the linked list
	stage_four(list, num_cards, cards);
	
	return 0;
}


// print stage header given stage number 
void 
print_stage_header(int stage_num) {
    printf(HEADER, stage_num);
}


/****************************************************************/


/* stage 1: reading one credit card record */

// read in a credit card record from the input data, 
//print the record and store information in cards[]
int
read_one_credit(card_t cards[], int *num_cards){
    // holds the rest of the separateline (indicates the end of credit record)
    char rest[REST_OF_LINE+1];
    card_t one_card;
    
    if (scanf("%8s %d %d\n", one_card.card_id, &(one_card.daily_limit),
        &(one_card.transaction_limit)) == 3) {
        if (*num_cards+1 > MAX_NUM_CARDS){
            fprintf(stderr, "More than maximum number of cards detected.\n");
            exit(EXIT_FAILURE);
        }
        else {
            // intitalize total daily transaction for stage4
            one_card.total.daytotal = 0000;
            one_card.total.date.yyyy = 00;
            one_card.total.date.mm = 00;
            one_card.total.date.dd = 00;
            // store card infomation
            cards[*num_cards] = one_card;
            *num_cards += 1;
            // successfully read one record
            return READ;
        }
    }
    
    // check if its the end of credit card records
    else if (scanf("%2s\n", rest) && strcmp(strcat(one_card.card_id, rest),
        END_OF_CREDIT) != 0){
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE); 
    }
    // end of the credit record
    return DONE;
}


// print the credit card record
void
print_one_credit(card_t one_card){
    printf("Card ID: %s\n", one_card.card_id);
    printf("Daily limit: %d\n", one_card.daily_limit);
    printf("Transaction limit: %d\n", one_card.transaction_limit);
    printf("\n");
}


// stage 1: reading one credit card record
void 
stage_one(card_t cards[], int *num_cards) {
    int total_cards = *num_cards; 
    
	// print stage header 
	print_stage_header(STAGE_NUM_ONE);
	
	// read the first credit card info 
    read_one_credit(cards, &total_cards);
    *num_cards = total_cards;
	
	// print the credit card infomation, -1 to get the index
	print_one_credit(cards[*num_cards-1]);
}


/**************************************************************************/


/* stage 2: reading all credit card records */
void
stage_two(card_t cards[], int *num_cards){
    int total_cards = *num_cards;
    // first record is already read
    // keep track of total daily limit and maximum transaction limit
    float total_daily_lim = cards[FIRST_RECORD].daily_limit;
    int max_trans_lim = cards[FIRST_RECORD].transaction_limit;
    float avg_daily_lim;
    // store the index in cards[] with maximum transaction
    int max_trans_i = 0;
    
    // print stage header 
    print_stage_header(STAGE_NUM_TWO);
    
	// continue to read the credit card records 
    while(read_one_credit(cards, &total_cards)){
        total_daily_lim += cards[total_cards-1].daily_limit;
        
        // find the maximum transaction limit 
        if ((cards[total_cards-1].transaction_limit) > max_trans_lim){
            // assume that the card is in the order of smallest to largest id 
            max_trans_lim = cards[total_cards-1].transaction_limit;
            max_trans_i = (total_cards - 1);
        }
    }
    
    *num_cards = total_cards;
    // calculate the avarage daily limit 
    avg_daily_lim = (total_daily_lim / *num_cards);
    
    // print the number of credit card records, 
    // max daily transaction limit and the credit card id
    printf("Number of credit cards: %d\n", *num_cards);
    printf("Average daily limit: %.2f\n", avg_daily_lim);
    printf("Card with the largest transaction limit: %s\n",
            cards[max_trans_i].card_id);
    printf("\n");
}


/**************************************************************************/


/* Stage 3: reading the transaction */ 

// create an empty list: adopted from listop.c
list_t*
make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}


// check if the list is empty: adopted from listop.c
int
is_empty_list(list_t *list) {
	assert(list!=NULL);
	return list->head==NULL;
}


// free the linked list: adopted from listop.c
void
free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
    
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
    
	free(list);
}


// insert record at the end of the list: adopted form listop.c
list_t*
insert_at_foot(list_t *list, trans_t value) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;
    
	if (list->foot == NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	}
    else {
		list->foot->next = new;
		list->foot = new;
	}
    
	return list;
}


// read in the transactions,store them in a linked data structure 
list_t*
readin(list_t *list) {
    trans_t trans;
    
    // get the values
	while (scanf("%12s %8s %4d:%2d:%2d:%2d:%2d:%2d %d\n", trans.transid, 
        trans.cardid, &(trans.datetime.date.yyyy), &(trans.datetime.date.mm), 
        &(trans.datetime.date.dd), &(trans.datetime.hh), &(trans.datetime.mm),
        &(trans.datetime.ss), &(trans.amount)) == 9) {
        list = insert_at_foot(list, trans);
	}
    
    return list;
}


// output the id
void 
printout(list_t *list) {
	node_t *new;
	new = list->head;
    
	while(new != NULL) {
		printf("%s\n", new->data.transid);
		new = new->next;
	}
}  


// stage 3: reading the transaction and return the linked list
list_t* 
stage_three(void) { 
    
	// print stage header 
	print_stage_header(STAGE_NUM_THREE);
    
    // create an empty list 
	list_t *list = make_empty_list();
    
	// read the info, store them in the linked structure 
    list = readin(list);
    
    // print the ids in the list 
    printout(list);
    
    printf("\n");
    return list;
}


/*************************************************************************/


/* Stage4:  checking for fraudulent transactions */

// check whether the card id is the same. refere to: 
// https://www.tutorialspoint.com/c_standard_library/c_function_bsearch.htm
int 
datacmp(const void *a, const void *b) {
    return strcmp(((card_t *)a)->card_id, ((card_t *)b)->card_id);
}    


// find the credit card information that matched the credit card id using
// binary search from the credit card records read in Stage 2
card_t*
findcard(card_t *key, int num_cards, card_t cards[]){
    card_t *card = (card_t *)bsearch(key, cards, num_cards,
                                     sizeof(card_t), datacmp);
 
    return card;
}


// check the date difference, returns -1 if date1 is befpre date2,
// 1 if date1 is after date2, 0 if they are the same.
int
checkdate(date_t date1, date_t date2){
    if (date1.yyyy < date2.yyyy){
        return BEFORE;
    }
    else if (date1.yyyy > date2.yyyy){
        return AFTER;
    }
    else {
        if (date1.mm < date2.mm){
            return BEFORE;
        }
        else if (date1.mm > date2.mm){
            return AFTER;
        }
        else {
            if (date1.dd < date2.dd){
                return BEFORE;
            }
            else if (date1.dd > date2.dd){
                return AFTER;
            }
            else {
                return SAME;
            }
        }
    }
}



// chek the fraud and print the output
void
fraudchecker(trans_t transdata, card_t *card){
    // check the daily lim and trans lim
    int daily_lim = card->daily_limit;
    int trans_lim = card->transaction_limit;

    // output the transaction id
    printf("%s             ", transdata.transid);
        
    //detecting the fraud transaction
    // over both limits
    if ((transdata.amount > trans_lim) && (card->total.daytotal > daily_lim)){
        printf("%s\n", OVER_BOTH_LIMITS);
    }
    // over daily limit
    else if (card->total.daytotal > daily_lim){
        printf("%s\n", OVER_DAILY_LIMIT);
    }
    // over trans limit
    else if (transdata.amount > trans_lim){
        printf("%s\n", OVER_TRANS_LIMIT);
    }
    // in both limits
    else {
        printf("%s\n", IN_BOTH_LIMITS);
    }
}

// check the daili limit and transaction limit of the card
// detect the fraud and print the output
void
checklim(card_t *card, trans_t transdata){
    // found the card info from the record, check total transaction in the day
    if (card != NULL) {
        // if the date the amount is calculated is before the transaction date,
        // update the total transaction (amount and date)
        // assume that trnsaction is sorted by date and time as spec says
        if (checkdate(card->total.date, transdata.datetime.date) < 0){
            card->total.daytotal = 0;
            card->total.date = transdata.datetime.date;
        }
        
        // calculate the total transaction on that date
        card->total.daytotal += transdata.amount;
        
        // detect the fraud, print the output
        fraudchecker(transdata, card); 
    }
    
    // matching record not found
    else {
        printf("Credit card ID was not found in the records.\n");
        exit(EXIT_FAILURE);
    }   
}


// check the transaction in the linked list
void 
checktransaction(list_t *list, int num_cards, card_t cards[]) {
    // check each node in the linked list
    node_t *new;
    new = list->head;
    // created to serve as the key in the binary search
    card_t tran_card;
    
	while (new != NULL) {
        // store the card id in the search key
        strcpy(tran_card.card_id, new->data.cardid);
        
        // check the credit card info by a binary search
        card_t* card = findcard(&tran_card, num_cards, cards);
        
        // check the daily lim and trans lim, the total trans in the day
        // and detect the fraud
        checklim(card, new->data);
        
        // check the next transaction record
		new = new->next;
	}
}  


// stage4:  checking for fraudulent transactions
void 
stage_four(list_t *list, int num_cards, card_t cards[]) { 
    
	// print stage header 
	print_stage_header(STAGE_NUM_FOUR);
    
	// read the linked list, check for fraudulent transaction
    checktransaction(list, num_cards, cards);
    
    // free the linked list
    free_list(list); 
}




/*******************************************************************/
// The average time complexity of this algorithm is O(nlogm) 
// where n is the number of transactions and m is the number of credit card
// records.
// This algorithm goes throught the linked list which stores n transaction 
// records. Thus the time complexity for this is O(n).
// As it goes throught the list, it search the credit card records by a binary
// search with O(logm) time complexity.
// If it finds the matching credit card id in the record, its total daily 
// transaction and the date of the calculated total transaction is compared
// with the transaction record to detect the fraudlent transaction.
// Therefore, the overall average time complexity is O(n*logm).
//

