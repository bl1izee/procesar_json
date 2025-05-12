 /*
******************************
*	Créditos: Pablo Martín	 *
******************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	char* nombreCalle;
	int piso;
	char puerta;
} calle_t;

typedef struct{
	char* nombre;
	int edad;
	calle_t calle;
} ciudadano_t;

char* leeLineaDinamicaFd(FILE* fd);
char* valorChar(char* cadena);
int valorInt(char* cadena);

int main(int argc, char* argv[]){
	// Variables para controlar el archivo
	FILE* fd;
	char* nombreFd = argv[1];
	
	fd = fopen(nombreFd, "r");
	
	if( fd == NULL ){
		printf("[-] Error al abrir el archivo %s", nombreFd);
		return 1;
	}
	
	// Variables para controlar los ciudadanos
	ciudadano_t* ciudadanos = (ciudadano_t*)malloc(sizeof(ciudadano_t));
	int numCiudadanos = 0;
	
	while( !feof(fd) ){
		char* lineaFd = leeLineaDinamicaFd(fd); // Guarda la linea
		
		if( !feof(fd) ){ // Evita que se ejecute cuando sea salto de linea
		
			char lineaFdTmp[strlen(lineaFd)+1]; // Guarda la linea original para poder después guardar la calle
			strcpy(lineaFdTmp, lineaFd); 
			
			// Guarda los datos del ciudadano
			ciudadanos[numCiudadanos].nombre = valorChar(strtok(lineaFd, ","));
			ciudadanos[numCiudadanos].edad = valorInt(strtok(NULL, ","));
			
			char* lineaFdCalle = strstr(lineaFdTmp, "Direccion")+strlen("Direccion")+2; // Guarda la cadena de la dirección
			
			// Guarda los datos de la calle 
			ciudadanos[numCiudadanos].calle.nombreCalle = valorChar(strtok(lineaFdCalle, ","));
			ciudadanos[numCiudadanos].calle.piso = valorInt(strtok(NULL, ","));
			char* puerta = valorChar(strtok(NULL, ","));
			ciudadanos[numCiudadanos].calle.puerta = puerta[0];
			
			// Redimensionamos el array de los ciudadanos
			numCiudadanos++;
			ciudadanos = (ciudadano_t*)realloc(ciudadanos, (numCiudadanos+1) * sizeof(ciudadano_t));
		}
	}
	
	for(int pos=0; pos < numCiudadanos; pos++){
		printf("\n[+] Nombre: %s\n", ciudadanos[pos].nombre);
		printf("[+] Edad: %d\n", ciudadanos[pos].edad);
		printf("[+] Calle: %s\n", ciudadanos[pos].calle.nombreCalle);
		printf("[+] Piso: %d\n", ciudadanos[pos].calle.piso);
		printf("[+] Puerta: %c\n", ciudadanos[pos].calle.puerta);
		
		if( pos != numCiudadanos-1 )printf("\n----------------\n");
	}
	
	free(ciudadanos);
	
	return 0;
}

char* leeLineaDinamicaFd(FILE* fd){
	char* cadena = (char*)malloc(sizeof(char));
	char letra;
	int tamCadena=0;
	
	while( ((letra = fgetc(fd)) != '\n') && (letra != EOF) ){
		cadena[tamCadena] = letra;
		
		tamCadena++;
		cadena = (char*)realloc(cadena, (tamCadena+1) * sizeof(char));
	}
	cadena[tamCadena] = '\0';

	return cadena;
}

char* valorChar(char* cadena){
	cadena = strchr(cadena, ':') + 1; // Se le suma 1 para saltar el : y "
	cadena = strchr(cadena, '"') + 1;
	
	char* nuevaCadena = (char*)malloc(sizeof(char));
	char letra;
	int pos=0;
	
	while( (letra = cadena[pos]) != '"' ){
		nuevaCadena[pos] = letra;
		
		pos++;
		nuevaCadena = (char*)realloc(nuevaCadena, (pos+1) * sizeof(char));
	}
	nuevaCadena[pos] = '\0';
	
	return nuevaCadena;
}

int valorInt(char* cadena){
	cadena = strchr(cadena, ':') + 1; // Se le suma 1 para saltar el : y "
	
	return strtol(cadena,NULL,10);
}