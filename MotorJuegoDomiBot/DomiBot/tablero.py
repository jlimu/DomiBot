# -*- coding: utf-8 -*-
"""
Created on Tue Apr 14 18:41:11 2020

@author: Sergi Pérez
"""
import numpy as np

class TableroV2:
    #Tablero para el modo de juego 2
    
    
    #inicializa el tablero
    def __init__(self, x, y):
        
        self.__tablero = np.empty((x,y), tuple)
        self.__fichas = []
        
        for i in range(self.__tablero.shape[0]):
            for j in range(self.__tablero.shape[1]):
                self.__tablero[i][j] = (9,9)
                
    
    #añadir una ficha al tablero de Domino
    def añadirFicha(self, ficha, i, j):
        
        vacio = tuple((9,9))
        up = vacio
        down = vacio
        right = vacio
        left = vacio
        
        if i-1 > 0:
            up = self.__tablero[i-1][j]
            
        if i+1 < self.__tablero.shape[0]:
            down = self.__tablero[i+1][j]
            
        if j-1 > 0:
            left = self.__tablero[i][j-1]
            
        if j+1 < self.__tablero.shape[1]:
           right = self.__tablero[i][j+1]
            
        
        if up == vacio and down == vacio and right == vacio and left == vacio:
            
            if len(self.__fichas) == 0:
                self.__tablero[i][j] = tuple(ficha)
                self.__fichas.append(tuple(ficha))
                return True
            
            else: return False
        
        if up != vacio or left != vacio:
            
            if up[1] == ficha[0] or left[1] == ficha[0]:
                
                self.__tablero[i][j] = tuple(ficha)
                self.__fichas.append(tuple(ficha))
                return True
            
            elif up[1] == ficha[1] or left[1] == ficha[1]:
                
                ficha = ficha[::-1]
                self.añadirFicha(ficha, i, j)
                
            else: return False
            
        if down != vacio or right != vacio:
            
            if down[0] == ficha[1] or right[0] == ficha[1]:
                
                self.__tablero[i][j] = tuple(ficha)
                self.__fichas.append(tuple(ficha))
                return True
            
            elif down[1] == ficha[1] or right[1] == ficha[1]:
                
                ficha = ficha[::-1]
                self.añadirFicha(ficha, i, j)
                
            else: return False
                
            
            
            
    
        
    #devuelve los numeros con los que el robot puede hacer jugada
    #y la coordenada del tablero donde la debe colocar respetando los limites del tablero
    def getExtremos(self):
        
        extremos = []
        vacio = tuple((9,9))
        
        for i in range(self.__tablero.shape[0]):
            for j in range(self.__tablero.shape[1]):
                
                fichaAct = self.__tablero[i][j]
                if fichaAct != vacio:
                    
                    adj = 0
                    vacio = tuple((9,9))
                    up = vacio
                    down = vacio
                    right = vacio
                    left = vacio
                    
                    if i-1 > 0:
                        up = self.__tablero[i-1][j]
                        
                    if i+1 < self.__tablero.shape[0]:
                        down = self.__tablero[i+1][j]
                        
                    if j-1 > 0:
                        left = self.__tablero[i][j-1]
                        
                    if j+1 < self.__tablero.shape[1]:
                        right = self.__tablero[i][j+1]
                        
                    for k in list([up, down, left, right]):
                        if k != vacio:
                            adj += 1
                            
                    if adj == 1 or fichaAct[0] == fichaAct[1]:
    
                    
                       if j+1 < self.__tablero.shape[1]:
                           
                           if right == vacio and (left != vacio or fichaAct[0] == fichaAct[1]):
                               extremos.append([fichaAct[1], [i, j+1]])
                               
                       elif fichaAct[0] != fichaAct[1]:  
                           extremos.append([fichaAct[1], [i+1, j]])
                           
                    
                       
                       if j-1 >= 0:
                           
                           if left == vacio and (right != vacio or fichaAct[0] == fichaAct[1]):
                               extremos.append([fichaAct[0], [i, j-1]])
                               
                       elif fichaAct[0] != fichaAct[1]: 
                           extremos.append([fichaAct[0], [i-1, j]])
                       
                       
                       if i+1 < self.__tablero.shape[0]:
                               
                           if down == vacio and (up != vacio or fichaAct[0] == fichaAct[1]):
                               extremos.append([fichaAct[1], [i+1, j]])
                               
                       elif fichaAct[0] != fichaAct[1]:
                           extremos.append([fichaAct[1], [i, j+1]])
                           
                       if i-1 >= 0:
                           
                           if up == vacio and (down != vacio or fichaAct[0] == fichaAct[1]):
                               
                               extremos.append([fichaAct[0], [i-1, j]])
                       elif fichaAct[0] != fichaAct[1]:
                           extremos.append([fichaAct[0], [i, j-1]])
                    
                    if adj == 0:
                        
                        extremos.append([fichaAct[0], [i, j-1]])
                        extremos.append([fichaAct[1], [i, j+1]])
                       
                       
                       
                       
        return extremos
                       
                       
                       
                       
                    
    
    def getShape(self):
        
        return self.__tablero.shape
    
    
    def getFichas(self):
        
        return self.__fichas
    
    
    def printTablero(self):
        
        for i in range(self.__tablero.shape[0]):
       
            print(i, self.__tablero[i])
    
    
    
                
                

    