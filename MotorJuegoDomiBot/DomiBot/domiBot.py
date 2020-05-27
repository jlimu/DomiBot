# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import random
import numpy as np
from tablero import TableroV2
from tablerov1 import TableroV1

def main(modo, tablero, fichas, fichasJugador, fichasRobot):
    
    Final = False
    while Final == False:
        
        valida = False
        

        
        while valida == False:
            print("Tus fichas son: ")
            print(fichasJugador)
            print("Selecciona una ficha (numero de izquierda a derecha)")
            valor = input()
        

            if int(valor) > len(fichasJugador):
                print("Numero incorrecto, solo tienes "+str(len(fichasJugador))+" fichas")
            else: valida = True
            
            if int(modo) == 1 and valida == True:
                res = False
                
                coord = 0
                if len(tablero.getFichas()) > 0:
                    
                    print("1 - Añadir al principio de la cadena")
                    print("2 - Añadir al final de la cadena")
                    coord = input()
                        
                res = tablero.añadirFicha(fichasJugador[int(valor)-1], int(coord))
                    
                
                if res == False:
                    valida = False
                    print("Movimiento incorrecto")
                else:
                    print("Movimiento correcto")
                    fichasJugador.remove(fichasJugador[int(valor)-1])
                    
                

                
            print("TABLERO:")
            tablero.printTablero()
            print("--------------------------")
            


            

        if len(fichasJugador) == 0:
            print("HAS GANADO")
            Final = True
            
                
        
       
        if Final == False:
        
            extremos = tablero.getExtremos()
    
            
            posficha, extremo, fichasRobot, fi = decisioJugada(fichasRobot, tablero.getFichas(), extremos, fichas, len(fichasJugador))
            
            if extremo > 0:
                extremo = 2
            else: extremo = 1
            
            res = tablero.añadirFicha(fichasRobot[posficha], extremo)
            
            
            if res == False:
                print("ERROR")
            else: 
                print("El robot ha añadido la ficha "+str(fichasRobot[posficha]))
                fichasRobot.remove(fichasRobot[posficha])  
            
            print("El robot tiene "+str(len(fichasRobot))+ " fichas")
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
        
                    print("No tienes fichas disponibles, se ha robado una")    
                    
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
                    print("HAS GANADO")
                    exit
                else:
                    print("HAS PERDIDO")  
                    exit
            else:
                
                if len(fichasRobot) == 0:
                    print("HAS PERDIDO")
                    Final = True
            
        
        
            
        print("TABLERO:")
        tablero.printTablero()
        print("---------------------------")
        

        

            

            
                   

def decisioJugada(fichasRobot, fichasTablero, extremos, fichas, nfichasjug):
    
    posExtremos = 0
    posFicha = 0
    acaba = False
    peso = [[],[],[],[]]
    
    #ESTRATEGIAS
    #0- peso de las fichas
    #1- la ficha es doble
    #2- numero de veces que el extremo aparece en fichasRobot
    #3- numero de veces que los nuevos extremos aparecen en el tablero i fichasRobot
   
    if len(fichasRobot) > 4:
        peso[0] = 3
        peso[1] = 2
        peso[2] = 5
    else:
        peso[0] = 2
        peso[1] = 1
        peso[2] = 10
 
    if len(fichasRobot) - nfichasjug >= 1:
        peso[3] = 10
        
    else:
        peso[3] = 5
    
    fi = False
    while acaba == False:
        heuristicas = np.zeros((len(fichasRobot), len(extremos)))
        for j, k in enumerate(extremos):
            
            #miramos cuantas fichas con ese extremo hay en fichasrobot
            count = 0

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
                print("El robot ha robado una ficha")
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
    
    print("--------------PANTALLA PRINCIPAL-------------")
    print("1-Modo de juego clasico")
    print("2-Modo de juego con multiples extremos")
    
    modo = input()
    
    if int(modo) == 1:
        valido = True
        tablero = TableroV1()
        
    elif int(modo) == 2:
        valido = True
        tablero = TableroV2(10, 10)
        
    else:
        print("Modo incorrecto")
      
        


main(modo, tablero, fichas, fichasJugador, fichasRobot)


        
                    
    




