/* Assignment1 Foundations of Algorithm
 * Written by Aoi Fujii in May 2020
 */

/* Emoticon message cleanser:
 * Skeleton code written by Farhana Choudhury and Jianzhong Qi, April 2020
 * Further developed by Aoi Fujii, May 2020
 */

/* Reference
 * C library function - strlen(): 
 * https://www.tutorialspoint.com/c_standard_library/c_function_strlen.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <string.h>

#define STAGE_NUM_ONE 1							/* stage numbers */ 
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_NUM_FIVE 5
#define STAGE_HEADER "Stage %d\n==========\n"	/* stage header format string */

#define MAX_MSG_LENGTH 280						/* maximum message length */
#define MAX_NUM_MSGS 100						/* maximum number of messages */
#define MAX_EMTCN_LENGTH 50						/* maximum emot. line length */
#define MAX_NUM_EMTCN 50						/* maximum number of emot. */

#define SEPARATE_LINE 3
#define TRUE 1
#define FALSE 0


typedef char msg_t[MAX_MSG_LENGTH+1];			/* a message */
typedef char emtcn_t[MAX_EMTCN_LENGTH+1];		/* an emoticon */


typedef struct {

    int length;
    emtcn_t emtcn;

} sorting_t;

/****************************************************************/

/* function prototypes */
void read_one_msg(msg_t one_msg, int max_len);
void print_stage_header(int stage_num);
int count_tokens(msg_t one_msg);

void stage_one(msg_t one_msg);
void stage_two(msg_t msgs[], int *num_msgs);
void stage_three(msg_t msgs[], int num_msgs);
void stage_four(emtcn_t emtcns[], int *num_emtcns);
void stage_five(msg_t msgs[], int num_msgs, emtcn_t emtcns[], int num_emtcns);

void cleanse_one_msg(msg_t one_msg);
void cleanse_msgs(msg_t msgs[], int *num_msgs);

void remove_comma(msg_t msgs[], int num_msgs);
void print2d_arr(msg_t new_msgs[], int num_msgs);

void get_one_emtcn(msg_t one_line, int *len, emtcn_t one_emtcn);
void get_all_emtcns(sorting_t emtcn_records[], int *num_emtcns);
void print_longest(sorting_t emtcn_records[], int num_emtcns);

void remove_first_i(msg_t one_line, int index);
int emtcn_matching(msg_t one_line, emtcn_t emtcns[], int num_emtcns,
                   msg_t new_line, int new_i);

/****************************************************************/

/* main function controls all the action, do NOT modify this function */
int
main(int argc, char *argv[]) {
	/* to hold all input messages */
	msg_t msgs[MAX_NUM_MSGS];	
	/* to hold the number of input messages */
	int num_msgs = 0;	
	/* to hold all input emoticons in the dictionary */
	emtcn_t emtcns[MAX_NUM_EMTCN];
	/* to hold the number of emoticons in the dictionary */
	int num_emtcns = 0;	
	
	/* stage 1: reading the first message */
	stage_one(msgs[num_msgs]); 
	num_msgs++;
	
	/* stage 2: removing alphanumeric characters */
	stage_two(msgs, &num_msgs);
	
	/* stage 3: removing extra commas */ 
	stage_three(msgs, num_msgs);
	
	/* stage 4: reading the dictionary and finding the longest emoticon */
	stage_four(emtcns, &num_emtcns);
	
	/* stage 5: removing invalid emoticons with the help of the dictionary */
	stage_five(msgs, num_msgs, emtcns, num_emtcns);
	
	/* all done; take some rest */
	return 0;
}

/* read a line of input into one_msg */
void
read_one_msg(msg_t one_msg, int max_len) {
	int i = 0, c;
	while (((c = getchar()) != EOF) && (c != '\n') && (c != '\r')) {
		if (i < max_len) {
			one_msg[i++] = c;
		} else {
			printf("Invalid input line, toooooooo long.\n");
			exit(EXIT_FAILURE);
		}
	}
	one_msg[i] = '\0';
}

/* print stage header given stage number */
void 
print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}


/****************************************************************/


/* scan a message and count the number of tokens (separated by commas) */
int 
count_tokens(msg_t one_msg) {
    int i;
    /* count the first token if the massage is non-empty */
    int num_tokens = (one_msg[0] != '\0');
    for (i=0; one_msg[i]!='\0'; i++){
        if (one_msg[i] == ','){
            num_tokens += 1;
        }
    }
	return num_tokens;
}


/* stage 1: reading the first message */
void 
stage_one(msg_t one_msg) {   
	/* print stage header */
	print_stage_header(STAGE_NUM_ONE);
	
	/* read the first message */
    read_one_msg(one_msg, MAX_MSG_LENGTH+1);
	
	/* count and print the number of tokens */
	printf("Number of tokens: %d\n\n", count_tokens(one_msg));
}


/********************************************************/


/* remove all english letters and digits from the input message */
void
cleanse_one_msg(msg_t one_msg){
    int i, new_i=0, found_alnum;
    // empty message is invalid
    if (one_msg[0] == '\0'){
        printf("Invalid input. no message contained.\n");
        exit(EXIT_FAILURE);
    }
    for (i=0; one_msg[i]!='\0'; i++) {
        found_alnum = isalnum(one_msg[i]);
        if (! found_alnum) {
            one_msg[new_i] = one_msg[i];
            new_i++;
        }
    }
    one_msg[new_i] = '\0';
}


/* read and cleanse each line from the input until the separating line */
void
cleanse_msgs(msg_t msgs[], int *num_msgs){
    // store separating line
    char str_separate[SEPARATE_LINE+1];
    strcpy(str_separate, "###");
    
    msg_t one_msg;
    // get the first message
    *num_msgs = 0;
    strcpy(one_msg, msgs[*num_msgs]);
    
    // cleanse the message until the separating line is found
    while(strcmp(one_msg, str_separate)){
        // cleanse each message and print
        cleanse_one_msg(one_msg);
        printf("%s\n", one_msg);
        
        // invalid if you get too many lines
        if (*num_msgs > MAX_NUM_MSGS){
            printf("Invalid. more than 100 messages are detected.\n");
            exit(EXIT_FAILURE);
        }
        // store the cleansed message
        strcpy(msgs[*num_msgs], one_msg);
        *num_msgs += 1;
        read_one_msg(one_msg, MAX_MSG_LENGTH);
        }
}

/* stage 2: removing alphanumeric characters */
void 
stage_two(msg_t msgs[], int *num_msgs) {
    int all_msgs;
    
	/* print stage header */
	print_stage_header(STAGE_NUM_TWO);
	
    /* print cleansed messages*/
    cleanse_msgs(msgs, &all_msgs);
    *num_msgs = all_msgs;
	    
	printf("\n");  
}


/*********************************************************/


/* message length and the number of messages of the input msgs[]
 * are within the limit (it was checked in cleanse_msgs when creating msgs[]) 
 */

/* remove unnecessary commas from the message in msgs */
void
remove_comma(msg_t msgs[], int num_msgs){
    int i, j, new_j=0, msg_length, in_token;
    char *new_msg;
    
    for (i=0; i<num_msgs; i++){
        // include the space for special char \0 for msg length
        msg_length = strlen(msgs[i]) + 1;
        
        new_msg = (char *) malloc(msg_length);
        if (!new_msg){
            printf("Memory allocation for mew_msg failed.\n");
            exit(EXIT_FAILURE);
        }
        
        in_token = FALSE; 
        for (j=0; j < msg_length; j++){
            // reach the end of msg
            if (msgs[i][j] == '\0'){
                // remove the comma in the end if there's any
                if(msgs[i][j-1] == ','){
                    new_msg[new_j-1] = '\0';
                    break;
                }
                new_msg[new_j] = '\0';
                break;
            }
            //store the comma right after a token
            else if (in_token && msgs[i][j] == ','){
                in_token = FALSE;
                new_msg[new_j++] = ',';
            }
            // skipping the comma
            else if (msgs[i][j] != ','){
                // the first token is found
                if (! in_token){
                    in_token = TRUE;
                }
                new_msg[new_j++] = msgs[i][j];
            }
        }
        
        strcpy(msgs[i], new_msg);
        free(new_msg);
        // assigning new index for the next line
        new_j = 0;
    }
}

/* print 2d arrray */
void
print2d_arr(msg_t msgs[], int num_msgs){
    int i;
    for (i=0; i< num_msgs; i++){
        printf("%s\n", msgs[i]);
    }   
}


/* stage 3: removing leading, trailing and consequtive commas */ 
void 
stage_three(msg_t msgs[], int num_msgs) {
    /* remove unnecessary commas */
    remove_comma(msgs, num_msgs);
    
	/* print stage header */
	print_stage_header(STAGE_NUM_THREE);
    
    /* print messages */
    print2d_arr(msgs, num_msgs);
    
	printf("\n");
}


/*************************************************************************/


/*spec says: each line in the dictionary contains a single emoticon 
 * followed by a text that represents the corresponding emotion. 
 * so assume that there is no line without emoticon
 */
 
/* read a line of dictionary. get the emoticon and count its length */
void
get_one_emtcn(msg_t one_line, int *len, emtcn_t one_emtcn){ 
    int i = 0;
    
    // get the emoticon before a comma
    for (i=0; one_line[i] != ','; i++){
        // the line finished without a comma
        if (one_line[i] == '\0'){
            break;
        }
        // the line should be at most the max length(i.e. 50)
        else if (i <= MAX_EMTCN_LENGTH) {
            one_emtcn[i] = one_line[i];
        } 
        else {
            printf("Invalid emoticon. The line is too long.\n");
            exit(EXIT_FAILURE);
        }
    }
    one_emtcn[i] = '\0';
    
    *len = i;
}


/* store the emoticon and its length to the array, emtcns.
 * count the total number of emoticons.
 */  
void
get_all_emtcns(sorting_t emtcn_records[], int *num_emtcns){
    // count the total number of emoticon
    *num_emtcns = 0;
    //store one line
    emtcn_t one_line;
    
    // get the first line of the dictionary
    read_one_msg(one_line, MAX_EMTCN_LENGTH);
    
    // store length of a emoticon
    int len;
    char* one_emtcn;
    
    // read each line
     while(one_line[0] != '\0'){
        len = 0;
        
        one_emtcn = (char *) malloc(MAX_EMTCN_LENGTH+1);
        if (!one_emtcn){
            printf("Memory allocation for one_emtcn failed.\n");
            exit(EXIT_FAILURE);
        }
        
        // get emoticon and its length, store the data in emtcn_records
        get_one_emtcn(one_line, &len, one_emtcn);
        
        // store the emoticon and its length
        strcpy(emtcn_records[*num_emtcns].emtcn, one_emtcn);
        emtcn_records[*num_emtcns].length = len;
        *num_emtcns += 1;
        
        free(one_emtcn);
        
        // too many lines
        if (*num_emtcns > MAX_NUM_EMTCN){
            printf("Invalid. more than 50 lines are detected.\n");
            exit(EXIT_FAILURE);
        }
        
        // get the next line
        read_one_msg(one_line, MAX_EMTCN_LENGTH);
        }
}


/* find the longest emoticon (the earliest that appear in the dict if tie) */
void
print_longest(sorting_t emtcn_records[], int num_emtcns){
    int i;
    sorting_t max_record = emtcn_records[0];
    
    if (num_emtcns == 0){
        printf("Invalid input. No emoticon found in a dictionary.\n");
        exit(EXIT_FAILURE);
    }
    
    for (i=1; i<num_emtcns; i++){
        // store the longest emoticon
        if (emtcn_records[i].length > max_record.length){
             max_record = emtcn_records[i];
        }
    }
    // print the emoticon and its length
    printf("Longest: %s\n", max_record.emtcn);
    printf("Length: %d\n", max_record.length);
}

  
/* stage 4: reading the dictionary and finding the longest emoticon */ 
void 
stage_four(emtcn_t emtcns[], int *num_emtcns) {
    sorting_t emtcn_records[MAX_NUM_EMTCN];
    int total_emtcns = 0, i;
    
    /* print stage header */
    print_stage_header(STAGE_NUM_FOUR);
    
    /* extract all emoticons */
    get_all_emtcns(emtcn_records, &total_emtcns);
    *num_emtcns = total_emtcns;
    
    /* print the total number of emoticons */
    printf("Emoticon total: %d\n", *num_emtcns);
    
    /* print the longest emoticon and its length */
    print_longest(emtcn_records, *num_emtcns);
    
    /* store emoticon in mntcn[] */
    for (i=0; i<*num_emtcns; i++){
        strcpy(emtcns[i], emtcn_records[i].emtcn);
    }
    
    printf("\n");
}


/**************************************************************************/


/* remove first ith vharacter from the input array */
void
remove_first_i(msg_t one_line, int index){
    int i;
    for (i=index; one_line[i]!='\0';i++){
        one_line[i-index] = one_line[i];
    }
    one_line[i-index] = '\0';
}


/* Remove invalid emoticons from the message in one_line. 
 * Store the cleansed message in new_line 
 */
int
emtcn_matching(msg_t one_line, emtcn_t emtcns[], int num_emtcns,
               msg_t new_line, int new_i){
    int len, different, i, j, k;
    emtcn_t one_emtcn;
    //check each emoticons in msgs (separated by ",")
    get_one_emtcn(one_line, &len, one_emtcn);
    
    // if the one_emtcn is empty, remove first char(comma which 
    // was not read in get_one_emtcn) from one_line and read the line again
    while(one_line[0] != '\0' && one_emtcn[0] == '\0'){
        new_line[new_i] = one_line[0];
        new_i++;
        remove_first_i(one_line, 1);
        get_one_emtcn(one_line, &len, one_emtcn);
    }
    
    // if one_line is empty, end the function
    if (one_line[0] == '\0'){
        new_line[new_i] = '\0';
        return 0;
    }
    
    // check the whether the emoticon is in the dictionary
    for (i=0; i<num_emtcns; i++){
        // use "different" as a flag to show the matching
        different = FALSE;
        for (j=0; emtcns[i][j]!='\0'; j++){
            if (emtcns[i][j]!=one_emtcn[j]){
                different = TRUE;
                break;
            }       
        }
        // the emoticon in a message is found in the dictionary
        // avoid matching emoticon in the dict that include one_emtcn in itself
        if ((!different) && (j==len)){ 
            for (k=0; one_emtcn[k]!='\0'; k++){
                new_line[new_i] = one_emtcn[k];
                new_i++;
            }
        }
    }
    // remove the emoticon and find the next one
    remove_first_i(one_line, len);
    return emtcn_matching(one_line, emtcns, num_emtcns, new_line, new_i);  
}


/* stage 5: removing invalid emoticons with the help of the dictionary */
void 
stage_five(msg_t msgs[], int num_msgs, emtcn_t emtcns[], int num_emtcns) {
    int i,  new_i, j, skip=0;
    msg_t new_line;
   
    /* print stage header */
    print_stage_header(STAGE_NUM_FIVE);    
    
    /* check the emoticons line by line */
    for(i=0; i<num_msgs; i++){
        new_i = 0;
        new_line[0] = '\0';
        emtcn_matching(msgs[i], emtcns, num_emtcns, new_line, new_i);
         
        /* copy cleansed message back in to msgs */
        if (new_line[0] != '\0'){
            for(j=0; new_line[j]!='\0'; j++){
                msgs[i-skip][j] = new_line[j];
            }  
            msgs[i-skip][j] = '\0';
        }
        // skip the line if the cleansed message is empty
        else{
            skip++;
        }
    }
    
    print2d_arr(msgs, num_msgs-skip);
}