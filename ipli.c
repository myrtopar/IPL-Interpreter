#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h> 
#include "list.h"
#include "functions.h"

int flag_error; //αρχικοποιείται αυτόματα με 0

int main(int argc, char *argv[])
{
    char line[1000];
    char *ptrchr, *token;
    char array[5][100];  //η αναθεση ειναι αυτη που εχει τα περισσοτερα tokens (5), οι μεταβλητες εχουν μεχρι 100 χαρακτηρες  //θεσεις για 100 μεταβλητες
    int i, count, v, n, flag_break = 0, fseek_successful;
    long int checkpoint;
    
    long int curtime = time(NULL);
    srand((unsigned int)curtime); 

    listptr variable_list, v_address;
    pnode result;
    blockptr while_list, if_list;
    FILE *fp;
    variable_list = NULL;
    while_list = NULL;
    if_list = NULL;
    

    fp = fopen(argv[1], "r");

    if(fp == NULL){
        printf("Error, could not find file\n");
        perror("error: ");
        exit(-1);
    }


    while(1){        //ενοσω η συνθηκη ειναι αληθης δηλαδη συνεχεια, εκτελει την επαναληψη μεχρι να γινει break εσωτερικα της while
        checkpoint = ftell(fp);

        fgets(line, 1000, fp); 


        if(feof(fp) && while_list == NULL){    //αν φτάσει στο τέλος του αρχείου και η στοίβα των while είναι άδεια το πρόγραμμα τερματίζει
            break;
        }
            
        if(line[tab_counter(line)] == '#'|| line[tab_counter(line)] == '\n' || line[tab_counter(line)] == '\r'){ //αν η το πρώτο στοιχείο μετά τα tabs είναι # ή η γραμμή είναι κενή, την αγνοεί
            continue;
        }
        else{                               //ελεγχει αν υπαρχει σχολιο μετα την εντολη
            first_occurence(line, '#');
        }



        if(while_list != NULL && flag_break == 1 && tab_counter(line) >= (while_list->tabs + 1) ){    //αν βρισκομαι μεσα σε while και κατω απο break απλα προχωρα αγνοωντας τις εντολες
            continue;
        }
        else if(while_list != NULL && flag_break == 1 && tab_counter(line) < (while_list->tabs + 1) ){  //μολις προσπερασει ολες τις εντολες του while 
            pop(&while_list);          //διαγραφει τον κομβο του while 
            if (flag_error == 1){
                break;
            }
            flag_break = 0;            //επαναφερει το flag σε 0 αφου η break πραγματοποιηθηκε
        }



        if(while_list != NULL && (while_list->cond) == 1){          //if we're in a loop and condition != 0
            
            if((tab_counter(line) <= (while_list->tabs)) && ((while_list->start) != checkpoint) || feof(fp)){      //αν η εντολή βρίσκεται εκτός του block (όχι στην αρχή του ίδιου while) ή έχουμε φτάσει στο τέλος του προγράμματος
                
                int fseek_successful = fseek(fp, (while_list->start), SEEK_SET);  //μετακινεί τον fp στην αρχή του τελευταίου while που διάβασε
                
                if(fseek_successful != 0){
                    printf("Error: Could not move to the start of the block successfully\n");
                    break;
                }
                            
                continue;                       //η εντολή που διαβάστηκε τελευταία βρίσκεται έξω από το block, οπότε δεν εκτελείται 
            }          
        }
        else if(while_list != NULL){                                          //αν condition == 0
            if((tab_counter(line) > (while_list->tabs)) && !feof(fp)){        //αγνοεί τις εντολές μέσα στο block
                continue;
            }
            else{                                                  //όταν βγεί από τον βρόχο διαγράφει τον πρώτο κόμβο του while list (που αντιστοιχεί στο τελευταίο while)
                pop(&while_list);

                if(feof(fp)){                                      //αν έχουμε φτάσει στο τέλος του προγράμματος
                    break;
                }
            }
        }




        if(if_list != NULL 																				//αν η στοίβα if_list δεν είναι κενή
        	&& (((if_list->cond) == 1 && (if_list->start) == 1) || ((if_list->cond) == 0 && (if_list->start) == 0)) 	//και βρισκόμαστε πριν το else με condtion == 1 ή μετα το else με condition == 0
        	&& (tab_counter(line) >= (if_list->tabs)+1)){												//και η εντολή είναι μέσα στο if

            continue;
        }
        else if(if_list != NULL && tab_counter(line) <= (if_list->tabs)){          //αν η εντολή είναι έξω από το if

			if(memcmp(&line[tab_counter(line)] , "else", 4) != 0){                 //και οι πρώτοι 4 χαρακτήρες του line δεν είναι else κάνε pop τον πρώτο κόμβο
            	pop(&if_list);
                if(flag_error == 1){
                    break;
                }
            }
        }

        i = 0;
        count = 0;

        first_occurence(line, '\n'); //αντικαθιστά το \n με \0 
        first_occurence(line, '\r'); //αντικαθιστά το \r με \0
        
        token = strtok(line, " \t");       //σπαει τα tokens οπου βρισκει whitespace η tab
        while(token != NULL){

            strcpy(array[i], token);             //βαζει το τοκεν στον πινακα

            i++;                         //προχωραει στην επομενη θεση του array για να φυλαξει το επομενο token
            count++;                     //μετραει ποσα tokens εχει η γραμμή
            token = strtok(NULL, " \t");     //σπαει το επομενο τοκεν
        }

        if(count == 0){            //αν η γραμμη δεν εχει κανενα token, προχώρα
            continue;
        }

        
        //read
        if(strcmp(array[0], "read") == 0){
            if(count != 2){                              //η γραμμή πρεπει να εχει τη μορφη read <var> δηλαδη ακριβως 2 tokens
                printf("Error! Wrong argument size! %d ", count);
                break;
            }
            scanf("%d", &v);
         	v_address = node_address(variable_list, array[1]);         //η node_address αναζητα στο variable_list την μεταβλητη που βρισκεται στο array[1] και επιστρεφει την διευθυνση της στο v_address

    	    if(v_address == NULL){              //αν η μεταβλητη δεν υπαρχει στο variable_list
                if(check_validity(array[1]) == ERROR){               //ελεγχει αν εχει εγκυρο ονομα
                    break;      
                }
                add_listnode(&variable_list, array[1], v);          //προσθετει τη μεταβλητη στο τελος της variable_list με τιμη v
            }
            else{
                (v_address->value) = v;                            //αλλιως τοποθετει στο value του v_address κομβου την τιμη v
            }

        }

        //write
        else if(strcmp(array[0], "write") == 0){  
            if(count != 2){                        //η γραμμή πρεπει να εχει τη μορφη write <var> δηλαδη ακριβως 2 tokens
                printf("Error! Wrong argument size");
                break;
            }
            printf("%d ", assign_value(array[1], variable_list));    //τυπωνει την τιμη της μεταβλητης του array[1]                  
        }

        //writeln
        else if(strcmp(array[0], "writeln") == 0){  
            if(count != 2){                      //η γραμμή πρεπει να εχει τη μορφη writeln <var> δηλαδη ακριβως 2 tokens
                printf("Error! Wrong argument size.");
                break;
            } 
            printf("%d\n", assign_value(array[1], variable_list));                            
        }

        //random
        else if(strcmp(array[0], "random") == 0){
            if(count != 2){                     //η γραμμή πρεπει να εχει τη μορφη random <var> δηλαδη ακριβως 2 tokens
                printf("Error! Wrong argument size.");
                break;
            }

            v = rand(); 

            v_address = node_address(variable_list, array[1]);

    	    if(v_address == NULL){
                if(check_validity(array[1]) == ERROR){
                    break;
                }
    			add_listnode(&variable_list, array[1], v);
    		}
            else{
                v_address->value = v;
            }                           
        } 


        //argument size
        else if((strcmp(array[0], "argument") == 0) && (strcmp(array[1], "size") == 0)){
            if(count != 3){                        //η γραμμή πρεπει να εχει τη μορφη argument size <var> δηλαδη ακριβως 2 tokens
                printf("Error! Wrong argument size.");
                break;
            }
            v = argc - 2;
            v_address = node_address(variable_list, array[2]);

    	    if(v_address == NULL){
                if(check_validity(array[2]) == ERROR){
                    printf("Error! Invalid variable name.\n");
                    break;
                }
    			add_listnode(&variable_list, array[2], v);
    		}
            else{
                v_address->value = v;
            } 
        }


        //command line arguments
        else if(strcmp(array[0], "argument") == 0){
            if(count != 3){                        //η γραμμή πρεπει να εχει τη μορφη argument <varint> <var> δηλαδη ακριβως 2 tokens
                printf("Error! Wrong argument size.");
                break;
            }

            v = assign_value(array[1], variable_list);          //αναθετει στο ν τον αριθμο που αναπαριστα το string <varint>

            if((v == ERROR) || (v > (argc-2)) || (v < 1)){      //αν το ν ειναι ERROR η το ν ειναι μεγαλυτερο απο το πληθος των ορισματων η το ν-οστο ορισμα ειναι αρνητικο η               
                printf("Error! Invalid argument!\n");
                break;
            }
            else{
                //αναθεσε στο n το ν-οστο ορισμα σε ακεραια μορφη
                n = atoi(array[1]);
                if(number(argv[n + 1]) == ERROR){           //εαν το ορισμα δεν ειναι αριθμητικη συμβολοσειρα 
                    printf("Error! Invalid argument.\n");
                    break;
                }

                v = atoi(argv[n + 1]);       //στις θεσεις argv[0] και argv[1] βρισκονται τα ./εκτελεσιμο ονομα.ipl οποτε τα ορισματα του προγραμματος ξεκινανε απο το n + 1 
                v_address = node_address(variable_list, array[2]);
    	       	if(v_address == NULL){
                    if(check_validity(array[2]) == ERROR){
                        printf("Error! Invalid variable name.\n");
                        break;
                    }
    				add_listnode(&variable_list, array[2], v);
                }
                else{
                    v_address->value = v;
                }                        
            }
        }


        //while
        else if(strcmp(array[0], "while") == 0){
        	reread_line(line, checkpoint, fp);	//διαβάζει ξανα την τελευταία γραμμή γιατι έχει αλλοιωθεί από τη strtok
            if(flag_error == 1){
                break;
            }

            condition_parsing(line, array);		//χωρίζει το condition σε tokens (υποστηρίζονται οι εκφράσεις του τύπου <varint1> <compoper> <varint2> και <varint1><compoper><varint2>)
            if(flag_error == 1){
                break;
            }

            if(while_list == NULL || (while_list != NULL && ((while_list->start) != checkpoint))){	//το while που έχει διαβαστεί είναι διαφορετικό από το τελευταίο δημιουργεί νέο κόμβο
                push(&while_list, checkpoint, tab_counter(line));
                if(flag_error == 1){
                    break;     
                }
            }
			(while_list->cond) = condition(array[1], array[2], array[3], variable_list);			//ανανεώνει τη συνθήκη με την τρέχουσα τιμή της (0 ή 1)
        }


        //if
        else if(strcmp(array[0], "if") == 0){
            reread_line(line, checkpoint, fp);	//διαβάζει ξανα την τελευταία γραμμή γιατι έχει αλλοιωθεί από τη strtok
            if(flag_error == 1){
                break;     
            }

            condition_parsing(line, array);		//χωρίζει το condition σε tokens (υποστηρίζονται οι εκφράσεις του τύπου <varint1> <compoper> <varint2> και <varint1><compoper><varint2>)
            if(flag_error == 1){
                break;     
            }

            push(&if_list, 0, tab_counter(line));	//δημιουργεί καινούριο κόμβο στο if_list (στο start του blocknode αποθηκευεται 0 ή 1 αναλογα με το αν έχουμε βρει else statement,
            if(flag_error == 1){                    //το αποθηκεύουμε για να μη χρειαστεί η δημιουργία δεύτερου είδους struct)
                break;     
            }            

            (if_list->cond) = condition(array[1], array[2], array[3], variable_list);				//βρίσκει την τιμή της συνθήκης
        }


        //else
        else if(strcmp(array[0], "else") == 0){   

            while(tab_counter(line) < (if_list->tabs)){     //αν φτάσουμε σε else και έχουμε κόμβους με εμφωλευμένα if τους διαγράφουμε για να ελέγχεται η σωστή συνθήκη
                pop(&if_list);
                if(flag_error == 1){
                    break;
                }
            }
            if(if_list == NULL){							//αν δαβάσουμε else χωρίς να έχει διαβαστεί if σε προηγούμενη εντολή
                printf("Error! no \"if\" was found\n");
                break;
            }
            (if_list->start) = 1;		//έχει βρεθεί else
        }


        //break
        else if(strcmp(array[0], "break") == 0){
            if(count != 1){
                printf("Error! Wrong argument size.");
                break;
            }

            if(while_list == NULL){
                printf("Error! \"break\" cannot be used outside of a while-command block.");
                break;
            }
            else{
                flag_break = 1;
                continue;      
            }
        }


        //continue
        else if(strcmp(array[0], "continue") == 0){
            if(count != 1){
                printf("Error! Wrong argument size.");
                break;
            }
            if(while_list == NULL){
                printf("Error! \"continue\" cannot be used outside of a while-command block.");
                break;
            }
            else{
                fseek_successful = fseek(fp, (while_list->start), SEEK_SET); //move to the start of it (while ...)
                
                if(fseek_successful != 0){
                    printf("Error: Could not move to the start of the block successfully\n");
                }
            }
        }


        else{       //αν η γραμμη δεν ξεκιναει με οποιαδηποτε απο τις παραπανω εντολες, θα ξεκιναει με μεταβλητη
        	reread_line(line, checkpoint, fp);			//διαβάζει ξανα την τελευταία γραμμή γιατι έχει αλλοιωθεί από τη strtok
            count = expression_parsing(line, array);	//χωρίζει το expression σε tokens (υποστηρίζονται οι εκφράσεις του τύπου <varint1> <operator> <varint2> και <varint1><operator><varint2>)
            if(count == ERROR){
                break;
            }
            v_address = node_address(variable_list, array[0]);

            if(v_address == NULL){
                if(check_validity(array[0]) == ERROR){
                    break;
                }
                add_listnode(&variable_list, array[0], 0); 

                v_address = node_address(variable_list, array[0]);
            }
            

            if(count == 3){       //αν η παρασταση ειναι της μορφης x = 5 => 3 tokens
                v_address->value = assign_value(array[2], variable_list);
                
            }

            else if(count == 5){          //αν η παρασταση ειναι της μορφης x = y + 5 => 5 tokens
                result = calculate_expression(array[2], array[3], array[4], variable_list);
                if(result.error == -1){
                    break;
                }
                v_address->value = result.value;
            }
        }
    }

    while(if_list != NULL){     //κάνει free τη μνήμη στην περίπτωση που η στοίβα δεν είναι κενή μετά το τέλος του προγράμματος
        pop(&if_list);
        if(flag_error == 1){
            break;
        }
    }
    free_list(variable_list);
    fclose(fp);
    printf("\n");
    return 0;
}