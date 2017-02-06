#Programming Assignment 1
##Intro/Problem Discription
For our first program assignment we were tasked with creating a type of concordance with
a binary tree structure, this program will read in a list of words from a given word file before
adding each word to a binary tree structure. After reading in all words and counting the number
of occurrences in with each word we then output all data to a separate file before deleteing
the tree and closing the program.

##Design
For starters, the tree structure we use to hold the words is a binary tree struct, comprised of
the following:
```
//The custom struct that forms each node in the tree
struct binarynode {
    char * word;                    //Each node will have a dynamically allocated char array to hold the node's word
    int count;                      //An int that counts how many times a word is found within the input file
    struct binarynode * left;       //Left and Right pointers to other nodes that form our tree
    struct binarynode * right;

};
```

Each node in the tree is dynamically allocated but the word array within each node
is also dynamically allocated, we also have an int that holds the number of times a given word is
found (default is 1) and then the standard left and right self referential node pointers that
we will use when allocated a new node in the tree.

In addition to this our tree comes with several functions that help in managing  the tree structure,
the following lists the lookup function:
```
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
```
The purpose of this lookup function is two-fold, it will first look if the word that is given to it is
the same as in the node it is currently at, if the two words (the word in the node and the word we
passed to it) don't match we then decide if we should traverse the left or right path as this
tree is sorted in lexicographic order, if the path we must traverse is NULL then we have
found the location of where the new node should be created. In either case, the purpose of this
function is ONLY to find the correct node in the tree and return the node. The insert function
(listed next) then actually manipulates the tree and data in the tree
```

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
        }
        else
        {
            printf("Error, given node has a right child\n");
        }

    }

}
```
Notice how the insert function does no movement within the tree and merely has three test
cases, if the word it was given is the same as the word in the node then it increments the
node's count variable and returns. Otherwise it inserts the new node into its proper
child, left or right, depending on the lexicographic order of the words. Also note
that if the pointer it must insert already has a node/data, then it was given an incorrect
node by an outside function or program, to maintain the data structure the program prints
an error message and then exits the function, not traversing or editing data in any way.
```
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
```
This is a simple recursive algorithm for deleting the entire tree, it deallocates each
word array within each node and then deletes the node itself, setting the pointer to null
and then returning it. We use the if statement to stop NULL pointers from getting dereferenced
should one be passed into the function on accident
```
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
```
Similar to the deletetree function, the printtree function recursively moves through
the tree and prints the word and count of each node. This is an in-order traversal which,
due to the lexicographic ordering of the tree, will print the words in alphabetical order

Finally we have the main function which handles file I/O, sets the root pointer, reads in the words,
calls functions, and sets filenames to their correct index.
```
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
        }
        else
        {
            searchnode = lookup(root, buffer);
            insert(searchnode, buffer, slength);
        }
    }

    printtree(root, output);
    deletetree(root);

    return 0;
}
```
##File Structure and Usage
The program will take in and read a textfile that is located in the same directory as the
executable, for Clion this will be in the Cmake debug folder or where ever your working directory
is set to in your run configurations. The file itself should contain no newline characters, punctuations,
or special characters, just letters and whitespaces on a single line.

#####IMPORTANT
ALL TEXT FILES *MUST* BE OF THE FOLLOWING FORMAT: 

inputxx.txt

where "xx" are both numbers that list a specific index, 01 and 35 for example. The program will
then output a similar text file with the format: 

myoutputxx.txt

Where the "xx" will correspond to the input file you gave the program (input34 will produce
output34 etc.). The output file will list all the words in the tree in alphabetical order and will
also list the number of times each word was found in the input file.

#####NOTE
The program may not return a filled in file when it first creates the output file, should this occur
simply run the program again with the same input file to fill the output file with data.

