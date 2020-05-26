# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import socket
import sys
import random
import numpy as np
from tablero import TableroV2
from tablerov1 import TableroV1



def main(modo, tablero, fichas, fichasJugador, fichasRobot):
    
    Final = False
    robado = False
    while Final == False:
        
        valida = False
        

        
        while valida == False:
            connection.sendall("Tus fichas son: \n".encode())
            connection.sendall(str(fichasJugador).encode())
            connection.sendall("Selecciona una ficha (numero de izquierda a derecha)\n".encode())
            valor = connection.recv(16)
        

            if int(valor) > len(fichasJugador):
                connection.sendall(("Numero incorrecto, solo tienes "+str(len(fichasJugador))+" fichas\n").encode())
            else: valida = True
            
            if int(modo) == 1 and valida == True:
                res = False
                
                coord = 0
                if len(tablero.getFichas()) > 0:
                    
                    connection.sendall("1 - Añadir al principio de la cadena\n".encode())
                    connection.sendall("2 - Añadir al final de la cadena\n".encode())
                    coord = connection.recv(16)
                        
                res = tablero.anadirFicha(fichasJugador[int(valor)-1], int(coord))
                    
                
                if res == False:
                    valida = False
                    connection.sendall("Movimiento incorrecto\n".encode())
                else:
                    connection.sendall("Movimiento correcto\n".encode())
                    fichasJugador.remove(fichasJugador[int(valor)-1])
                    
                

                
            connection.sendall("TABLERO:\n".encode())
            tablero.printTablero()
            connection.sendall("--------------------------\n".encode())
            


            

        if len(fichasJugador) == 0:
            connection.sendall("HAS GANADO\n".encode())
            Final = True
            
                
        
       
        
        
        extremos = tablero.getExtremos()

        connection.sendall("Fichas robot: \n".encode())
        connection.sendall(str(fichasRobot).encode())
        connection.sendall("--------------------------------\n".encode())
        
        posficha, extremo, fichasRobot, fi = decisioJugada(fichasRobot, tablero.getFichas(), extremos, fichas, len(fichasJugador), robado)
        
        robado = False
        if extremo > 0:
            extremo = 2
        else: extremo = 1
        
        res = tablero.anadirFicha(fichasRobot[posficha], extremo)
        
        
        if res == False:
            connection.sendall("ERROR\n".encode())
        else: 
            connection.sendall(("El robot ha añadido la ficha "+str(fichasRobot[posficha])).encode())
            fichasRobot.remove(fichasRobot[posficha])  
        
        connection.sendall(("El robot tiene "+str(len(fichasRobot))+ " fichas\n").encode())
        extremos = tablero.getExtremos()
            
                   
        robar = True
        
        while robar == True:
            
            for k in range(len(extremos)):
                
                for s in range(len(fichasJugador)):
                    
                    if extremos[k][0] == fichasJugador[s][0] or extremos[k][0] == fichasJugador[s][1]:
                        robar = False
         
            if robar == True:
                ficha, fi = robarFicha(fichas)
                if fi == False:
                    fichasJugador.append(ficha)
                    fichas.remove(ficha)
                else: robar = False
                robado = True
                connection.sendall("No tienes fichas disponibles, se ha robado una\n".encode())    
                
        if fi == True:
            Final = True
            jugador = 0
            robot = 0
            for k in fichasJugador:
                jugador += k[0]
                jugador += k[1]
            for k in fichasRobot:
                robot += k[0]
                robot += k[1]
            
            if jugador < robot:
                connection.sendall("HAS GANADO\n".encode())
                exit
            else:
                connection.sendall("HAS PERDIDO\n".encode())  
                exit
        
        

            
        connection.sendall("TABLERO:\n".encode())
        tablero.printTablero()
        connection.sendall("---------------------------\n".encode())
        

        
        if len(fichasRobot) == 0:
            connection.sendall("HAS PERDIDO\n".encode())
            Final = True
            

            
                   

def decisioJugada(fichasRobot, fichasTablero, extremos, fichas, nfichasjug, robado):
    
    posExtremos = 0
    posFicha = 0
    acaba = False
    peso = [[],[],[],[]]
    #mas puntos, ficha doble, extremos antes, extremos despues
    if len(fichasRobot) > 5:
        peso[0] = 8
        peso[1] = 7
        peso[2] = 5
    else:
        peso[0] = 4
        peso[1] = 5
        peso[2] = 10
 
    if len(fichasRobot) - nfichasjug > 1:
        peso[3] = 10
        
    else:
        peso[3] = 5
    
    fi = False
    while acaba == False:
        heuristicas = np.zeros((len(fichasRobot), len(extremos)))
        for j, k in enumerate(extremos):
            
            #miramos cuantas fichas con ese extremo hay en fichasrobot
            count = 0
            """for f in fichasTablero:
                if f[0] == k[0] or f[1] == k[0]:
                    count += 1"""
                    
                    
            for f in fichasRobot:
                if f[0] == k[0] or f[1] == k[0]:
                    count += 1
            
            
            for i, ficha in enumerate(fichasRobot):
                
                #miramos si la ficha es valida para la jugada
                if ficha[0] == k[0] or ficha[1] == k[0]:
                    
                    prior = [[],[],[],[]]
                    
                    prior[0] = (ficha[0] + ficha[1]) / 12
                    
                    if ficha[0] == ficha[1]:
                        prior[1] = 1
                        
                    else: prior[1] = 0
                    
                    prior[2] = (count / (7))
                    
                    count2 = 0
                    if ficha[0] == k[0]:
                        num = ficha[1]
                    else: num = ficha[0]
                    
                    for f in fichasTablero:
                        if f[0] == num or f[1] == num:
                            count2 += 1
                    
                    for f in fichasRobot:
                        if f[0] == num or f[1] == num:
                            count2 += 1
                    
                    for e in extremos:
                        
                        if e[0] != k[0]:
                            for f in fichasTablero:
                                if f[0] == e[0] or f[1] == e[0]:
                                    count2 += 1
                    
                            for f in fichasRobot:
                                if f[0] == e[0] or f[1] == e[0]:
                                    count2 += 1
                    
                    prior[3] = (count2 / 14)
                            
                    
                    heuristicas[i][j] = (prior[0] * peso[0]) + (prior[1] * peso[1]) + (prior[2] * peso[2]) + (prior[3] * peso[3])
                            
                    

                        
        heuristicaMax = 0
    
        for i in range(len(fichasRobot)):
            for j in range(len(extremos)):
                
                if heuristicas[i][j] > heuristicaMax:
                    
                    heuristicaMax = heuristicas[i][j]
                    posFicha = i
                    posExtremos = j
        
    
        
        if heuristicaMax == 0:
            ficha, fi = robarFicha(fichas)
            if fi == False:
                connection.sendall("El robot ha robado una ficha\n".encode())
                fichas.remove(ficha)
                fichasRobot.append(ficha)
            else: acaba = True
        else:
            acaba = True
    #devolver ficha i extremo (jugada)

    return posFicha, extremos[posExtremos][1][0], fichasRobot, fi


def robarFicha(fichas):
    fi = False
    ficha = [9,9]
    
    if len(fichas) > 0:
        ficha = random.sample(fichas, 1)
        ficha = ficha[0]
    else:
        fi = True
    
    
    return ficha, fi


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 10000)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

sock.listen(1)

while True:
    print('waiting a connection')
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)
        while True:

            fichas = [[0,0],[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],
                      [1,1],[1,2],[1,3],[1,4],[1,5],[1,6],
                      [2,2],[2,3],[2,4],[2,5],[2,6],
                      [3,3],[3,4],[3,5],[3,6],
                      [4,4],[4,5],[4,6],
                      [5,5],[5,6],
                      [6,6]]



            fichasRobot = random.sample(fichas, 7)

            for k in fichasRobot:
                fichas.remove(k)

            fichasJugador = random.sample(fichas, 7)

            for k in fichasJugador:
                fichas.remove(k)






            valido = False


            while valido == False:
                
                connection.sendall("--------------PANTALLA PRINCIPAL-------------\n".encode())
                connection.sendall("1-Modo de juego clasico\n".encode())
                connection.sendall("2-Modo de juego con multiples extremos\n".encode())
                
                modo = connection.recv(16)
                
                if int(modo) == 1:
                    valido = True
                    tablero = TableroV1(connection)
                    
                elif int(modo) == 2:
                    valido = True
                    tablero = TableroV2(10, 10, connection)
                    
                else:
                    connection.sendall("Modo incorrecto\n".encode())
                  
                    


            main(modo, tablero, fichas, fichasJugador, fichasRobot)
    
    finally:
        connection.close()