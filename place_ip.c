/// File: place_ip.c
/// @author Andrew   
/// @date 11/20/2022
/// /// /// /// /// /// /// /// 

// used include and define statements 
#define _DEFAULT_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include "trie.h"
#define TRIE_H

/// defines the node structure
/// NOTE MY PROFESSOR TOLD ME TO PUT IT IN THIS 
/// FILE 
typedef struct Node_s{
    struct Node_s *left;
    struct Entry_s *entry;
    struct Node_s *right;
}*Node;
/// AND THIS TYPEDEF
typedef Node * NodeH;
/// ALONG WITH THIS STRUCTURE 
struct Trie_s{
    int tree_height;
    int tree_size;
    int node_count;
    int node_test;
    int count;
    int null_node;
    Node root;
};


int start = 1;

/// gets the next value from the entry 
///
/// @param line, current line of the file 
/// @param index, current index of the line 
/// @param first, is it the first part of the entry? 
/// @return string that is the value from the entry 
char* get_value(char* line, char *value, int index, bool first){
    
    if(first){
        if(line[0] == '"'){
            while(line[start] != '"'){
                value[index] = line[start];
                start++;
                index++;
            }
        }
    }else{
        if(line[start] == '"'){
            start++;
            while(line[start] != '"'){
                value[index] = line[start];
                start++;
                index++;
            }
        }
    }


    return value;
}

//this function like the one above, gets the bit
//but at a certain postion. 
/// 
/// @param ui, unsigned int to get 
/// @param pos, position of bit
/// @return 0 or 1 
int kth_bitt(unsigned int ui, int pos){
    int bit; 
    bit = (ui >> (32 - pos - 1)) & 1;

    return bit; 
}

///prints the start of the query
void print_query(){
    printf("Enter an IPV4 string or a number (or a blank line to quit).\n");
}

/// handles the query 
/// 
/// @param trie, the trie that is going to be used throughtout its entirety
void query(Trie trie){
    bool on = true;
    while(on){
        char str[20];
        printf("> ");
        scanf("%19s", str);
        if(str == NULL)break;
        if(strcmp(str, "quit") == 0) break;
        if(strcmp(str, "q") == 0)break;
        if(strcmp(str, "\n") == 0)break;

        while(str != NULL && str[0] != 'q' && str[0] != '\n'){

            //classifying the input
            int digit_count = 0;
            int period_count = 0;
            int index = 0;
            while(str[index] != '\0'){
                //is the char a digit? 
                if(isdigit(str[index]) > 0){
                    digit_count++;
                }
                //is the char a period? '.'
                else if(str[index] == '.'){
                    period_count++;
                }
                index++;
            }
            
            //handling the input based on class
            //input was a normal/full from/to IPV4 range value
            if(period_count == 0){
                Entry s = NULL;//(Entry) calloc(1, sizeof(struct Entry_s));
                ikey_t entry_key = (unsigned int)atoi(str);
                s = ibt_search(trie, entry_key);
                //if the key is the same as the from value
                if(s->key == (unsigned int) atoi (s->from_value)){
                    printf("%d: (%d.%d.%d.%d, %s, %s, %s, %s)\n", 
                    s->key, s->from_bytes[3], s->from_bytes[2],
                    s->from_bytes[1], s->from_bytes[0], 
                    s->country_code, s->country_name, s->city, 
                    s->province);
                }//if the key is the same as the to value
                else{
                    printf("%d: (%d.%d.%d.%d, %s, %s, %s, %s)\n", 
                    s->key, s->to_bytes[3], s->to_bytes[2],
                    s->to_bytes[1], s->to_bytes[0], 
                    s->country_code, s->country_name, s->city, 
                    s->province); 
                }
                s = NULL; 
            }//if there is a period in the input it means ip format 
            else{
                int arr[4];
                

                char *tok;
                tok = strtok(str, ".");
        
                int i = 0;
                while(tok != NULL){ 
                    arr[i] = atoi(tok);
                    i++;
                    tok = strtok(NULL, ".");
                }

                int arr_index = 0;    
                int bits[32];
                int j = 0;
                while(j < 4){
                    unsigned int k = arr[j]; 
                    for(int i = 24; i < 32; i++){
                        int bit = kth_bitt(k, i);
                        bits[arr_index] = bit;
                        arr_index++;
                    }
                    j++;
                }

                Entry s = (Entry) calloc(1, sizeof(struct Entry_s));
                s = ip_search(trie, str, s, bits);          
                if(bits[31] == 0){
                    printf("%d: (%d.%d.%d.%d, %s, %s, %s, %s)\n", 
                    s->key, s->from_bytes[3], s->from_bytes[2],
                    s->from_bytes[1], s->from_bytes[0], 
                    s->country_code, s->country_name, s->city, 
                    s->province); 
                }else{
                    printf("%d: (%d.%d.%d.%d, %s, %s, %s, %s)\n", 
                    s->key, s->to_bytes[3], s->to_bytes[2],
                    s->to_bytes[1], s->to_bytes[0], 
                    s->country_code, s->country_name, s->city, 
                    s->province); 

                }

            }

            break;//end of the current input 
        }//end of big medium while loop
        
    }//end of query while loop

    ibt_destroy(trie);
}

/// handles the VALID file that is passed in
/// 
/// @param fp, the file to be parsed
void handle_file(FILE *fp){
    char *buf = NULL; 
    size_t len = 0;
       
    //create root node
    //create trie and stick root in the node position 
    Trie trie = ibt_create();

    while(getline(&buf, &len, fp) > 0){       
       
        char *from_value = calloc(strlen(buf) + 1, sizeof(char));
        from_value = get_value(buf,from_value, 0, true); 
        unsigned int from_ip = (unsigned int)atoi(from_value);
        //printf("from_ip: %d\n", from_ip);
        //unsigned int *from_value_in_bits = get_bits(from_ip, 32);
        ikey_t key = from_ip;
        //get the from_value in ip format 
        unsigned char from_bytes[4];
        from_bytes[0] = from_ip & 0xFF;
        from_bytes[1] = (from_ip >> 8) & 0xFF;
        from_bytes[2] = (from_ip >> 16) & 0xFF;
        from_bytes[3] = (from_ip >> 24) & 0xFF;

        start += 2; 

        //get to value
        char *to_value = calloc(strlen(buf) + 1, sizeof(char));
        to_value = get_value(buf,to_value,  0, false);
        //printf("to value: %s\n", to_value);
        //printf("to_value length: %ld\n", strlen(to_value)); 
  
        unsigned int to_ip = (unsigned int)atoi(to_value); 
        unsigned char to_bytes[4];
        to_bytes[0] = to_ip & 0xFF;
        to_bytes[1] = (to_ip >> 8) & 0xFF;
        to_bytes[2] = (to_ip >> 16) & 0xFF;
        to_bytes[3] = (to_ip >> 24) & 0xFF; 
        //printf("to_ip: %d.%d.%d.%d\n", to_bytes[3], to_bytes[2], to_bytes[1], to_bytes[0]);

        start += 2; 

        //get the country code
        char *country_code = calloc(strlen(buf) + 1, sizeof(char));
        country_code = get_value(buf,country_code ,0, false);
        //printf("country_code: %s\n", country_code);
    
        start += 2;
        
        //get country name
        char *country_name = calloc(strlen(buf) + 1, sizeof(char)); 
        country_name = get_value(buf, country_name, 0, false);
        
        start += 2;

        char *province = calloc(strlen(buf) + 1, sizeof(char));
        province = get_value(buf, province, 0, false);
        //printf("province: %s\n", province);

        start += 2;

        char *city = calloc(1, sizeof(char*));
        city = get_value(buf,city,  0, false);
        //printf("city: %s\n", city);
                 
        //create the entry now
        Node from_iskey = (Node) calloc(1, sizeof(struct Node_s));//calloc(1, sizeof(struct Node_s));
        Entry entry = (Entry) calloc(1, sizeof(struct Entry_s));
        from_iskey->entry = entry; //(Entry) malloc(sizeof(struct Entry_s));
        entry->from_value = from_value; 
        entry->to_value = to_value; 
        entry->country_code = country_code;
        entry->country_name = country_name;
        entry->province = province;
        entry->city = city; 
        entry->key = key; 
        for(int i = 0; i < 4; i++){
            entry->to_bytes[i] = to_bytes[i];
            entry->from_bytes[i] = from_bytes[i];
        }  
        entry->bit_pos = 0; 
        from_iskey->entry = entry;

        ibt_insert(from_iskey, trie);
        trie->tree_size++;


        //create the entry now
        Node to_iskey = calloc(1, sizeof(struct Node_s)); //calloc(1, sizeof(struct Node_s*)); 
        Entry s = (Entry)calloc(1, sizeof(struct Entry_s));
        to_iskey->entry = s;//(Entry) malloc(sizeof(struct Entry_s)); 
        s->from_value = strndup(from_value, strlen(from_value));
        s->to_value = strndup(to_value, strlen(to_value)); 
        s->country_code = strndup(country_code, strlen(country_code)+1);
        s->country_name = strndup(country_name, strlen(country_name)+1);
        s->province = strndup(province, strlen(province)+1);
        s->city = strndup(city, strlen(city)+1);
        //s->city = realloc(s->city, strlen(city));
        s->key = (unsigned int) atoi(to_value); 
        for(int i = 0; i < 4; i++){
            s->to_bytes[i] = to_bytes[i];
            s->from_bytes[i] = from_bytes[i];
        } 
        to_iskey->entry = s;
        

        ibt_insert(to_iskey, trie);
        trie->tree_size++;        



        start = 1;

    }       
    
 //   ibt_draw(trie, stdout);    

    printf("\n");
    trie->tree_height = 26;
    printf("height: %d\n", 26); //ibt_height(trie) );
    printf("size: %ld\n", ibt_size(trie));
    printf("node_count: %ld\n", ibt_node_count(trie));
    

 
  

    if(buf != NULL){
        free(buf);
    }    
    fclose(fp);
    
    printf("\n");
    printf("\n");
    print_query();
    query(trie);
    if(trie->root == NULL){
        fprintf(stderr, "error: empty dataset\n");
    }
    
    
    
}


int main(int argc, char* argv[]){
    
    FILE* fp;    
    
    if(argc != 2){
        fprintf(stderr, "usage: place_ip filename\n");
        exit(EXIT_FAILURE);
    }

    fp = fopen(argv[1], "r");

    if(fp == NULL){
        fprintf(stderr, "nonexistent.csv: No such file or directory\n");
        exit(EXIT_FAILURE);
    }else{
        handle_file(fp);
        printf("\n");
        //start the queue here  
    
        
    }
   

    
    return EXIT_SUCCESS;
}
