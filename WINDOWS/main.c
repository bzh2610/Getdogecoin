/*Made by /u/bzh2610 on 18/05/14
Copyright Bassguitarman
*/


#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include  <signal.h>
#include <windows.h>
#include <process.h>
#include <conio.h>

    char username [200]; //used to login
    char password [200]; //used to login
    char minersettings [200]; //options specified by user for miner
    char minercommand [200];  //command used by system() function.
    char miningsoftware [20]; //Used to choose between cuda/minerd/cgminer
    char identifier [200]="username=";
    int currentmining=0;
    char answer [2000];
    char defaultminercommand [200];
    //the following variables are used to miner for someone using getdogecoin

    char webserver [200];
    char webport [200];
    char webusername [200];
    char webpassword [200];


    HANDLE hThread4 = NULL;
    unsigned int idThread4;

    HANDLE hThread5 = NULL;
    unsigned int idThread5;
///////////////////

/*
THIS PART IS USED
TO LOG CURL
*/
struct string {
  char *ptr;
  size_t len;
};


void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

//buffer clean
void bufferclean()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

void makecommand (){
  if(strcmp(miningsoftware, "1")==0){//FOR CPUMINER
    system("Taskkill /F /IM minerd.exe");
    strcat(minercommand, "MINERD\\minerd.exe -o stratum+tcp://");
    strcat(minercommand, webserver);
    strcat(minercommand, ":");
    strcat(minercommand, webport);
    strcat(minercommand, " -O ");
    strcat(minercommand, webusername);
    strcat(minercommand, ":");
    strcat(minercommand, webpassword);

   // printf("%s", minercommand);
  }

    else if(strcmp(miningsoftware, "2")==0){//FOR CUDA
    system("Taskkill /F /IM cudaminer.exe");
    strcat(minercommand, "CUDAMINER\\cudaminer.exe -o stratum+tcp://");
    strcat(minercommand, webserver);
    strcat(minercommand, ":");
    strcat(minercommand, webport);
    strcat(minercommand, " -O ");
    strcat(minercommand, webusername);
    strcat(minercommand, ":");
    strcat(minercommand, webpassword);

   // printf("%s", minercommand);
  }


    else if(strcmp(miningsoftware, "3")==0){//FOR CUDA
    system("Taskkill /F /IM cgminer.exe");
    strcat(minercommand, "cgminer\\cgminer.exe -o stratum+tcp://");
    strcat(minercommand, webserver);
    strcat(minercommand, ":");
    strcat(minercommand, webport);
    strcat(minercommand, " -O ");
    strcat(minercommand, webusername);
    strcat(minercommand, ":");
    strcat(minercommand, webpassword);

   // printf("%s", minercommand);
  }
}


unsigned int __stdcall procedureThread4(void *pv){//default
    printf("%s", defaultminercommand);
    system(defaultminercommand);
    return 0;
}


unsigned int __stdcall procedureThread5(void *pv){//network
    printf("%s", minercommand);
    system(minercommand);
    return 0;
}


void startt4(){
    printf("\n\n************\n\nLAUNCHING DEFAULT MINING THREAD....\n\n");
		hThread4 = (HANDLE)_beginthreadex(NULL, 0, procedureThread4, NULL, 0, &idThread4);
        if(hThread4==NULL)
        {
		puts("Failure");
		return EXIT_FAILURE;
        }
}

void startt5(){
    printf("\n\n************\n\nLAUNCHING NETWORK MINING THREAD....\n\n");
		hThread5 = (HANDLE)_beginthreadex(NULL, 0, procedureThread5, NULL, 0, &idThread5);
        if(hThread5==NULL)
        {
		puts("Failure");
		return EXIT_FAILURE;
        }
}

unsigned int __stdcall procedureThread3(void *pv){

    if(currentmining==0)
        startt4();
    else if (currentmining==1)
        startt5();

    while(1){
    Sleep(10000);
    printf("Cheking informations on getdogecoin.com, mining will not be interupted\n");
    CURL *curl;
    CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    struct string s;
    init_string(&s);

    curl_easy_setopt(curl, CURLOPT_URL, "getdogecoin.com/clienttest.php");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, identifier);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);

    char answer [2000];
    FILE *fichier = fopen("test.txt", "w+");
    fprintf(fichier, "%s", s.ptr);
    fseek(fichier, 0, SEEK_SET);
    fscanf(fichier, "%s", answer);

    if(strcmp(answer, "ACCESS_GRANTED")==0){
    printf("CONNECTED\n\n");
    printf("Username : %s\n", username);
    fscanf(fichier, "%s", answer);
    printf("Hashrate: %s\n", answer);
    fscanf(fichier, "%s", answer);
    printf("Algo: %s\n", answer);
    fscanf(fichier, "%s", answer);
    printf("%s", answer);

    if(strcmp(answer, "1")==0){
    printf("Leased: True\n");
    fscanf(fichier, "%s", webserver);
    printf("Server: %s\n", webserver);
    fscanf(fichier, "%s", webport);
    printf("Port: %s\n", webport);
    fscanf(fichier, "%s", webusername);
    printf("User: %s\n", webusername);
    fscanf(fichier, "%s", webpassword);
    printf("Password: %s\n", webpassword);

    if (currentmining==0){
    currentmining=1;
    minercommand [0]='\0';
    makecommand();
	CloseHandle(hThread4); //STOPS DEFAULT MINING

    startt5();
    }}


    else{
    printf("Leased: False\n");
    if(currentmining == 1){
        currentmining=0;
        if(strcmp(miningsoftware, "1")==0)
        system("Taskkill /F /IM minerd.exe");

        else if(strcmp(miningsoftware, "2")==0)
        system("Taskkill /F /IM cudaminer.exe");

        else if(strcmp(miningsoftware, "3")==0)
        system("Taskkill /F /IM cgminer.exe");

    CloseHandle(hThread5);
    startt4();
    }
    }
    }//access granted
    }//if curl

}//while true
WaitForSingleObject(hThread4, INFINITE);
CloseHandle(hThread4);
WaitForSingleObject(hThread5, INFINITE);
CloseHandle(hThread5);
    return 0;
}


/*

MAIN

*/

int main(void){

HANDLE hThread = NULL;
	unsigned int idThread;

	puts("Getdogecoin client 0.0.1\n");
    printf("         A \n        AAA \n       AAAAA \n      AAAAAAA \n     |-------| \n     |-------| \n     |-------| \n     |-TO----| \n     |-THE---| \n     |-MOON--| \n     |-------| \n     |-------| \n     |-------| \n    (////|\\\\\\\\) \n   (\\\\\\\\|||////) \n    (__________) \n      ./VVVV\\. \n     ./VVVVVV\\. \n     .\\VVVVVV/. \n      .\\VVVV/. \n       .\\VV/. \n        .\\/.\n\n");
    printf("Username : ");
    scanf("%s", username);
    printf("Password : ");
    scanf("%s", password);
    system("cls");//erase the console (password)
    printf("\n\nCONNECTING TO GETDOGECOIN.COM ....   ");

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
    struct string s;
    init_string(&s);

    //Pack the variable into a string for the server request
    strcat(identifier, username);
    strcat(identifier, "&password=");
    strcat(identifier, password);

    curl_easy_setopt(curl, CURLOPT_URL, "getdogecoin.com/clienttest.php");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, identifier);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    res = curl_easy_perform(curl);

    FILE *fichier = fopen("test.txt", "w+");
    fprintf(fichier, "%s", s.ptr);
    fseek(fichier, 0, SEEK_SET);
    fscanf(fichier, "%s", answer);


    if(strcmp(answer, "ACCESS_GRANTED")==0){

    printf("CONNECTED\n\n");


    printf("Select your mining sofware :\n1)CPUMiner\n2)CudaMiner\n3)CGMiner\n4)Specify my own miner\n");
    scanf("%s", miningsoftware);
    printf("\n");

    if(strcmp(miningsoftware,"1")==0){
        printf("CPUMiner selected\n");
        strcat(defaultminercommand, "MINERD\\minerd.exe -o stratum+tcp://");
        printf("Complete this field as usual :\n\nminerd.exe -o stratum+tcp://");
        bufferclean();
        fgets(minersettings, 200, stdin);
        strcat(defaultminercommand, minersettings);
        }

        else if (strcmp(miningsoftware,"2")==0){
        printf("Cudaminer selected\n");
        strcat(defaultminercommand, "CUDAMINER\\cudaminer.exe -o stratum+tcp://");
        printf("Complete this field as usual :\n\cudaminer.exe -o stratum+tcp://");
        bufferclean();
        fgets(minersettings, 200, stdin);
        strcat(defaultminercommand, minersettings);
        }

        else if (strcmp(miningsoftware,"3")==0){
        printf("CGMiner selected\n");
        printf("Complete this field as usual :\n\cgminer.exe");
        bufferclean();
        fgets(minersettings, 200, stdin);
        strcat(defaultminercommand, minersettings);
        }

        else if (strcmp(miningsoftware,"4")==0){
        //will be done soon
        }


        else{
            printf("Invalid option");
        }

    printf("Username : %s\n", username);
    fscanf(fichier, "%s", answer);
    printf("Hashrate: %s\n", answer);
    fscanf(fichier, "%s", answer);
    printf("Algo: %s\n", answer);
    fscanf(fichier, "%s", answer);

      if(strcmp(answer, "1")==0){ //leased = true
        printf("Leased: True\n", answer);
        fscanf(fichier, "%s", webserver);
        printf("Server: %s\n", webserver);
        fscanf(fichier, "%s", webport);
        printf("Port: %s\n", webport);
        fscanf(fichier, "%s", webusername);
        printf("User: %s\n", webusername);
        fscanf(fichier, "%s", webpassword);
        printf("Password: %s\n", webpassword);

    minercommand [0]='\0';
    makecommand();
    currentmining=1;
    }


    else{  //not leased
    printf("Leased: False\n");
    }


    HANDLE hThread3 = NULL;
        unsigned int idThread3;
		hThread3 = (HANDLE)_beginthreadex(NULL, 0, procedureThread3, NULL, 0, &idThread3);
        if(hThread3==NULL)
        {
		puts("Failure");
		return EXIT_FAILURE;
        }

    fclose(fichier);
    free(s.ptr);
    curl_easy_cleanup(curl);

    WaitForSingleObject(hThread3, INFINITE);
	CloseHandle(hThread3);
    }

    else
        printf("Incorrect login");

}
return 0;
}


