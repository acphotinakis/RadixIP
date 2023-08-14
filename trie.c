// 
// File: trie.c 
// Starter trie module for an integer-keyed trie ADT data type
//  
// @author Andrew     
// // // // // // // // // // // // // // // // // // // // // // // // 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "trie.h"
#include <strings.h> 
#include <stdbool.h>
#include <assert.h>
#define TRIE_H
#include "trie.h"
#define COUNT 5

/// Node_s: binary tree node TODO 
/// Node is a pointer to a dynamically allocated struct Node_s.
/// NodeH is a pointer to a pointer to a struct Node_s.

typedef struct Node_s { 
    struct Node_s *left;
    Entry entry;  
    struct Node_s *right; 
} *Node;

typedef Node * NodeH;


/// Trie_s: TODO define the Trie ADT here

struct Trie_s { 
    int tree_height;
    int tree_size; 
    int node_count; 
    int node_test;
    int count;
    int null_node;
    Node root;   
};

//prototypes for functions
Entry nearest(Trie trie, NodeH node, ikey_t key, Node closest, Entry s );
static Entry search_trie(Trie trie, NodeH handle, ikey_t key, int d);
// global constants for bit operations and sizes
// used by the application program

#define BUFLENGTH 256

/// get bit at position in number
///
/// @param ui, unsigned int to get positional bit from 
/// @param pos, position 0-32 to get bit from
/// @return 0 or 1 
int kth_bit(unsigned int ui, int pos){
    return (ui >> pos) & 1; 
}

/// creates emtpy tree to be used for the file
///
/// @return trie, empty trie
Trie ibt_create(){
    Trie trie = (Trie)calloc(1, sizeof(struct Trie_s));
    if(trie != NULL){
        trie->tree_height = 0; 
        trie->tree_size = 0;
        trie->node_count = 0;  
        trie->count = 0;
        trie->node_test = 0;
        trie->null_node = 0;
        trie->root = NULL; 
    }

    return trie; 
}

/// returns bit of key at position
///
/// @param key, the key value
/// @param bit_pos, the postion
/// @return 0 or 1 
int get_bit(ikey_t key, int bit_pos){
    //int bit = (key >> bit_pos) & 1;
    return (key >> (32 - bit_pos - 1)) & 1;
}

/// splits the nodes creating null links for as long as 
/// they keys match up
///
/// @param hande_two, a pointer to a node
/// @param handle, a node that contains the data being inserted 
/// @param d, the positon for bit 
/// @param trie, the trie we are inserting into
/// @return a node
Node split(NodeH handle_two, Node handle, int d, Trie trie){
    Node t = (Node) calloc(1, sizeof(struct Node_s)); 
    ikey_t v = (*handle_two)->entry->key;
    ikey_t w = (handle->entry->key); 
    
    switch( (get_bit(v, d)*2) + (get_bit(w, d) ))
    { 
        case 0: 
            d++;
            t->left = split(handle_two, handle, d, trie); 
            break;
        case 1: 
            t->left = *handle_two;
            t->right = handle;
            break; 
        case 2: 
            t->right = *handle_two;
            t->left = handle;
            break; 
        case 3:
            d++;
            t->right = split(handle_two, handle, d, trie); 
            break;
    }
    
    return t; 
}

/// inserts the node in the trie 
///
/// @param handle, the handle which is the root of the trie 
/// @param node_data, the created node to be inserted 
/// @param d, the bit position 
/// @param trie, the trie we are inserting into
static void insert_node(NodeH handle, Node node_data, int d, Trie trie){
    

    if(*handle == NULL){ 
        *handle = node_data;
        return; 
    }
    Node new_node = *handle;
    if(new_node->left == NULL && new_node->right == NULL){
        Node handle_two = node_data;
        *handle = split( &handle_two, *handle, d, trie);
        //*handle = (Node) realloc(*handle, sizeof(struct Node_s));
   
    }

    if( (get_bit(node_data->entry->key, d)) == 0){
        d++;
        insert_node(&(new_node)->left, node_data, d, trie);
    }else{ 
        d++;
        insert_node(&(new_node)->right, node_data, d, trie);
    } 
    
    return;
}

/// calls to actually insert the node in the trie 
///
/// @param entry, the entry to be inserted 
/// @param trie, the trie being inserted into
void ibt_insert(Node entry, Trie trie){
    NodeH handle = &trie->root; 
    insert_node(handle, entry, 0, trie);
   // trie->root = insert_node(handle, entry, 0, trie);
}

/// takes in a ip address and searches for it
///
/// @param trie, databas that is being searched
/// @param input, the ip from the user 
/// @param s, the entry 
/// @param bits, the use input in bits stored in an indexed array
/// @return the entry being searched for
Entry ip_search(Trie trie, char* input, Entry s, int bits[]){ 
    //Entrys a = get_ip_entry(trie, &trie->root, h, bits, 0);
    NodeH handle = &trie->root;
    Node curr = *handle;
    
    int i = 0; 
    while(i < 32){
        if(curr->entry != NULL && curr->right == NULL && curr->left == NULL){ 
        s = curr->entry;
            return s;
        }
        if(bits[i] == 0){
            curr = curr->left;
       
        }else if(bits[i] == 1){
            curr = curr->right;
      
        }
        i++;
    }    

    return s;
}

/// gets closest match to a node
///
/// @param root, root of he trie 
/// @param value, the key being searched for
/// @return the node that is the closest
Entry smallest(NodeH handle, ikey_t key){

    Node smallest = (Node) calloc(1, sizeof(struct Node_s));
    int mindistance = 0x7FFFFFFF;
    
    Node curr = *handle;

    while(curr != NULL){
        int distance = abs((int)curr->entry->key - (int)key);
        if(distance < mindistance){
            mindistance = distance;
            smallest = curr;
        }

        if(distance == 0) break;

        if(curr->entry->key > key){
            curr = curr->left;
        }else{
            curr = curr->right;
        }
    }

    return smallest->entry;
}

Entry greatest(NodeH handle, ikey_t key){

    Node biggest = (Node) calloc(1, sizeof(struct Node_s));
    int maxdistance = 0x80000000;
    Node curr = *handle;

    while(curr != NULL){
        int distance = abs((int)curr->entry->key - (int)key);
        if(distance > maxdistance){
            maxdistance = distance;
            biggest = curr;
        }

        if(distance == 0) break;

        if(curr->entry->key < key){
            curr = curr->left;
        }else{
            curr = curr->right;
        }
    }

    return biggest->entry;
}

///searching for an entry node
///
/// @param trie, the trie beign searched 
/// @param handle, handle to the root of the trie 
/// @param key, the key that is being searched for
/// @param d, the bit position
/// @return the entry that is found in the database
static Entry search_trie(Trie trie, NodeH handle, ikey_t key, int d){
    if(*handle == NULL || d == 32){
        Entry null = calloc(1, sizeof(struct Entry_s));
        null->key = -1;
        null->from_value = NULL;
        null->to_value = NULL;
        null->country_name = NULL;
        null->country_code = NULL;
        null->province = NULL;
        null->city = NULL;
        
        return null;
    }

    Node curr = *handle;
    if(curr->left == NULL && curr->right == NULL){
        return curr->entry;
    }
     
    if(get_bit(key, d) == 0){
        if(curr->left != NULL){
            d++;
            curr->entry = search_trie(trie, &(curr)->left, key, d);
        }else{
            curr->entry = smallest(&(curr)->right, key);
        }
    }else{
        if(curr->right != NULL){
            d++;
            curr->entry = search_trie(trie, &(curr)->right, key, d);
        }else{
            curr->entry = greatest(&(curr)->left, key);
        }
    }    
    
    return curr->entry; 
}

/// calls to search the trie for the entry from the user
/// 
/// @param trie, the trie being searched 
/// @param key, the user inputed search key 
/// @return the entry found
Entry ibt_search(Trie trie, ikey_t key){
    NodeH handle = &trie->root; 
    return search_trie(trie, handle, key, 0);
}

void show(Trie trie, NodeH node, FILE* stream){
    if(*node == NULL){  
        return;
    }
    Node new_node = *node;
    show(trie, &(new_node)->left, stream);
    if(new_node != NULL){ 
        if(new_node->entry != NULL){
            if(new_node->entry->key == (unsigned int) atoi(new_node->entry->from_value)){ 
                printf("%d: (%d.%d.%d.%d, %s, %s, %s, %s)\n",
                new_node->entry->key, new_node->entry->from_bytes[3], new_node->entry->from_bytes[2],
                new_node->entry->from_bytes[1], new_node->entry->from_bytes[0],
                new_node->entry->country_code, new_node->entry->country_name, new_node->entry->city, 
                new_node->entry->province);
            }else{
                printf("%d: (%d.%d.%d.%d, %s, %s, %s, %s)\n",
                new_node->entry->key, new_node->entry->to_bytes[3], new_node->entry->to_bytes[2],
                new_node->entry->to_bytes[1], new_node->entry->to_bytes[0],
                new_node->entry->country_code, new_node->entry->country_name, new_node->entry->city, 
                new_node->entry->province);

            }
        }
    }  
    show(trie, &(new_node)->right, stream);
}

void ibt_show(Trie trie, FILE* stream){
    printf("ibt_show output:\n");
    printf("\n");
    printf("keys:\n");
    show(trie, &trie->root, stream);
}

/// prints the trie in order 
///
/// @param trie, trie to search 
/// @param node, handle to the root
/// @param stream, where to print the trie 
/// @param pass, random variable 
void inorder(Trie trie, NodeH node, FILE* stream){
    if(*node == NULL){  
        printf("NULL pass, %d\n", trie->node_count);
///        trie->node_count++;
        return;
    }
    Node new_node = *node;
    inorder(trie, &(new_node)->left, stream);
    if(node == NULL) printf("null\n");
    if(node != NULL){ 
       if(new_node->entry != NULL) 
            fprintf(stream, "(key): %d\n", new_node->entry->key);
    }  
    inorder(trie, &(new_node)->right, stream);
}

/// calls the function above
void ibt_draw(Trie trie, FILE *stream){
    if(trie->root == NULL){
        fprintf(stream, "empty");
    }
    printf("\n");
    NodeH handle = &trie->root;
    inorder(trie, handle, stream); 
}

/// returns how many entries are in the trie 
///
/// @param trie, the trie to be analyzed 
/// @return number of entries in the trie
size_t ibt_size(Trie trie){
    return trie->tree_size;
}

/// gets height of the trie 
///
/// @param trie to be searched 
/// @param node, handle to the root 
/// @param left_height, height of left subtree from root 
/// @param right_height, height of right subtree from root
size_t set_tree_height(Trie trie, NodeH node, int left_height, int right_height){
    if(node == NULL) return 0; 
    Node curr = *node;
    
    if(curr->left != NULL)left_height = set_tree_height(trie, &(curr)->left, left_height, right_height);
    if(curr->right != NULL) right_height = set_tree_height(trie, &(curr)->right, left_height, right_height);

    if(left_height > right_height){
        return left_height + 1;
    }

    return right_height + 1;
}

/// calls function above to return the height of the trie
size_t ibt_height(Trie trie){ 
    NodeH handle = &trie->root;
    trie->tree_height = set_tree_height(trie, handle, 0, 0);
    return trie->tree_height;
}

/// in order traversal without printing to count nodes
/// 
/// @param trie, trie
/// @param node, pointer to root of trie 
/// @param pass, passed count around 
void inord_noprint(Trie trie, NodeH node){
    if(*node == NULL){  
        trie->node_count++;
        return;
    }
    Node new_node = *node;
    
    inord_noprint(trie, &(new_node)->left);
    inord_noprint(trie, &(new_node)->right);   
}

/// calls function above and returns the trie node count 
///
/// @param trie, trie to be counted
size_t ibt_node_count(Trie trie){
    inord_noprint(trie, &trie->root);
    return trie->node_count - trie->tree_size - 1;
}


/// uses post order traversal to free the trie
///
/// @param trie to be destroyed 
/// @param node access point to the rest of the trie
void trie_destroy(Trie trie, NodeH node){

    if(*node == NULL){
        return;
    }
        
    //dereference the NodeH pointer to get a Node struct
    Node new_node = *node; 
    //visit left subtree
    trie_destroy(trie, &(new_node)->left);
    //visit right subtree  
    trie_destroy(trie, &(new_node)->right);
    
    //destroy leaf nodes
    if(new_node != NULL && new_node->left == NULL && new_node->right == NULL){

        if(new_node->entry != NULL){
            free(new_node->entry->from_value);
            free(new_node->entry->to_value);
            free(new_node->entry->country_name);
            free(new_node->entry->country_code);
            free(new_node->entry->province);
            free(new_node->entry->city);
//            free(new_node->entry->from_value_in_bits);
//            free(new_node->entry->to_value_in_bits);
        }
        free(new_node->entry);
        free(new_node->left);
        free(new_node->right);
    }else{
        //free internal nodes but check their children first
        if(new_node != NULL ){ 
            if(new_node->right != NULL){ 
                free(new_node->right);
                new_node->right = NULL;
            }
            if(new_node->left != NULL){
                free(new_node->left);
                new_node->left = NULL;
            }
        }
    }
    
  

}

/// destroys the trie 
///
/// @param trie, trie to be destroyed
void ibt_destroy(Trie trie){
    assert(trie != NULL);

    trie_destroy(trie, &trie->root);
    
    //after freeing all the nodes and possible entries:
    //free the root of the tree
    free(trie->root);
    //free the tree
    free(trie);
}










