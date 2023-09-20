
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "functions.h"
#define ERROR -1

int check_validity(char *variable)
{
    int i = 0;

    if(strcmp(variable, "read") == 0 || strcmp(variable, "write") == 0 || strcmp(variable, "writeln") == 0 || strcmp(variable, "random") == 0 || strcmp(variable, "argument") == 0){
        printf("Error! The word \"%s\" is reserved by ipl.", variable);
        return ERROR;
    }
    else if(strcmp(variable, "break") == 0 || strcmp(variable, "continue") == 0 || strcmp(variable, "while") == 0 || strcmp(variable, "if") == 0 || strcmp(variable, "else") == 0){
        printf("Error! This word is reserved by ipl.");
        return ERROR;    
    }

    if(variable[0] == '_' || (variable[0] >= '0' && variable[0] <= '9')){
        return ERROR;
    }   //κοιταει το πρωτο χαρακτηρα της μεταβλητης. Δεν επιτρεπεται να ξεκιναει με underscore η ψηφιο

    while(variable[i] != '\0'){
        if((variable[i] >= 'a' && variable[i] <= 'z') || (variable[i] >= 'A' && variable[i] <= 'Z') || (variable[i] >= '0' && variable[i] <= '9') || variable[i] == '_'){
            i++;
        }
        else{      //αν βρει οποιονδηποτε χαρακτηρα εκτος απο πεζα και κεφαλαια λατινικα, ψηφια η udnderscore η μεταβλητη ειναι μη εγκυρη
            printf("Error! Invalid variable name.\n");
            return ERROR;
        }
    }
    return 1;    //αν η μεταβλητη δεν εχει μη δεκτους χαρακτηρες, ολα καλα, επιστρεφει 1
}

int tab_counter(char *line)        
{    int i = 0;                      

    while(line[i] == '\t'){
        i++;
    }
    return i;
}

int number(char *variable)
{
    int i = 0, n;
    while(variable[i] != '\0'){
        if((variable[i] < '0') || (variable[i] > '9')){
            return ERROR;
        }
        i++; 
    }
    return 1;
}

int assign_value(char *varint, listptr list)  //δεχεται ενα string και αναθετει την τιμη που αυτο κουβαλαει σε εναν ακεραιο, τον οποιο και επιστρεφει                    
{
    listptr p;
    int var;
    if(number(varint) == ERROR){                          //αμα δεν ειναι αριθμος αλλα ειναι καποια μεταβλητη
        p = node_address(list, varint);                   //δες αν υπαρχει η μεταβλητη
        if(p == NULL){                                    //αν οχι
            if(check_validity(varint) == ERROR){          //δες αν ειναι συντακτικα ορθη
                return ERROR;                             //αν οχι πετα ερρορ και μηνυμα λαθους
            } 
            add_listnode(&list, varint, 0);               //αν ειναι συντακτικα ορθη προσθεσε την στη λιστα και αρχικοποιησε τη με 0
            p = node_address(list, varint);               //δωσε στο var τη διευθυνση του κομβου της μεταβλητης που μολις προσθεσες
                                      
        }
        var = (p->value);                                 //δωσε στο int1 το μελος value αυτου του κομβου
        
    }
    else{                                                 //αν τωρα το varint ειναι αριθμος
        var = atoi(varint);
    }
    return var;    
}



pnode calculate_expression(char *varint1, char *operator, char *varint2, listptr list) //κανει μια πραξη μεταξυ 2 ακεραιων και επιστρεφει το αποτελεσμα τους σε ενα struct pnode
{

    int int1, int2;
    pnode r;
    int1 = assign_value(varint1, list);
    if(int1 == ERROR){
        r.value = 0;
        r.error = -1;
        return r;
    }
    int2 = assign_value(varint2, list);
    if(int2 == ERROR){
        r.value = 0;
        r.error = -1;
        return r;
    }

    if(strcmp(operator, "+") == 0){
        r.value = int1 + int2;
        r.error = 0;
    }
    else if(strcmp(operator, "-") == 0){
        r.value = int1 - int2;
        r.error = 0;
    }
    else if(strcmp(operator, "*") == 0){
        r.value = int1 * int2;
        r.error = 0;
    }
    else if(strcmp(operator, "/") == 0){
        if(int2 == 0){
            printf("Error! Division by zero is undefined.");
            r.value = 0;
            r.error = -1;
            return r;
        }
        r.value = int1 / int2;
        r.error = 0;
    }
    else if(strcmp(operator, "%") == 0){
        if(int2 == 0){
            printf("Error! Division by zero is undefined.");
            r.value = 0;
            r.error = -1;
            return r;
        }
        r.value= int1 % int2;
        r.error = 0;
    }
    else{
        r.value = 0;
        r.error = -1;
    }
    return r;
}



int condition(char *varint1, char *compoper, char *varint2, listptr list)     //επιστρέφει την τιμή της συνθήκης (0 ή 1)
{
    int int1, int2;

    int1 = assign_value(varint1, list);
    int2 = assign_value(varint2, list);   


    if(strcmp(compoper, "==") == 0){
        return (int1 == int2);
    }
    else if(strcmp(compoper, "!=") == 0){
        return (int1 != int2);
    }
    else if(strcmp(compoper, "<=") == 0){
        return (int1 <= int2);
    }
    else if(strcmp(compoper, "<") == 0){
        return (int1 < int2);
    }
    else if(strcmp(compoper, ">=") == 0){
        return (int1 >= int2);
    }
    else if(strcmp(compoper, ">") == 0){
        return (int1 > int2);
    }
    return 0;
}

void first_occurence(char *string, char x)
{
    char *ptrchr;
    ptrchr = strchr(string, x);
    if(ptrchr != NULL){
        *ptrchr = '\0';
    } 
}


int expression_parsing(char *line, char array[5][100]){     //χωρίζει την έκφραση σε token
    int i, count = 0;
    char *token, *ptrchr;


    ptrchr = strchr(line, '=');
    if(ptrchr != NULL){
        *ptrchr = ' ';          //βρίσκει σε ποια θέση του line υπάρχει = και το αντικαθιστά με κενό στο line
    }
    else{
        printf("Error! \"=\" character wasn't found\n");
        return ERROR;
    }

    array[1][0] = '=';          //στη 2η θέση του array βάζει "=\0", αφού δεν μπορεί να υπάρχει κάποιο άλλο σύμβολο εκεί
    array[1][1] = '\0';

    for(i = 0 ; line[i] != '\0' ; i++){     //βρίσκει σε ποια θέση του line υπάρχει operator
        if(line[i] == '+'|| line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%'){
            break;
        }

    }


    if(line[i] == '\0'){        //αν δεν βρεθεί operator, η ανάθεση (εχει μορφή x = a άρα μόνο 3 tokens)
        *array[3] = '\0';       //και οι τελευταίες 2 θέσεις του array δεν χρησιμοποιούνται
        *array[4] = '\0';
        count = 3;

    }
    else{                       //εκφραση (εχει τη μορφη x = a <operator> b αρα 5 tokens)
        *array[3] = line[i];    //ο τελεστής αποθηκεύεται στην 4η θέση του πίνακα και αντικαθίσταται με κενό στο line
        line[i] = ' ';
        count = 5;

    }

    //τώρα το line εχει τη μορφή "<var1> <var2> (<var3>)"
    //οι μεταβλητές μπαίνουν στις θέσεις 0, 2 και 4 (αν υπάρχει 3η μεταβλητή) του πίνακα
    i = 0;

    token = strtok(line, " \t");
    while(token != NULL){

        strcpy(array[i], token);             //βαζει το τοκεν στον πινακα

        first_occurence(array[i], '\n');

        first_occurence(array[i], '\r');

        i = i + 2;                         //προχωραει στην μεθεπόμενη θέση του array (η οποία αντιστοιχεί σε μεταβλητή) για να φυλάξει το επόμενο token
        
        token = strtok(NULL, " \t");
    }
    if(count == 3 && *array[4] != '\0'){
        printf("Error! No operator found.\n");
        return ERROR;
    }
    return count;

}


void condition_parsing(char *line, char array[5][100]){     //χωρίζει τη συνθήκη σε token
    int i;
    char *token, *ptrchr;

    for(i = 0 ; line[i] != '\0' ; i++){     //βρίσκει σε ποιά θέση του line αρχίζει ο comporper
        if(line[i] == '='|| line[i] == '!' || line[i] == '<' || line[i] == '>'){
            break;
        }

    }

    if(line[i] == '\0'){
        printf("Error! No operator was found\n");
        flag_error = 1;
        return;
    }
    
    if((line[i] == '<' || line[i] == '>'|| line[i] == '=' || line[i] == '!') && line[i+1] == '='){       //αν ο copmpoper είναι "<=", ">=", "==" ή "!="

        array[2][0] = line[i];      //βάζει τους χαρακτήρες στην 3η θέση του array
        array[2][1] = line[i+1];
        array[2][2] = '\0';
        line[i] = ' ';              //και τους αντικαθιστά με τον κενό χαρακτήρα
        line[i+1] = ' ';
    }
    else if(line[i] == '<' || line[i] == '>'){      //αν ο copmpoper είναι "<" ή ">"
        array[2][0] = line[i];
        array[2][1] = '\0';
        line[i] = ' ';
    }
    else{
        printf("Error! Invalid operator\n");
        flag_error = 1;
        return;
    }

    i = 0;
    token = strtok(line, " \t");
    while(token != NULL){
        if(i == 2){ //προσπερνάει την 3η θέση του πίνακα γιατί εκεί έχει αποθηκευτεί ο compoper
            i++;
        }
        strcpy(array[i], token);             //βαζει το τοκεν στον πινακα

        first_occurence(array[i], '\n');

        first_occurence(array[i], '\r');

        i++;                         //προχωραει στην επομενη θεση του array για να φυλαξει το επομενο token
        
        token = strtok(NULL, " \t");
    }

}

void reread_line(char *line, long int checkpoint, FILE *fp){    //ξαναδιαβάζει την τελευταία γραμμή του προγράμματος που διαβάστηκε
    int fseek_successful = fseek(fp, checkpoint, SEEK_SET); //μετακινεί τον fp στη θέση που είχε πριν διαβάσει το τρέχον line (while ...)
                
    if(fseek_successful != 0){
        printf("Error: Could not re-read line successfully\n");
        flag_error = 1;
        return;
    }
    fgets(line, 1000, fp);
    first_occurence(line, '#');     //ελέγχει αν υπάρχει σχόλιο μετά την εντολή ώστε ο διερμηνευτής να το αγνοήσει
    first_occurence(line, '\n'); //αντικαθιστά το \n με \0 
    first_occurence(line, '\r'); //αντικαθιστά το \r με \0
}