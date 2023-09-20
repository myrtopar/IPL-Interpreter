typedef struct listnode *listptr;    //struct που περιεχει πληροφοριες για μια μεταβλητη
struct listnode{                 
    char name[100];
    int value;
    listptr next;
};

typedef struct blocknode *blockptr;  //struct που περιεχει πληροφοριες για ενα block εντολων 
struct blocknode{
    long start;
    //στο while_list το start αποθηκεύει την αρχη του while, δηλαδή την απόσταση της εντολής σε bytes από την αρχή του αρχείου
    //ενώ στο if_list το αν έχει βρεθεί else που να αντιστοιχεί στο if (με την τιμή 1) ή αν δεν έχει (με την τιμή 0)
    int tabs;
    int cond;	//0 or 1
    blockptr next;
};

listptr node_address(listptr, char *);
void add_listnode(listptr *, char *, int);
void free_list(listptr);
void push(blockptr *, long, int);
void pop(blockptr *);

extern int flag_error;