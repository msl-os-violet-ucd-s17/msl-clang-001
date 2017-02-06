//This C Program creates a concordance based off of an input file, sorted into a binary tree, then using a pre-order
//traversal to print all values to the console before deallocating the tree.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


//The custom struct that forms each node in the tree
struct binarynode {
    char * word;                    //Each node will have a dynamically allocated char array to hold the node's word
    int count;                      //An int that counts how many times a word is found within the input file
    struct binarynode * left;       //Left and Right pointers to other nodes that form our tree
    struct binarynode * right;

};

//This function will search the tree for a given word within a given tree and returns a pointer to that node.
//If it cannot find a node that has the word it will return a pointer to the leaf node it should be inserted into.
struct binarynode * lookup (struct binarynode *node, char * searchword) {


    do
    {
        if (node == NULL)                               //If the node is null we return it as the word does not exist
        {                                               //within the tree. Otherwise, we traverse the tree based on
            return node;                                //the lexicographic order of the tree to either find the node
        }                                               //that contains the word we are searching for or to find
        else if((strcmp(searchword, node->word) == 0))  //where the new word should be placed
        {
            return node;
        }
        else if ((strcmp(searchword, node->word) < 0))
        {
            if(node->left != NULL)
            {
                node = node->left;
            }
            else
            {
                return node;
            }
        }
        else if((strcmp(searchword, node->word) > 0))
        {
            if(node->right != NULL)
            {
                node = node->right;
            }
            else
            {
                return node;
            }
        }
    }while (node != NULL);

}

//The insert function will either increment a given node's count member variable if it's given word and node
//match or it will create a new node to insert the new word into the tree in lexicographic order, returns NULL if given
//NULL
struct binarynode * insert (struct binarynode *node,char * searchword, size_t wordsize) {

    if(node == NULL)                                //The function will return NULL if given NULL, if given a node to
    {                                               //insert we will first check if the word is equal to the word in
        return NULL;                                //node, leading to us increasing it's count, or we will create a
    }                                               //new node with either the left or right pointer

    if((strcmp(searchword, node->word)) == 0)
    {
        node->count++;
    }
    else if((strcmp(searchword, node->word)) < 0)
    {
        if(node->left == NULL)
        {
            node->left = (struct binarynode*) malloc(sizeof(struct binarynode));
            node = node->left;
            node->word = (char*)calloc(wordsize, sizeof(char));
            strcpy(node->word, searchword);
            node->count = 1;
            node->left = NULL;
            node->right = NULL;
        }
        else
        {
            printf("Error, given node has a left child\n");
        }

    }
    else if((strcmp(searchword, node->word)) > 0)
    {
        if(node->right == NULL)
        {
            node->right = (struct binarynode*) malloc(sizeof(struct binarynode));
            node = node->right;
            node->word = (char*)calloc(wordsize, sizeof(char));
            strcpy(node->word, searchword);
            node->count = 1;
            node->left = NULL;
            node->right = NULL;
        }
        else
        {
            printf("Error, given node has a right child\n");
        }

    }

}

//Given a node, this function will delete every node beneath it including the given node itself. This is a recursive
//implementation.
struct binarynode * deletetree(struct binarynode  *root) {
    if (root != NULL)                       //If the root pointer is not NULL then traverse all of that node's left and
    {                                       //right children before deallocating the dynamic array for the word and then
        deletetree(root->left);             //deallocating the node itself, setting it to NULL and then returning it
        deletetree(root->right);
        free(root->word);
        free(root);
        root = NULL;
        return root;
    }
}

//This function recursively goes through a pre-order traversal and prints all words within the tree. Also outputs to file.
void printtree(struct binarynode  *root, FILE *outputfile) {
    if (root == NULL)
    {
        return;
    }
    printtree(root->left, outputfile);
    printf("%s : %d\n",root->word, root->count);
    fprintf(outputfile,"%s : %d \n",root->word, root->count);
    printtree(root->right, outputfile);
}

int main(int argc, char **argv) {
    size_t slength;
    struct binarynode *root = NULL;             //The two binarynode variables, one being the root pointer and the other
    struct binarynode *searchnode = NULL;       //being the node we use with the lookup abd insert functions
    char buffer[100];
    char inputname[60];
    strcpy(inputname, argv[1]);
    char outputname[13] = "outputxx.txt";
    FILE *output;
    FILE *input;                                //This variable and the fopen command in the next line will open the text
    input = fopen(argv[1], "r");                //file that contains the sentence we will read in to build our concordance
                                                //*IMPORANT NOTE* the word file must be stored in your working directory
                                                //for it to be read in correctly which you can set in your run configurations
                                                //(Run->Edit Configurations->Working Directory), the file itself is a
                                                //simple text file
    outputname[6] = inputname[5];
    outputname[7] = inputname[6];               //These two statements parse the index from the input filename and inserts
                                                //the index into the output filename

    output = fopen (outputname, "w");

    while (fscanf(input, "%s", buffer) == 1)
    {
        slength = strlen(buffer) + 1;
        if(root == NULL)
        {
            root = (struct binarynode*) malloc(sizeof(struct binarynode));
            root->word = (char*)calloc(slength, sizeof(char));
            strcpy(root->word, buffer);
            root->count = 1;
            root->left = NULL;
            root->right = NULL;
        }
        else
        {
            searchnode = lookup(root, buffer);
            insert(searchnode, buffer, slength);
        }
    }

    printtree(root, output);
    deletetree(root);

    fclose(input);
    fclose(output);

    return 0;
}