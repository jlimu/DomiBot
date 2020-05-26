// Eloy Bardaji Pujadas 1461986


// Trivial Torrent

#include "file_io.h"
#include "logger.h"
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <openssl/sha.h>
#include <assert.h>

static const uint32_t MAGIC_NUMBER = 0xde1c3230;

static const uint8_t MSG_REQUEST = 0;
static const uint8_t MSG_RESPONSE_OK = 1;			// Respuestas posibles del server y peticion del cliente
static const uint8_t MSG_RESPONSE_NA = 2;

enum { RAW_MESSAGE_SIZE = 13 };

void u8from32i (uint8_t b[14], uint32_t u32);
void u8from64i (uint8_t b[18], uint64_t u64);		// Correccion de warnings por parte del compilador
unsigned sleep(unsigned seconds);
long int strtol(const char *str, char **endptr, int base);
int strcmp(const char * s1,const char * s2);
pid_t fork(void);
int close(int fd);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int load_block (const struct torrent_t * const torrent, const uint64_t block_number, struct block_t * const block);
int store_block (struct torrent_t * const torrent, const uint64_t block_number, const struct block_t * const block);
int destroy_torrent (struct torrent_t * const torrent);
int create_torrent_from_metainfo_file (char const * const metainfo_file_name, struct torrent_t * const torrent,char const * const downloaded_file_name);

void u8from32i (uint8_t b[14], uint32_t u32){		// Funcion de tranformacion de uint32_t a uint8_t
    b[0] = (uint8_t)u32;
    b[1] = (uint8_t)(u32>>=8);
    b[2] = (uint8_t)(u32>>=8);
    b[3] = (uint8_t)(u32>>=8);
}

void u8from64i (uint8_t b[18], uint64_t u64){		// Funcion de tranformacion de uint64_t a uint8_t
    b[5] = (uint8_t)u64;
    b[6] = (uint8_t)(u64>>=8);
    b[7] = (uint8_t)(u64>>=8);
    b[8] = (uint8_t)(u64>>=8);
    b[9] = (uint8_t)(u64>>=8);
    b[10] = (uint8_t)(u64>>=8);
    b[11] = (uint8_t)(u64>>=8);
    b[12] = (uint8_t)(u64>>=8);
}

int main(int argc, char** argv) {

	set_log_level(LOG_DEBUG);

	log_printf(LOG_INFO, "Trivial Torrent (build %s %s) by %s", __DATE__, __TIME__, "J. DOE and J. DOE");

	if(argc == 4){					// Si se le pasan 4 argumentos damos por sentado que se utilizara como servidor

		int esTorrent = 0;
		char *p = strrchr(argv[3], '.');
		if (p){
	    esTorrent = strcmp(p, ".ttorrent");		// Comparacion del archivo pasado con .ttorrent
		}

		if (esTorrent != 0)				// Si el archivo que se pasa no es un .ttorrent se acaba la ejecucion
		{
			printf("Los datos introducidos no son validos\n");
			return 1;
		}
		if (strcmp(argv[1], "-l") != 0)		// Si el argv[1] no es "-l" se acaba la ejecucion
		{
			printf("Los datos introducidos no son validos\n");
			return 1;
		}	

		char* pAux;
		long arg2 = strtol(argv[2], &pAux, 10);
		if (*pAux != '\0') {			// Si el argv[2] no es un puerto se acaba la ejecucion
			printf("Los datos introducidos no son validos\n");
		    return 1; 		
		}
		int argvToInt = (int)arg2;			// Utilizamos el argumento que se nos pasa como puerto para la ejecucion argv[2] --> Puerto

		if ((argvToInt > 49151) || (argvToInt < 1024))
		{
			printf("Los datos introducidos no son validos\n");
		    return 1; 
		}

		struct torrent_t torrent;
		if (create_torrent_from_metainfo_file(argv[3], &torrent, "download_file")) 
		{
			perror("CreateTorrent");		// Se crea la estructura torrent_t segun el fichero que tenemos
			return 1;
		}

		printf("El programa se va a ejecutar como servidor \n");

		struct sockaddr_in myaddr, clientaddr;
		int sockid;									// Creacion de la estructura del socket (igual que en el cliente)
		sockid = socket(AF_INET, SOCK_STREAM, 0);	// con la diferencia del uso de myaddr y clientaddr (Podriamos añadirle utilidades)
		memset(&myaddr, '0', sizeof(myaddr));
		myaddr.sin_family = AF_INET;
		myaddr.sin_port = htons((uint16_t)argvToInt);					// En este caso usamos la entrada argv[2] transformada
		myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		if (sockid == -1)
		{
			perror("socket");								// Si hay un error en la creacion del socket nos informa
		}
		int len = sizeof(myaddr);
		if (bind(sockid, (struct sockaddr*) & myaddr, (unsigned int)len) == -1)	// En el caso del servidor utilizamos bind y listen como funciones tcp
		{
			perror("bind");							// Usamos bind para asociar un socket con un puerto
		}
		if (listen(sockid, 10) == -1) 				// Usamos listen para que alguien pueda conectarse a nuestra maquina
		{
			perror("listen");					
		}

		int pid, new;			// Variable new para aceptar clientes, pid para el uso de forks, y i para la iteracion del bucle
		int i = 0;

		for (;;)				// Bucle infinito con el que recibir todos los clientes que deseemos
		{
			new = accept(sockid, (struct sockaddr*) & clientaddr, (unsigned int *)&len);		// Se utiliza para aceptar al cliente
			
			if ((pid = fork()) == -1)
			{
				close(new);					// Si hay error cerramos conexion y vamos con la siguiente iteracion
				continue;
			}
			else if (pid > 0)
			{
				close(new);					// Si es el padre cerramos conexion y vamos con la siguiente iteracion
				continue;
			}
			else if (pid == 0)				// Si es el hijo continuaremos con el proceso 
			{

				i = 0;

				while(i < (int)torrent.block_count)			// Una iteracion por cada bloque del archivo
				{

					uint8_t recibir[13];				// Utilizamos este buffer para recibir los datos por parte del cliente
					int bloque = -1;					// Inicializamos a -1 por posible error al recibir datos

					if(recv(new, &recibir, 13, 0) < 0)	// Si el recv falla, o no se reciben datos se cerrara la conexion y 
					{									// se continuara con la siguiente conexion
						close(new);
						break;
					}
					else
					{									// Si todo va bien recibiremos un bloque divido en 3(4b-1b-8b), 
						bloque = recibir[5];			// recibir[5] nos dira el bloque que esta solicitando el cliente 
						i = bloque;						// Igualamos i al bloque por posibles alteraciones en el orden de peticion del cliente
					}

					if (bloque == -1 || bloque > (int)torrent.block_count)
					{
						close(new);						// Medida de seguridad adicional ante posibles errores
						break;
					}

					if ((torrent.block_map[bloque]) && (recibir[4] == 0))	// Comprobamos que el bloque este en nuestro archivo disponible y 
					{														// ademas comprobamos que recibir[4] sea un MSG_REQUEST(0) 
				    	struct block_t block;								// Si es asi creamos un bloque auxiliar
				  
				        if (load_block(&torrent, (uint64_t)bloque, &block))			// Cargamos ese bloque con la informacion de nuestro fichero
				        {
				           printf("error load\n");							// Si algo va mal saltara un error de carga
				        }

				  		if (bloque == (int)torrent.block_count - 1)			// Si el bloque es el ultimo tenemos que hacer unos ajustes 
				  		{													// ya que el tamano no vendra predeterminado
				  			int tamano = (int)(torrent.downloaded_file_size - ((uint64_t)bloque * 65536)); // Calculo del tamano del fichero

				  			uint8_t mandar[13];
							uint64_t bnum = (uint64_t)bloque;				// Estructura para mandar la respuesta del tipo server
							uint8_t *puntM = NULL;

							u8from32i(mandar,MAGIC_NUMBER);					// Utilizamos funciones para adaptar los formatos
							mandar[4]=MSG_RESPONSE_OK;						// Estructura para mandar la respuesta del tipo server
							u8from64i(mandar,bnum);							// en este caso tenemos el bloque disponible MSG_RESPONSE_OK(1)

							puntM = mandar;
							send(new, puntM, 13, 0);						// Mandamos la respuesta anterior
					  		send(new, &block, (unsigned int)tamano, 0);		// Mandamos el contenido del bloque con el tamano calculado

					  		printf("El bloque %d", bloque);
							printf(" se ha enviado con exito.\n");
				  		}
				  		else{												// Si el bloque no es el ultimo lo trataremos como predefinido

				  			uint8_t mandar[13];
							uint64_t bnum = (uint64_t)bloque;				// Misma estructura que el anterior
							uint8_t *puntM = NULL;
							
							u8from32i(mandar,MAGIC_NUMBER);
							mandar[4]=MSG_RESPONSE_OK;						// Misma estructura que el anterior
							u8from64i(mandar,bnum);

							puntM = mandar;
							send(new, puntM, 13, 0);
					  		send(new, &block, 65536, 0);					// Mandamos el contenido del bloque con el tamano estandar

					  		printf("El bloque %d", bloque);
							printf(" se ha enviado con exito.\n");
				  		}	
				    }
				    else if (recibir[4] == 0)								// Si el bloque no esta en nuestro fichero le comunicaremos 
				    {														// al cliente que no lo tenemos
				    	uint8_t mandar[13];
						uint64_t bnum = (uint64_t)bloque;					// Estructura para mandar la respuesta del tipo server
						uint8_t *puntM = NULL;
						
						u8from32i(mandar,MAGIC_NUMBER);						// Estructura para mandar la respuesta del tipo server
						mandar[4]=MSG_RESPONSE_NA;							// en este caso tenemos el bloque disponible MSG_RESPONSE_NA(2)
						u8from64i(mandar,bnum);								// ya que no disponemos del archivo

						puntM = mandar;
						send(new, puntM, 13, 0);							// Mandamos la respuesta anterior y en este caso no mandaremos 
																			// el contenido del bloque ya que no lo tenemos
						printf("El bloque %d", bloque);
						printf(" no se encuentra en el servidor.\n");
				    }
				    else													// En el caso de que el cliente nos mande algo malicioso/erroneo
				    {
				   		printf("Se han recibido datos maliciosos o erroneos, el servidor procede a cerrarse.\n");
		    			return 1; 
				    }
					i++;													// Aumento del iterador
				}
				close(new);													// Cerramos el Accept(new)
				printf("El cliente ha cerrado la conexión con el servidor.\n");
				break;														// Salimos al bucle for para continuar 
			}
		}
		close(sockid);														// Cerramos el socket creado y finalizamos la ejecucion
		return 0;
	}
	else if (argc == 2){					// Si se le pasan 2 argumentos entramos como cliente (se pasa un file valido y no es server)

	int esTorrent = 0;
	char *p = strrchr(argv[1], '.');
	if (p){
    esTorrent = strcmp(p, ".ttorrent");
	}

	if (esTorrent != 0)				// Si el archivo que se pasa no es un .ttorrent se acaba la ejecucion
	{
		printf("Los datos introducidos no son validos\n");
		return 1;
	}							
	
	int j = 0;		// Variables para iterar 
	int i = 0;
	int puerto;		// Variable para almacenar los puertos recibidos del torrent

	struct torrent_t torrent;
	if (create_torrent_from_metainfo_file(argv[1], &torrent, "download_file")) {		// Creación del archivo download_file y del ttorrent
		perror("CreateTorrent");
		return 1;
	}

	printf("El programa se va a ejecutar como cliente \n");
	
	struct sockaddr_in myaddr;					// Estructura de nuestro cliente TCP
	int sockid;
												// Creacion de la estructura TCP para conectarnos con el servidor
	memset(&myaddr, '0', sizeof(myaddr));		// Memset de la estructura myaddr (Por si acaso)
	myaddr.sin_family = AF_INET;
	
	int errorPeer = 1;				// Variable que controlara si es posible la conexion con la IP y su respectivo puerto
	int primerBloque = 0;			// Variable que controlara si es el primer bloque que descargamos en la ejecucion
	int r = -1;							// Variable que controla si el store se ha realizado correctamente o no

	while(j < (int)torrent.peer_count){ 		// Se iterara dependiendo del numero de peers que tenga disponible la estructura torrent

		i = 0;
		sockid = socket(AF_INET, SOCK_STREAM, 0);		// Creacion del socket para poder recibir archivos del servidor
		puerto = htons(torrent.peers[j].peer_port);			// Htons utilizada para dar formato correcto al puerto que obtenemos
		myaddr.sin_port = htons((uint16_t)puerto);				// Asignacion de los valores a la estructura TCP para conectarnos con el servidor
		myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");	// Direccion a la que conectarse, en este caso 127.0.0.1 para utilizar en local

		if (sockid == -1)
		{
			perror("socket");				// Printa error si el socket no se crea correctamente
		}
		int len = sizeof(myaddr);
		if (connect(sockid, (const struct sockaddr*) & myaddr, (unsigned int)len) == -1)	// Se conecta con el servidor y si no puede nos dice que ese puerto no esta disponible
		{
			errorPeer=0;
			//perror("connect");
			printf("La conexion con el puerto %d", puerto);
			printf(" no dispone de un Server activo\n");
		}
		else
		{
			printf("La conexion con el puerto %d", puerto);
			printf(" esta activa\n");						// Se conecta con el servidor correctamente
			errorPeer=1;
		}

		while(i < (int)torrent.block_count){		// Se iterara dependiendo del numero de bloques que tenga disponible la estructura torrent
			if (errorPeer == 1)				// Si el puerto esta disponible...
			{
				if (torrent.block_map[i]) 
				{							// Si el archivo ya esta descargado no interactuamos con el servidor
					printf("El bloque %d", i);	
					printf(" ya esta descargado.\n");
					primerBloque = 1;			
				}
				else
				{
					uint8_t recibir[13];			// Creacion de variables para mandar y recibir datos del servidor
					uint8_t mandar3[13];
					uint64_t bnum3 = (uint64_t)i;
					uint8_t *puntM3 = NULL;
					
					u8from32i(mandar3,MAGIC_NUMBER);	// Se crea la estructura compuesta por MAGIC_NUMBER , MSG_REQUEST , BLOCK_NUMBER
					mandar3[4]=MSG_REQUEST;				// que haremos con unas funciones que transforman uints en uint8_t
					u8from64i(mandar3,bnum3);

					puntM3 = mandar3;
					send(sockid, puntM3, 13, 0); 	// Se manda la informacion del bloque requerido al servidor

					printf("Recibiendo el bloque %d", i);	
					printf("...\n");

					if (i == 0 )		// Se es la primera iteracion
					{
						struct block_t another_block;		// Se crea la estructura con la que leeremos y almacenaremos en memoria
						recv(sockid, &recibir, 13, 0);
						if (recibir[4] != 1 || bnum3 != recibir[5])			// Si el mensaje de respuesta es MSG_RESPONSE_NA (un 2) entonces seguimos con 
						{													// la siguiente iteracion o si el bloque recibido no es el que hemos solicitado.
						i++;
						printf("Error en la descarga o almacenaje del bloque.\n");
						primerBloque = 0;
						continue;
						}
						recv(sockid, &another_block.data, 65536, 0); 		// Se lee del servidor diferentes informaciones,
						recv(sockid, &another_block.data[65523], 13, 0); 	// datos y una respuesta al mensaje mandado anteriormente 

						another_block.size = sizeof(another_block.data);	// Se le asigna el tamaño correcto al bloque creado anteriormente

						if (recibir[4] == 1)			// Si el mensaje de respuesta es MSG_RESPONSE_OK (un 1) entonces nos disponemos a almacenar el bloque
						{
						r = store_block(&torrent, 0, &another_block);
						}
						primerBloque = 0;		// Deja de ser el primer bloque en descargar si es que lo era
					}
					else			// Las explicaciones en los siguientes apartados son similares a la anteriormente explicada
					{
						if ((int)torrent.block_count - 1 == i && primerBloque == 0)	// Si es el ultimo bloque y ademas no es el primer bloque en descargarse
						{	
							struct block_t another_block;		// Se crea la estructura con la que leeremos y almacenaremos en memoria...
							
							int tamano = ((int)torrent.downloaded_file_size - (i * 65536));	// En este caso el size del bloque que descargamos no tiene porque ser el maximo,
																						// por lo que necesitaremos una variable auxiliar para controlar este parametro de entrada
							another_block.size = (uint64_t)tamano;

							recv(sockid, &recibir, 13, 0);
							if (recibir[4] != 1 || bnum3 != recibir[5])			// Si el mensaje de respuesta es MSG_RESPONSE_NA (un 2) entonces seguimos con 
							{													// la siguiente iteracion o si el bloque recibido no es el que hemos solicitado.
							i++;
							printf("Error en la descarga o almacenaje del bloque.\n");
							continue;
							}
							recv(sockid, &another_block.data, (unsigned int)tamano, 0);

							if (recibir[4] == 1)
							{
							r = store_block(&torrent, (uint64_t)i , &another_block);
							}

						}
						if ((int)torrent.block_count - 1 == i && primerBloque == 1)	// Explicacion similar en el anterior if
						{	
							struct block_t another_block;
							recv(sockid, &recibir, 13, 0);

							if (recibir[4] != 1 || bnum3 != recibir[5])			// Si el mensaje de respuesta es MSG_RESPONSE_NA (un 2) entonces seguimos con 
							{													// la siguiente iteracion o si el bloque recibido no es el que hemos solicitado.
							i++;
							printf("Error en la descarga o almacenaje del bloque.\n");
							continue;
							}

							int tamano = (int)(torrent.downloaded_file_size - ((uint64_t)i * 65536));

							another_block.size = (uint64_t)tamano;

							recv(sockid, &another_block.data, (unsigned int)tamano, 0);

							if (recibir[4] == 1)
							{
							r = store_block(&torrent, (uint64_t)i , &another_block);
							}
							primerBloque = 0;
						}
						if ((int)torrent.block_count - 1 != i && primerBloque == 0)	// Es un bloque que no es ni el primero ni el ultimo
						{														// y no es el primero en descargarse de la ejecucion
							struct block_t another_block;
							recv(sockid, &recibir, 13, 0);

							if (recibir[4] != 1 || bnum3 != recibir[5])			// Si el mensaje de respuesta es MSG_RESPONSE_NA (un 2) entonces seguimos con 
							{													// la siguiente iteracion o si el bloque recibido no es el que hemos solicitado.
							i++;
							printf("Error en la descarga o almacenaje del bloque.\n");
							continue;
							}
							recv(sockid, &another_block.data, 65536, 0);		// Recibe los datos como en las anteriores comparaciones
							recv(sockid, &another_block.data[65523], 13, 0);

							another_block.size = sizeof(another_block.data);	// Size maximo permitido

							if (recibir[4] == 1) 
							{
							r = store_block(&torrent, (uint64_t)i , &another_block); 
							}

						}
						if ((int)torrent.block_count - 1 != i && primerBloque == 1)	// Es un bloque que no es ni el primero ni el ultimo
						{														// pero es el primero en descargarse de la ejecucion
							struct block_t another_block;
							recv(sockid, &recibir, 13, 0);
							if (recibir[4] != 1 || bnum3 != recibir[5])			// Si el mensaje de respuesta es MSG_RESPONSE_NA (un 2) entonces seguimos con 
							{													// la siguiente iteracion o si el bloque recibido no es el que hemos solicitado.
							i++;
							printf("Error en la descarga o almacenaje del bloque.\n");
							continue;
							}
							recv(sockid, &another_block.data, 65536, 0);
							recv(sockid, &another_block.data[65523], 13, 0);	// Recibe los datos de la misma forma que en el primer if (i==0)
							
							another_block.size = sizeof(another_block.data);

							if (recibir[4] == 1) 
							{
							r = store_block(&torrent, (uint64_t)i , &another_block); 		// Store normal como en las demas comparaciones
							}
							primerBloque = 0;
						}
					}

					if (r == -1) {
				     	printf("Error en la descarga o almacenaje del bloque.\n");	// Error en la descarga o almacenaje del bloque debido al valor de r
					}																// que recibe del store_block
					else 															
					{
				    	printf("Bloque descargado y almacenado con exito.\n");		// Bloque descargado y almacenado con exito
						assert(torrent.block_map[i]);
					}
				}	
			}
			i = i+1;		// Se itera el bucle de bloques
		}

		if (errorPeer == 1)
		{
			printf("La conexion con el puerto %d", puerto);		// Se informa del final de la conexion
			printf(" ha finalizado\n");
		}

		close(sockid);											// Se finaliza la conexion con el servidor cerrando el socket
		j = j+1;			// Se itera el bucle de peers
	}
	
	if (destroy_torrent(&torrent)) 		// Destruccion y liberacion de memoria de la estructura torrent creada anteriormente
	{
		perror("DestruirTorrent");		
	}

	sleep(1);							// Esperamos un segundo y salimos
	return 0;
	}
}

