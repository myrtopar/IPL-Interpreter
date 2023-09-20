#define ERROR -1
int check_validity(char *);
int tab_counter(char *);
int number(char *);
struct p calculate_expression(char *, char *, char *, listptr);
int condition(char *, char *, char *, listptr);
int assign_value(char *, listptr);
void first_occurence(char *, char);
int expression_parsing(char *, char [5][100]);
void condition_parsing(char *, char [5][100]);
void reread_line(char *, long int, FILE *);

struct p{                    //στην συναρτηση calculate_expression δε μπορω να επιστρεφω -1 για error διοτι -1 μπορει να ειναι το αποτελεσμα μιας πραξης
  int value;
  int error;
};

typedef struct p pnode;

extern int flag_error;