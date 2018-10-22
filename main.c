#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define UNDERSCORE 256

typedef struct transizione{
    char letto;
    char scritto;
    char spostamento;
    long int statoProssimo;
    unsigned short int numTransizioni;
    struct transizione *prossimaTransizioneOrizzontale;// transizioni con carattere letto differente
    struct transizione *prossimaTransizioneVerticale;// transizioni con carattere letto uguale
}Transizione;

typedef struct tempState{
    long int numeroStato;
    struct tempState *nextStateOrizontal;
    char *stringa;
    int posizioneStringa;
    size_t lunghezzaStringa;
}TempState;

long int acceptControl = 4, *accept, maxState=0;
unsigned int max;
Transizione *transitionPointer, *struttura[5000];
TempState *tempHead=NULL;
char *stringa_ = "________________________________________________________________________________________________________________________________________________________________________________________________________";

// FUNZIONE PER LETTURA DELL'INPUT
void read(char* input){
    int i=0;
    input[i] = (char) getchar();
    while(input[i] != '\n' &&  input[i]!= EOF){
        i++;
        input[i] = (char) getchar();
    }
    i++;
    input[i] = '\0';
}

// FUNZIONE PER LIBERARE LE ULTIME STRUCT DOPO AVER DECISO SE STAMPARE 1/0/U
void lastFree(struct tempState *tempState1){
    TempState *ts;
    while (tempState1 != NULL) {
        ts = tempState1;
        tempState1 = tempState1->nextStateOrizontal;
        free(ts->stringa);
        free(ts);
    }
    while (tempHead != NULL){
        ts = tempHead;
        tempHead = tempHead->nextStateOrizontal;
        free(ts->stringa);
        free(ts);
    }
}

// FUNZIONE PER COSTRUIRE LA STRUTTURA DATI CONTENENTE LE TRANSIONI
void insertTransition (long int stato){
    char *character;
    struct transizione *temp;

    if (stato > maxState){
        maxState = stato;
    }

    if (struttura[stato] == NULL){
        temp = (Transizione*) malloc (sizeof(Transizione));
        temp->prossimaTransizioneOrizzontale = NULL;
        temp->prossimaTransizioneVerticale = NULL;
        temp->numTransizioni = 1;
        character = strtok(NULL," ");
        temp->letto = *character;
        character = strtok(NULL," ");
        temp->scritto = *character;
        character = strtok(NULL," ");
        temp->spostamento = *character;
        character = strtok(NULL," ");
        temp->statoProssimo = strtol(character, NULL, 10);
        struttura[stato] = temp;
    } else{
        character = strtok(NULL," ");
        transitionPointer = struttura[stato];
        while (transitionPointer != NULL){
            if (transitionPointer->letto == *character){
                transitionPointer->numTransizioni++;
                temp = (Transizione*) malloc (sizeof(Transizione));
                temp->numTransizioni = 0;
                temp->prossimaTransizioneOrizzontale = NULL;
                temp->prossimaTransizioneVerticale = NULL;
                temp->letto = *character;
                character = strtok(NULL," ");
                temp->scritto = *character;
                character = strtok(NULL," ");
                temp->spostamento = *character;
                character = strtok(NULL," ");
                temp->statoProssimo = strtol(character, NULL, 10);
                while (transitionPointer->prossimaTransizioneVerticale != NULL){
                    transitionPointer = transitionPointer->prossimaTransizioneVerticale;
                }
                transitionPointer->prossimaTransizioneVerticale = temp;
                break;
            }
            transitionPointer = transitionPointer->prossimaTransizioneOrizzontale;
        }
        if (transitionPointer == NULL){
            temp = (Transizione*) malloc (sizeof(Transizione));
            temp->prossimaTransizioneVerticale = NULL;
            temp->prossimaTransizioneOrizzontale = NULL;
            temp->numTransizioni = 1;
            temp->letto = *character;
            character = strtok(NULL," ");
            temp->scritto = *character;
            character = strtok(NULL," ");
            temp->spostamento = *character;
            character = strtok(NULL," ");
            temp->statoProssimo = strtol(character, NULL, 10);

            transitionPointer = struttura[stato];
            temp->prossimaTransizioneOrizzontale = transitionPointer;
            struttura[stato] = temp;
        }
    }
}
// FUNZIONE PER VERIFICARE SE LO STATO PASSATO COME PARAMETRO è UNO STATO DI ACCETTAZIONE
int isAccept(long int acceptState){
    int i=0;
    while (accept[i] != -1){
        if (accept[i]==acceptState) {
            return 1;
        }
        i++;
    }
    return 0;
}

// FUNZIONE PER COMPUTARE LA STRINGA CHE RESTITUISCE 1/0/U
void compute (char *string, int stringLenght) {
    unsigned int numMosse = 0;
    unsigned int i = 0, j = 0, numTrans = 0;
    unsigned short int loop = 0, flagLoop = 0, flag=0;
    char *temp;
    TempState *tempState1 = NULL, *tempState2 = NULL, *tempState3 = NULL, *tempState4 = NULL;

    tempHead = NULL;
    tempState1 = (TempState *) malloc(sizeof(TempState));
    tempState1->nextStateOrizontal = NULL;
    tempState1->numeroStato = 0;
    tempState1->posizioneStringa = 0;
    tempState1->lunghezzaStringa = (size_t) stringLenght;
    tempState1->stringa = (char *) malloc((size_t) (stringLenght + 1));
    tempState1->stringa[0] = '\0';
    strcpy(tempState1->stringa, string);

    while (tempState1 != NULL) {
        if (isAccept(tempState1->numeroStato) == 1) {
            lastFree(tempState1);
            printf("1\n");
            return;
        }
        transitionPointer = struttura[tempState1->numeroStato];
        while (transitionPointer != NULL &&
               transitionPointer->letto != tempState1->stringa[tempState1->posizioneStringa]) {
            transitionPointer = transitionPointer->prossimaTransizioneOrizzontale;
        }
        //uscito dal ciclo trpointer punta alla transizone con la lettera corrisponente o a NULL se non l'ha trovata
        if (transitionPointer != NULL) {
            numTrans = transitionPointer->numTransizioni;
            j = 0;
            while (transitionPointer != NULL) {
                if(numMosse > max){
                    if (j == numTrans - 1) {
                        flagLoop = 1;
                    }
                    loop=1;
                } else if (transitionPointer->spostamento == 'S' && transitionPointer->scritto == transitionPointer->letto &&
                           tempState1->numeroStato == transitionPointer->statoProssimo) {
                    if (j == numTrans - 1) {
                        flagLoop = 1;
                    }
                    loop = 1;
                } else if (transitionPointer->letto == '_' &&
                           transitionPointer->statoProssimo == tempState1->numeroStato) {
                    if (j == numTrans - 1) {
                        flagLoop = 1;
                    }
                    loop = 1;
                } else {
                    if (j == numTrans - 1) {
                        tempState2 = tempState1;
                        tempState2->numeroStato = transitionPointer->statoProssimo;

                        if (transitionPointer->spostamento == 'L' && tempState2->posizioneStringa == 0) {
                            temp = (char *) malloc(sizeof(char) * (tempState2->lunghezzaStringa + 1));
                            temp[0] = '\0';
                            strcpy(temp,tempState2->stringa);
                            tempState2->stringa = (char *) realloc(tempState2->stringa,
                                                                   tempState2->lunghezzaStringa + UNDERSCORE + 1);
                            for (i = UNDERSCORE; i > 0; i--) {
                                tempState2->stringa[i - 1] = '_';
                            }
                            tempState2->stringa[UNDERSCORE + i] = '\0';
                            strcat(tempState2->stringa, temp);
                            free(temp);
                            tempState2->stringa[UNDERSCORE] = transitionPointer->scritto;
                            tempState2->posizioneStringa = UNDERSCORE - 1;
                            tempState2->lunghezzaStringa = tempState2->lunghezzaStringa + UNDERSCORE;
                        } else if (transitionPointer->spostamento == 'R' &&
                                   tempState2->posizioneStringa == tempState2->lunghezzaStringa - 1) {
                            tempState2->posizioneStringa++;
                            tempState2->stringa = (char *) realloc(tempState2->stringa,
                                                                   (size_t) (tempState2->posizioneStringa + UNDERSCORE +
                                                                             1));
                            tempState2->stringa[tempState2->posizioneStringa - 1] = transitionPointer->scritto;
                            strcat(tempState2->stringa, stringa_);
                            tempState2->stringa[tempState2->posizioneStringa + UNDERSCORE] = '\0';
                            tempState2->lunghezzaStringa = tempState2->lunghezzaStringa + UNDERSCORE;
                        } else if (transitionPointer->spostamento == 'L') {
                            tempState2->stringa[tempState2->posizioneStringa] = transitionPointer->scritto;
                            tempState2->posizioneStringa--;
                        } else if (transitionPointer->spostamento == 'R') {
                            tempState2->stringa[tempState2->posizioneStringa] = transitionPointer->scritto;
                            tempState2->posizioneStringa++;
                        } else {
                            tempState2->stringa[tempState2->posizioneStringa] = transitionPointer->scritto;
                        }

                    } else {
                        tempState2 = (TempState *) malloc(sizeof(TempState));
                        tempState2->nextStateOrizontal = NULL;
                        tempState2->numeroStato = transitionPointer->statoProssimo;
                        tempState2->posizioneStringa = tempState1->posizioneStringa;
                        tempState2->lunghezzaStringa = tempState1->lunghezzaStringa;

                        if (transitionPointer->spostamento == 'L' && tempState2->posizioneStringa == 0) {
                            tempState2->stringa = (char *) malloc(tempState2->lunghezzaStringa + UNDERSCORE + 1);
                            for (i = UNDERSCORE; i > 0; i--) {
                                tempState2->stringa[i - 1] = '_';
                            }
                            tempState2->stringa[UNDERSCORE + i] = '\0';
                            strcat(tempState2->stringa, tempState1->stringa);
                            tempState2->stringa[UNDERSCORE] = transitionPointer->scritto;
                            tempState2->posizioneStringa = UNDERSCORE - 1;
                            tempState2->lunghezzaStringa = tempState2->lunghezzaStringa + UNDERSCORE;
                        } else if (transitionPointer->spostamento == 'R' &&
                                   tempState2->posizioneStringa == tempState1->lunghezzaStringa - 1) {
                            tempState2->posizioneStringa++;
                            tempState2->stringa = (char *) malloc(
                                    (size_t) (tempState2->posizioneStringa + UNDERSCORE + 1));
                            tempState2->stringa[0] = '\0';
                            strcpy(tempState2->stringa, tempState1->stringa);
                            tempState2->stringa[tempState2->posizioneStringa - 1] = transitionPointer->scritto;
                            strcat(tempState2->stringa, stringa_);
                            tempState2->stringa[tempState2->posizioneStringa + UNDERSCORE] = '\0';
                            tempState2->lunghezzaStringa = tempState2->lunghezzaStringa + UNDERSCORE;
                        } else if (transitionPointer->spostamento == 'L') {
                            tempState2->stringa = (char *) malloc(tempState1->lunghezzaStringa + 1);
                            tempState2->stringa[0] = '\0';
                            strcpy(tempState2->stringa, tempState1->stringa);
                            tempState2->stringa[tempState2->posizioneStringa] = transitionPointer->scritto;
                            tempState2->posizioneStringa--;
                        } else if (transitionPointer->spostamento == 'R') {
                            tempState2->stringa = (char *) malloc(tempState1->lunghezzaStringa + 1);
                            tempState2->stringa[0] = '\0';
                            strcpy(tempState2->stringa, tempState1->stringa);
                            tempState2->stringa[tempState2->posizioneStringa] = transitionPointer->scritto;
                            tempState2->posizioneStringa++;
                        } else {
                            tempState2->stringa = (char *) malloc(tempState1->lunghezzaStringa + 1);
                            tempState2->stringa[0] = '\0';
                            strcpy(tempState2->stringa, tempState1->stringa);
                            tempState2->stringa[tempState2->posizioneStringa] = transitionPointer->scritto;
                        }
                    }

                    if (tempHead == NULL) {
                        tempHead = tempState2;
                    } else {
                        tempState3->nextStateOrizontal = tempState2;
                    }
                    tempState3 = tempState2;
                    j++;
                }
                transitionPointer = transitionPointer->prossimaTransizioneVerticale;
            }
        } else {
            flag = 1;
            //se flag è a 1 significa che ts1 non ha figli e devo freearlo
        }
        if (tempState1->nextStateOrizontal != NULL) {//scorro a dx ts1
            tempState4 = tempState1;
            tempState1 = tempState1->nextStateOrizontal;
            tempState4->nextStateOrizontal = NULL;
            if (flag == 1 || flagLoop == 1) {
                free(tempState4->stringa);
                free(tempState4);
                flag = 0;
                flagLoop = 0;
            }
            //tempState4 = NULL;
        } else { //sposto ts1 al piano inferiore
            if (flag == 1 || flagLoop == 1) {
                if (numMosse != 0) {
                    tempState3->nextStateOrizontal = NULL;
                }
                free(tempState1->stringa);
                free(tempState1);
                flag = 0;
                flagLoop = 0;
            }
            if (tempHead == NULL) {
                if (loop == 0)
                    printf("0\n");
                else {
                    printf("U\n");
                }
                return;
            }
            tempState1 = tempHead;
            tempHead = NULL;
            numMosse++;
        }
    }
}


void main() {
    char input[30], *token, *stringaLetta = NULL;
    long int tempState;
    int i;
    read(input);
    read(input);
    while (input[0] != 'a') {
        token = strtok(input, " ");
        tempState = strtol(token, NULL, 10);
        insertTransition(tempState);
        read(input);
    }
    //STATI ACCETTAZIONE
    read(input);
    accept = (long int*) malloc (sizeof(long int)*acceptControl);
    i=0;
    while (input[0] != 'm'){
        if(i == acceptControl-1){
            acceptControl = acceptControl*2;
            accept = (long int*) realloc (accept,sizeof(long int)*acceptControl);
        }
        accept[i] = strtol(input,NULL,10);
        read(input);
        i++;
    }
    accept[i]= -1;
    //LEGGO MAX CARATTERI
    read(input);
    max = (unsigned int) strtol(input, NULL, 10);
    read(input); //leggo run
    //LEGGO STRINGHE E COMPUTO
    while (scanf("%ms", &stringaLetta) != EOF) {
        compute(stringaLetta, (int) strlen(stringaLetta));
        free(stringaLetta);
    }
    free(stringaLetta);
    free(accept);
}