#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#define Hilos 3




int Gasolina;
char buffer[20];
int tamBuffer =0;
/*Mutual Exclusion*/
pthread_mutex_t mxShared;

void* Llenarbuffer(char  mensaje[200], int valor)
{
   if (tamBuffer<=20)
   { 
   /* char svalor[20];
    sprintf( svalor,"%d" , valor);*/
    buffer[tamBuffer]=valor;
    tamBuffer++;
   }
}

void* more_Gasolina(void* args)
{
    while(1)
    {
        pthread_mutex_lock(&mxShared);
        Gasolina = Gasolina + 5;
        printf("\nllenando Gasolina\n");
        Llenarbuffer("llenar gasolina ",Gasolina);
        pthread_mutex_unlock(&mxShared);
        sleep(7);
        if (Gasolina > 20)
        {
        printf("\nGasolinera llena\n");    
        return NULL;
        }
    }
}

void* gasolina_Carrito(void* args)
{
   int espera=0;
    while(1)
    {
        if (Gasolina >= 1)
        {
            pthread_mutex_lock(&mxShared);
            Gasolina = Gasolina - 1;
            sleep(2);
            pthread_mutex_unlock(&mxShared);
            printf("\nCarrito tanquedo\n");
            Llenarbuffer("llenar carrito ",Gasolina);
            return NULL;
        }
        else
        {
            printf("\nEspere por favor, le estamos tanqueando\n");
        }
        sleep(1);
        espera=espera+1;
        if (espera>20){
            printf("\ntiempo de espera agotado\n");
            Llenarbuffer("carrito abandono ",Gasolina);
            return NULL;
        }

           
    }
    
}

void* gasolina_Camioneta(void* args)
{
   int espera=0;
    while(1)
    {
        if (Gasolina >= 5)
        {
            pthread_mutex_lock(&mxShared);
            Gasolina = Gasolina - 5;
            sleep(2);
            pthread_mutex_unlock(&mxShared);
            printf("\nCamioneta tanqueda\n");
            Llenarbuffer("llenar camioneta ",Gasolina);
            return NULL;
        }
        else
        {
            printf("\nEspere por favor, le estamos tanqueando\n");
        }
        sleep(1);
        espera=espera+1;
        if (espera>20){
            printf("\ntiempo de espera agotado\n");
            Llenarbuffer("camioneta abandono ",Gasolina);
            return NULL;
        }
    }
    
}

void* cantidad_Gasolina(void* args)
{
    while(1)
    {
        sleep(30);
        printf("\nEn este momento hay: %i galones de gasolina\n", Gasolina);
        if (Gasolina > 20)
        {
        return NULL;
        }
    }
    
}


int main(int argc, char const *argv[])
{
     Gasolina = 20;
    pthread_t hilo[Hilos];
    char strlen[10];

    for (int i = 0; i < Hilos; i++)
    {
        hilo[i]=NULL;
    }


    pthread_create(&hilo[0], NULL, &more_Gasolina, NULL);
while(1)
{
    printf("Digite por favor qué carro tiene. Carrito / Camioneta / Cierre \n");
    gets(strlen);

    if (!strcmp(strlen , "Carrito"))
    {
        pthread_create(&hilo[1], NULL, &gasolina_Carrito, NULL);
    }

    if (!strcmp(strlen , "Camioneta"))
    {
    pthread_create(&hilo[2], NULL, &gasolina_Camioneta, NULL);
    }
       if (!strcmp(strlen , "Cierre"))
    { 
        printf("\nCiere bomba\n");
        break;
    }
 
}



    
    /*
    while (tacos < 600)
    {
        tacos = tacos + 5;
        sleep(20);
    }
    */

    
    for (int i = 0; i < Hilos; i++)
    {
        if(pthread_join(hilo[i],NULL) != 0)
        {
            perror("Failed to join");
        }
    }
    
for(int i=0;i<tamBuffer;i++)
{
    printf("Gasolina por operacion %i \n ", buffer[i]);
}

    pthread_mutex_destroy(&mxShared);
   return 0;
}