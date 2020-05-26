// Eloy Bardaji Pujadas 1461986

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
static const uint8_t MSG_RESPONSE_OK = 1;
static const uint8_t MSG_RESPONSE_NA = 2;

enum { RAW_MESSAGE_SIZE = 13 };

void u8from32i (uint8_t b[14], uint32_t u32){
    b[0] = (uint8_t)u32;
    b[1] = (uint8_t)(u32>>=8);
    b[2] = (uint8_t)(u32>>=8);
    b[3] = (uint8_t)(u32>>=8);
}

void u8from64i (uint8_t b[18], uint64_t u64){
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

	if(argc == 4){

		struct torrent_t torrent;
		if (create_torrent_from_metainfo_file("test_file.ttorrent", &torrent, "download_file")) 
		{
			perror("CreateTorrent");
		}

		char* pAux;
		long arg2 = strtol(argv[2], &pAux, 10);
		if (*pAux != '\0') {
		    return 1; 
		}
		int argvToInt = arg2;

		struct sockaddr_in myaddr, clientaddr;
		int sockid;
		sockid = socket(AF_INET, SOCK_STREAM, 0);
		memset(&myaddr, '0', sizeof(myaddr));
		myaddr.sin_family = AF_INET;
		myaddr.sin_port = htons(argvToInt);
		myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		if (sockid == -1)
		{
			perror("socket");
		}
		int len = sizeof(myaddr);
		if (bind(sockid, (struct sockaddr*) & myaddr, len) == -1)
		{
			perror("bind");
		}
		if (listen(sockid, 10) == -1) 
		{
			perror("listen");
		}

		int pid, new;
		/*int numBlocs = 0;

		for (int x = 0; x < torrent.block_count; ++x)
		{
			if (torrent.block_map[x])
			{
				numBlocs++;
			}
		}*/

		int i = 0;

		for (;;)
		{
			new = accept(sockid, (struct sockaddr*) & clientaddr, &len);
			
			if ((pid = fork()) == -1)
			{
				close(new);
				continue;
			}
			else if (pid > 0)
			{
				close(new);
				continue;
			}
			else if (pid == 0)
			{

				i = 0;

				while(i < torrent.block_count)
				{

					uint8_t recibir[13];
					//memset(&recibir, '0', sizeof(recibir));

					/*printf("Recibido1 %d\n", recibir[0]);
					printf("Recibido2 %d\n", recibir[1]);
					printf("Recibido3 %d\n", recibir[2]);
					printf("Recibido4 %d\n", recibir[3]);
					printf("Recibido5 %d\n", recibir[4]);
					printf("Recibido6 %d\n", recibir[5]);
					printf("Recibido7 %d\n", recibir[6]);
					printf("Recibido8 %d\n", recibir[7]);
					printf("Recibido9 %d\n", recibir[8]);
					printf("Recibido10 %d\n", recibir[9]);
					printf("Recibido11 %d\n", recibir[10]);
					printf("Recibido12 %d\n", recibir[11]);
					printf("Recibido13 %d\n", recibir[12]);*/

					int bloque = -1;

					printf("pre recv\n");

					if(recv(new, &recibir, 13, 0) < 0)
					{
						printf("close recv\n");
						close(new);
					}
					else
					{
						bloque = recibir[5];
						printf("bloque %d\n", bloque);
						i = bloque;
						printf("i %d\n", i);
					}

					// COMPROBAR K EL MAGIN NUM ENCAJA recibir[0-1-2-3]

					if (bloque == -1)
					{
						close(new);
						break;
					}

					
					printf("blokmap %d\n", torrent.block_map[recibir[5]]);
					//printf("bloke %d\n", bloque);

					/*if (bloque == torrent.block_count - 1)
					{
						i = torrent.block_count;
					}*/

					if ((torrent.block_map[bloque]) && (recibir[4] == 0))
					{
				    	struct block_t block;
				  
				        if (load_block(&torrent, bloque, &block))
				        {
				           printf("error load\n");
				        }

				  		if (bloque == torrent.block_count - 1)
				  		{
				  			int tamano = (torrent.downloaded_file_size - (bloque * 65536));

				  			uint8_t mandar[13];
							uint64_t bnum = bloque;
							uint8_t *puntM = NULL;
							
							// Falta por comprobar si el magic number es el mismo que el que se envia por el cliente, necesario??

							u8from32i(mandar,MAGIC_NUMBER);
							mandar[4]=MSG_RESPONSE_OK;
							u8from64i(mandar,bnum);

							puntM = mandar;
							send(new, puntM, 13, 0);
					  		send(new, &block, tamano, 0);

					  		//close(new);
				  		}
				  		else{

				  			uint8_t mandar[13];
							uint64_t bnum = bloque;
							uint8_t *puntM = NULL;
							
							u8from32i(mandar,MAGIC_NUMBER);
							mandar[4]=MSG_RESPONSE_OK;
							u8from64i(mandar,bnum);

							puntM = mandar;
							send(new, puntM, 13, 0);
					  		send(new, &block, 65536, 0);
				  		}	
				    }
				    else
				    {
				    	uint8_t mandar[13];
						uint64_t bnum = bloque;
						uint8_t *puntM = NULL;
						
						u8from32i(mandar,MAGIC_NUMBER);
						mandar[4]=MSG_RESPONSE_NA;
						u8from64i(mandar,bnum);

						puntM = mandar;
						send(new, puntM, 13, 0);

						printf("El bloque no se encuentra en el servidor. \n");
				    }


				    printf("pasa while \n");
					i++;

					/*if (i == torrent.block_count)
					{
						printf("cierre new DW");
						close(new);
						break;
					}*/

				}

				printf("cierre new");
				close(new);
				break;

			}

		}
		close(sockid);
		return 0;
	}
	else if (strcmp(argv[1],".ttorrent") != NULL)
	{

	printf("El programa se va a ejecutar como cliente \n");

	int j = 0;
	int puerto;
	struct torrent_t torrent;
	if (create_torrent_from_metainfo_file("test_file.ttorrent", &torrent, "download_file")) {
		perror("CreateTorrent");
	}

	struct sockaddr_in myaddr, serveraddr;
	int sockid;
	
	memset(&myaddr, '0', sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	
	int errorPeer = 1;
	int primerBloque = 0;

	int r;
	while(j < torrent.peer_count){ //torrent.peer_count

		int i = 0;
		sockid = socket(AF_INET, SOCK_STREAM, 0);
		puerto = htons(torrent.peers[j].peer_port);
		myaddr.sin_port = htons(puerto);
		myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		if (sockid == -1)
		{
			perror("socket");
		}
		int len = sizeof(myaddr);
		if (connect(sockid, (const struct sockaddr*) & myaddr, len) == -1)
		{
			errorPeer=0;
			//perror("connect");
			printf("La conexion con el puerto %d", puerto);
			printf(" no dispone de un Server activo\n");
		}
		else
		{
			errorPeer=1;
		}

		while(i < torrent.block_count){
			if (errorPeer == 1)
			{
				if (torrent.block_map[i]) {
					printf("El bloque %d", i);	
					printf(" ya esta descargado.\n");
					primerBloque = 1;
				}
				else{
					uint8_t recibir[13];
					uint8_t mandar3[13];
					uint64_t bnum3 = i;
					uint8_t *puntM3 = NULL;
					
					u8from32i(mandar3,MAGIC_NUMBER);
					mandar3[4]=MSG_REQUEST;
					u8from64i(mandar3,bnum3);

					puntM3 = mandar3;
					send(sockid, puntM3, 13, 0);

					printf("Recibiendo el bloque %d", i);	
					printf("...\n");

					if (i == 0 )
					{
						
						struct block_t another_block;
						recv(sockid, &recibir, 13, 0);
						recv(sockid, &another_block.data, 65536, 0); 
						recv(sockid, &another_block.data[65523], 13, 0); 

						another_block.size = sizeof(another_block.data);

						if (recibir[4] == 1)
						{
						r = store_block(&torrent, 0, &another_block);
						}
						primerBloque = 0;
					}
					else
					{
						if (torrent.block_count - 1 == i && primerBloque == 0)
						{	
							struct block_t another_block;
							
							int tamano = (torrent.downloaded_file_size - (i * 65536));

							another_block.size = tamano;

							recv(sockid, &recibir, 13, 0);
							recv(sockid, &another_block.data, tamano, 0);
							//recv(sockid, &another_block.data[tamano], 13, 0); 

							if (recibir[4] == 1)
							{
							r = store_block(&torrent, i , &another_block);
							}
							primerBloque = 0;
						}
						if (torrent.block_count - 1 == i && primerBloque == 1)
						{	
							struct block_t another_block;
							recv(sockid, &recibir, 13, 0);

							int tamano = (torrent.downloaded_file_size - (i * 65536));

							another_block.size = tamano;

							recv(sockid, &another_block.data, tamano, 0);

							if (recibir[4] == 1)
							{
							r = store_block(&torrent, i , &another_block);
							}
							primerBloque = 0;
						}
						if (torrent.block_count - 1 != i && primerBloque == 0)
						{
							struct block_t another_block;
							recv(sockid, &recibir, 13, 0);
							recv(sockid, &another_block.data, 65536, 0);
							
							another_block.size = sizeof(another_block.data);

							if (recibir[4] == 1) 
							{
							r = store_block(&torrent, i , &another_block); 
							}

						}
						if (torrent.block_count - 1 != i && primerBloque == 1)
						{
							struct block_t another_block;
							recv(sockid, &recibir, 13, 0);
							recv(sockid, &another_block.data, 65536, 0);
							recv(sockid, &another_block.data[65523], 13, 0);
							
							another_block.size = sizeof(another_block.data);

							if (recibir[4] == 1) 
							{
							r = store_block(&torrent, i , &another_block); 
							}
							primerBloque == 0;
						}
					}


					if (r) {
				     	printf("Error en la descarga o almacenaje del bloque.\n");
					}
					else {
				    	printf("Bloque descargado y almacenado con exito.\n");
						assert(torrent.block_map[i]);
					}
				}	
			}
			i = i+1;
		}

		close(sockid);
		j = j+1;
	}
	
	if (destroy_torrent(&torrent)) 
	{
		perror("DestruirTorrent");
	}

	sleep(10);
	//close(sockid);
	return 0;

	}

